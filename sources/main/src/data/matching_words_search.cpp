#include <boost/algorithm/string.hpp> //boost::to_upper_copy
#include <boost/bind.hpp>
#include <boost/foreach.hpp>
#include <boost/format.hpp>
#include <boost/optional.hpp>
#include <data/logging_base.h>
#include <data/matching_words_search.h>
#include <data/settings.h>
#include <tools/loggers.h>

class MatchingWordsSearchPrivate : public LoggingBase
{
    struct ConsonantsPos {
        size_t acceptable;
        size_t forbiden;
    };

  public:
    MatchingWordsSearchPrivate(MatchingWordsSearch* ptrPublic);
    ~MatchingWordsSearchPrivate();
    boost::optional<MatchingWord> testWord(const std::string& tested_word, const std::string& searched_number);

    std::list<char> getConsonatsPos(const std::string& word);
    void setStartDigitIndex(MatchingWord& result, size_t digitIndex);
    void setEndDigitIndex(MatchingWord& result, size_t digitIndex);
    void addMatchingLetter(MatchingWord& result, char letter);
    void addMatchingWord(MatchingWord word);

  public:
    MatchingWordsSearch*                 m_ptrPublic;
    CSingleSubstituteDigitsConfiguration digits_conf;
    boost::shared_ptr<DictionaryData>    dictionary;
    std::string                          number;
    WordSearchResultMap                  searchResult;
};

MatchingWordsSearchPrivate::MatchingWordsSearchPrivate(MatchingWordsSearch* ptrPublic)
    : m_ptrPublic(ptrPublic)
    , LoggingBase()
{
}

MatchingWordsSearchPrivate::~MatchingWordsSearchPrivate() {}

std::list<char> MatchingWordsSearchPrivate::getConsonatsPos(const std::string& word)
{
    StartingIndex   pos(0);
    std::list<char> result;
    pos = word.find_first_of(digits_conf.allConsonants, pos);
    while (pos != std::string::npos) {
        result.push_back(word.at(pos));
        pos = word.find_first_of(digits_conf.allConsonants, pos + 1);
    }
    return result;
}

void MatchingWordsSearchPrivate::setStartDigitIndex(MatchingWord& result, size_t digitIndex)
{
    if (result.coveredDigitsIndices.back().start_index == std::string::npos)
        result.coveredDigitsIndices.back().start_index = digitIndex;
}

void MatchingWordsSearchPrivate::setEndDigitIndex(MatchingWord& result, size_t digitIndex)
{
    if (result.coveredDigitsIndices.back().start_index != std::string::npos) {
        result.coveredDigitsIndices.back().end_index = digitIndex;
    }
}

void MatchingWordsSearchPrivate::addMatchingLetter(MatchingWord& result, char letter)
{
    if (result.coveredDigitsIndices.size() <= 1)
        result.matchingLetters.push_back(letter);
}

boost::optional<MatchingWord> MatchingWordsSearchPrivate::testWord(const std::string& tested_word,
                                                                   const std::string& searched_number)
{
    MatchingWord  result;
    StartingIndex digitIndex(0);
    std::string   word = boost::to_upper_copy(tested_word);
    result.coveredDigitsIndices.push_back(MatchingPair());
    result.words.push_back(tested_word);

    std::list<char>                 consonantsIndices = getConsonatsPos(word);
    std::list<char>::const_iterator iter              = consonantsIndices.begin();
    for (; digitIndex < word.size(); digitIndex++) {
        unsigned int       digit          = searched_number[digitIndex] - '0';
        ConsonantsForDigit consonantsSets = digits_conf.digitsConsonantsSetMap[digit];

        if (consonantsSets.acceptable.find_first_of(*iter) != std::string::npos) {
            setStartDigitIndex(result, digitIndex);
            addMatchingLetter(result, *iter);
            iter++;
            if (iter == consonantsIndices.end()) {
                setEndDigitIndex(result, digitIndex);
                iter = consonantsIndices.begin();
                result.coveredDigitsIndices.push_back(MatchingPair());
            }
        }
        // this means that we're in the middle of tested_word
        else if (iter != consonantsIndices.begin()) {
            return boost::optional<MatchingWord>();
        }
    }
    setEndDigitIndex(result, digitIndex - 1);
    result.bFullCoverage = (result.matchingLetters.size() == searched_number.size());
    return boost::optional<MatchingWord>(result);
}

void MatchingWordsSearchPrivate::addMatchingWord(MatchingWord word)
{
    for (std::list<MatchingPair>::iterator iter = word.coveredDigitsIndices.begin();
         iter != word.coveredDigitsIndices.end(); iter++) {
        MatchingWord item(word);
        item.coveredDigitsIndices.clear();
        item.coveredDigitsIndices.push_back(*iter);
        searchResult[iter->start_index].push_back(item);
    }
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
MatchingWordsSearch::MatchingWordsSearch(CSingleSubstituteDigitsConfiguration conf, DictionaryData::ptr dictionary)
    : _pimpl(new MatchingWordsSearchPrivate(this))
{
    _pimpl->digits_conf = conf;
    _pimpl->dictionary  = dictionary;
}

MatchingWordsSearch::~MatchingWordsSearch(void) {}

void MatchingWordsSearch::startSearchForNumber(const std::string& number)
{
    _pimpl->logger.log(log4cplus::INFO_LOG_LEVEL,
                       str(boost::format("Searching substitute for number '%1%' started") % number));
    _pimpl->searchResult.clear();
    _pimpl->number               = number;
    unsigned int words_count     = _pimpl->dictionary->getWordsCount();
    unsigned int notifyStepCount = words_count / 100;
    unsigned int notifyStep      = 0;
    Q_EMIT       searchProgress(0, words_count);
    for (unsigned int index = 0; index < words_count; index++, notifyStep++) {
        std::string word = _pimpl->dictionary->get_word_by_index(index);
        if (word.size() < 2)
            continue;
        boost::optional<MatchingWord> result = _pimpl->testWord(word, number);
        if (result)
            _pimpl->addMatchingWord(*result);

        if (notifyStep == notifyStepCount) {
            Q_EMIT searchProgress(index + 1, words_count);
            notifyStep = 0;
        }
    }
    Q_EMIT searchProgress(words_count, words_count);
    _pimpl->logger.log(log4cplus::INFO_LOG_LEVEL,
                       str(boost::format("Searching substitute for number '%1%' finished") % number));
    Q_EMIT searchFinished(true);
}

MatchingWordsList MatchingWordsSearch::getSearchResult(StartingIndex start_index, StartingIndex end_index)
{
    MatchingWordsList             result;
    WordSearchResultMap::iterator iter = _pimpl->searchResult.find(start_index);
    if (iter != _pimpl->searchResult.end()) {
        MatchingWordsList matching_words = iter->second;
        if (end_index == max_index)
            end_index = _pimpl->number.length();
        for (MatchingWordsList::const_iterator iter = matching_words.begin(); iter != matching_words.end(); iter++) {
          if (iter->matching_pair(start_index, end_index))
            result.push_back(*iter);
        }
    }
    return result;
}
