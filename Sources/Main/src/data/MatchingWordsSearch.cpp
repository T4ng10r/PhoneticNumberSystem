#include <data/MatchingWordsSearch.h>
#include <data/Settings.h>
#include <Tools/loggers.h>
#include <boost/algorithm/string.hpp> //boost::to_upper_copy
#include <boost/bind.hpp>
#include <boost/foreach.hpp>
#include <boost/format.hpp>
#include <boost/optional.hpp>

class MatchingWordsSearchPrivate
{
    struct ConsonantsPos {
        size_t acceptable;
        size_t forbiden;
    };

  public:
    MatchingWordsSearchPrivate(MatchingWordsSearch* ptrPublic);
    ~MatchingWordsSearchPrivate();
    boost::optional<MatchingWord> testWord(const std::string& tested_word, const std::string& searched_number);

    ConsonantsPos getConsonatsPos(const std::string& word, unsigned int digit, size_t start);
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
{
}

MatchingWordsSearchPrivate::~MatchingWordsSearchPrivate() {}

MatchingWordsSearchPrivate::ConsonantsPos MatchingWordsSearchPrivate::getConsonatsPos(const std::string& word,
                                                                                      unsigned int digit, size_t start)
{
    ConsonantsPos                         result;
    OneDigitConsonantsSet::const_iterator iter = digits_conf.digitsConsonantsSetMap.find(digit);
    result.acceptable                          = word.find_first_of(iter->second.acceptable, start);
    result.forbiden                            = word.find_first_of(iter->second.forbiden, start);
    return result;
}
std::list<char> MatchingWordsSearchPrivate::getConsonatsPos(const std::string& word)
{
  StartingIndex pos(0);
  std::list<char> result;
  pos = word.find_first_of(digits_conf.allConsonants,pos);
  while (pos!=std::string::npos)
  {
    result.push_back(word.at(pos));
    pos = word.find_first_of(digits_conf.allConsonants,pos+1);
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

    std::list<char> consonantsIndices = getConsonatsPos(word);
    std::list<char>::const_iterator iter = consonantsIndices.begin();
    for(;digitIndex<word.size();digitIndex++)
    {
      unsigned int digit = searched_number[digitIndex] - '0';
      ConsonantsForDigit consonantsSets = digits_conf.digitsConsonantsSetMap[digit];

      if (consonantsSets.acceptable.find_first_of(*iter)!=std::string::npos)
      {
         setStartDigitIndex(result, digitIndex);
         addMatchingLetter(result, *iter);
         iter++;
         if (iter==consonantsIndices.end())
         {
           setEndDigitIndex(result, digitIndex);
           iter = consonantsIndices.begin();
           result.coveredDigitsIndices.push_back(MatchingPair());
         }
      }
      //this means that we're in the middle of tested_word
      else if (iter!=consonantsIndices.begin())
      {
        return boost::optional<MatchingWord>();
      }
    }
    setEndDigitIndex(result, digitIndex-1);
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
MatchingWordsSearch::MatchingWordsSearch()
    : _pimpl(new MatchingWordsSearchPrivate(this))
{
}

MatchingWordsSearch::~MatchingWordsSearch(void) {}

void MatchingWordsSearch::setSubstituteDigitsConfiguration(CSingleSubstituteDigitsConfiguration conf)
{
    _pimpl->digits_conf = conf;
}

void MatchingWordsSearch::setDictionaryWords(boost::shared_ptr<DictionaryData> dictionary)
{
    _pimpl->dictionary = dictionary;
}

void MatchingWordsSearch::startSearchForNumber(const std::string& number)
{
    printLog(eDebug, eInfoLogLevel, str(boost::format("Searching substitute for number '%1%' started") % number));
    _pimpl->searchResult.clear();
    _pimpl->number             = number;
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
    printLog(eDebug, eInfoLogLevel, str(boost::format("Searching substitute for number '%1%' finished") % number));
    Q_EMIT searchFinished(true);
}

MatchingWordsList MatchingWordsSearch::getSearchResult(StartingIndex start_index)
{
    WordSearchResultMap::iterator iter = _pimpl->searchResult.find(start_index);
    if (iter != _pimpl->searchResult.end())
        return iter->second;
    return MatchingWordsList();
}