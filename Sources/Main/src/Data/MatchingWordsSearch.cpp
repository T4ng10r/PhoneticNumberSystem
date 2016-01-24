#include <Data/MatchingWordsSearch.h>
#include <Data/SearchResultTreeNode.h>
#include <Data/Settings.h>
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
    void buildSearchResultsTree();

    void          clearSearchResult();
    void          prepareSearchResults();
    ConsonantsPos getConsonatsPos(const std::string& word, unsigned int digit, size_t start);
    void setStartDigitIndex(MatchingWord& result, size_t digitIndex);
    void setEndDigitIndex(MatchingWord& result, size_t digitIndex);
    void addMatchingLetter(MatchingWord& result, char letter);

  public:
    MatchingWordsSearch*                 m_ptrPublic;
    boost::shared_ptr<CDictionaryData>   dictionary;
    std::string                          number;
    WordsList                            searchResult;
    WordsList                            searchCandidates;
    FittingWordsMap                      searchResultMap;
    SharedTreeNodes                      searchResultTreeRoot;
    CSingleSubstituteDigitsConfiguration digits_conf;
};

MatchingWordsSearchPrivate::MatchingWordsSearchPrivate(MatchingWordsSearch* ptrPublic)
    : m_ptrPublic(ptrPublic)
    , searchResultTreeRoot(new SearchResultTreeNode(0))
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

void MatchingWordsSearchPrivate::setStartDigitIndex(MatchingWord& result, size_t digitIndex)
{
    if (result.coveredDigitsIndices.back().startIndex == std::string::npos)
        result.coveredDigitsIndices.back().startIndex = digitIndex;
}

void MatchingWordsSearchPrivate::setEndDigitIndex(MatchingWord& result, size_t digitIndex)
{
    if (result.coveredDigitsIndices.back().startIndex != std::string::npos) {
        result.coveredDigitsIndices.back().endIndex = digitIndex;
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
    MatchingPair  matchingPair;
    std::string   coveredDigits;
    size_t        searchStartPos(0);
    StartingIndex digitIndex(0);
    std::string   word = boost::to_upper_copy(tested_word);
    result.coveredDigitsIndices.push_back(MatchingPair());
    do {
        for (; digitIndex < searched_number.size(); digitIndex++) {
            unsigned int digit = searched_number[digitIndex] - '0';
            // test
            ConsonantsPos pos = getConsonatsPos(word, digit, searchStartPos);
            // if both result are end - we reached end of word or no chars in
            if (pos.acceptable == std::string::npos && pos.forbiden == std::string::npos) {
                if (result.coveredDigitsIndices.back().startIndex == std::string::npos)
                    result.coveredDigitsIndices.pop_back();
                else {
                    setEndDigitIndex(result, digitIndex - 1);
                    result.coveredDigitsIndices.push_back(MatchingPair());
                    searchStartPos = 0;
                    coveredDigits.clear();
                }
                break;
            }
            // if forbidden char is before accept char - we have rejection candidate
            else if (pos.acceptable > pos.forbiden) {
                setEndDigitIndex(result, digitIndex);
                coveredDigits.clear();
                result.bFullCoverage = false;
            }
            // ok, expect subset char found
            else if (pos.acceptable < pos.forbiden) {
                // if (result.matchingLetters.empty())
                //    matchingPair.startIndex = digitIndex;
                setStartDigitIndex(result, digitIndex);
                addMatchingLetter(result, word.at(pos.acceptable));
                coveredDigits.push_back(searched_number[digitIndex]);
                searchStartPos = pos.acceptable + 1;
            }
            if (digitIndex + 1 == searched_number.size()) {
                // if this is last tested digit - check if in word appear any more of acceptable consontant
                if (tested_word.find_first_of(digits_conf.allConsonants, searchStartPos) == std::string::npos)
                    result.coveredDigitsIndices.back().endIndex = digitIndex;
                else
                    result.coveredDigitsIndices.pop_back();
            }
        }
    } while (digitIndex < searched_number.size());
    result.bFullCoverage = (coveredDigits == searched_number);

    if (result.coveredDigitsIndices.size() == 0)
        return boost::optional<MatchingWord>();
    else
        return boost::optional<MatchingWord>(result);
}

void MatchingWordsSearchPrivate::clearSearchResult()
{
    searchResult.clear();
    searchResultTreeRoot->clear();
    searchResultTreeRoot->iCurrentIndex = 0;
}

void MatchingWordsSearchPrivate::buildSearchResultsTree()
{
    clearSearchResult();

    printLog(eDebug, eInfoLogLevel, "Building tree of search results");
    BOOST_FOREACH (const FittingWordsMap::value_type& resultItem, searchResultMap) {
        const WordsList& searchResult = resultItem.second;
        StartingIndex    start        = resultItem.first.startIndex;
        StartingIndex    end          = resultItem.first.endIndex;
        std::for_each(searchResult.begin(), searchResult.end(),
                      boost::bind(&SearchResultTreeNode::addNode, searchResultTreeRoot, start, end, _1));
    }
}

void MatchingWordsSearchPrivate::prepareSearchResults()
{
    printLog(eDebug, eInfoLogLevel, "Building tree of search results");
    buildSearchResultsTree();
    printLog(eDebug, eInfoLogLevel, "Gathering processed search results");
    // searchResult = searchResultTreeRoot->parseDFS(number.length());
}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
MatchingWordsSearch::MatchingWordsSearch()
    : privPart(new MatchingWordsSearchPrivate(this))
{
}

MatchingWordsSearch::~MatchingWordsSearch(void) {}

void MatchingWordsSearch::setSubstituteDigitsConfiguration(CSingleSubstituteDigitsConfiguration conf)
{
    privPart->digits_conf = conf;
}

void MatchingWordsSearch::setDictionaryWords(boost::shared_ptr<CDictionaryData> dictionary)
{
    privPart->searchResultMap.clear();
    privPart->dictionary = dictionary;
}

void MatchingWordsSearch::startSearchForNumber(const std::string& number)
{
    printLog(eDebug, eInfoLogLevel, str(boost::format("Searching substitute for number '%1%' started") % number));
    privPart->clearSearchResult();
    privPart->number             = number;
    unsigned int words_count     = privPart->dictionary->getWordsCount();
    unsigned int notifyStepCount = words_count / 100;
    unsigned int notifyStep      = 0;
    Q_EMIT       searchProgress(0, words_count);
    for (unsigned int index = 0; index < words_count; index++, notifyStep++) {
        std::string word = privPart->dictionary->get_word_by_index(index);
        if (word.size() < 2)
            continue;
        privPart->testWord(word, number);
        if (notifyStep == notifyStepCount) {
            Q_EMIT searchProgress(index + 1, words_count);
            notifyStep = 0;
        }
    }
    Q_EMIT searchProgress(words_count, words_count);
    privPart->buildSearchResultsTree();
    printLog(eDebug, eInfoLogLevel, str(boost::format("Searching substitute for number '%1%' finished") % number));
    Q_EMIT searchFinished(true);
}

WordsList MatchingWordsSearch::getSearchResult(StartingIndex start_index)
{
    WordsList     searchResult;
    TreeNodesList nodes_list = privPart->searchResultTreeRoot->find_node(start_index);

    for (SharedTreeNodes tree_node : nodes_list) {
        for (ChildrenMap::value_type& child : tree_node->children) {
            std::copy(child.second.words_list().begin(), child.second.words_list().end(), back_inserter(searchResult));
        }
    }
    return searchResult;
}
