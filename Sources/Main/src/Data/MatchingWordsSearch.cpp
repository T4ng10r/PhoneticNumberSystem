#include <Data/MatchingWordsSearch.h>
#include <Data/SearchResultTreeNode.h>
#include <Data/Settings.h>
#include <Tools/loggers.h>
#include <boost/algorithm/string.hpp> //boost::to_upper_copy
#include <boost/bind.hpp>
#include <boost/foreach.hpp>
#include <boost/format.hpp>

class MatchingWordsSearchPrivate
{
  public:
    MatchingWordsSearchPrivate(MatchingWordsSearch* ptrPublic);
    ~MatchingWordsSearchPrivate();
    bool testWord(const std::string&);
    void buildSearchResultsTree();

    void clearSearchResult();
    void prepareSearchResults();

  public:
    MatchingWordsSearch*                 m_ptrPublic;
    boost::shared_ptr<CDictionaryData>   dictionaryWords;
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

bool MatchingWordsSearchPrivate::testWord(const std::string& wordToTest)
{
    MatchingWord  result;
    MatchingPair  matchingPair;
    std::string   coveredDigits;
    size_t        searchStartPos(0);
    StartingIndex digitIndex(0);
    std::string   word   = boost::to_upper_copy(wordToTest);
    result.bFullCoverage = true;
    size_t acceptPos;
    for (; digitIndex < number.size(); digitIndex++) {
        unsigned int digit = number[digitIndex] - '0';
        // test
        OneDigitConsonantsSet::const_iterator iter = digits_conf.digitsConsonantsSetMap.find(digit);
        acceptPos                                  = word.find_first_of(iter->second.first, searchStartPos);
        size_t forbPos                             = word.find_first_of(iter->second.second, searchStartPos);
        // if both result are end - we reached end of word or no chars in
        if (acceptPos == std::string::npos && forbPos == std::string::npos) {
            digitIndex--;
            break;
        }
        // if forbidden char is before accept char - we have rejection candidate
        if (acceptPos > forbPos) {
            result.matchingLetters.clear();
            coveredDigits.clear();
            result.bFullCoverage = false;
            //
            matchingPair = MatchingPair();
        }
        // ok, expect subset char found
        if (acceptPos < forbPos) {
            if (result.matchingLetters.empty())
                matchingPair.first = digitIndex;
            result.matchingLetters.push_back(word.at(acceptPos));
            coveredDigits.push_back(number[digitIndex]);
            searchStartPos = acceptPos + 1;
        }
    }
    result.bFullCoverage = (coveredDigits == number);
    if (word.find_first_of(digits_conf.allConsonants, searchStartPos) == std::string::npos && coveredDigits.size()) {
        result.words.push_back(wordToTest);
        if (digitIndex >= number.size())
            digitIndex      = number.size() - 1;
        matchingPair.second = digitIndex;
        result.coveragePairs.push_back(matchingPair);
        searchResultMap[matchingPair].push_back(result);
        if (!result.bFullCoverage) {
            while ((acceptPos = number.find(coveredDigits, matchingPair.second + 1)) != std::string::npos) {
                matchingPair.first           = acceptPos;
                matchingPair.second          = acceptPos + coveredDigits.size();
                result.coveragePairs.front() = matchingPair;
                searchResultMap[matchingPair].push_back(result);
            }
        }
        return true;
    }
    return false;
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
        StartingIndex    start        = resultItem.first.first;
        StartingIndex    end          = resultItem.first.second;
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

void MatchingWordsSearch::setDictionaryWords(boost::shared_ptr<CDictionaryData> dictionaryWords)
{
    privPart->searchResultMap.clear();
    privPart->dictionaryWords = dictionaryWords;
}

void MatchingWordsSearch::startSearchForNumber(const std::string& number)
{
    printLog(eDebug, eInfoLogLevel, str(boost::format("Searching substitute for number '%1%' started") % number));
    privPart->clearSearchResult();
    privPart->number             = number;
    unsigned int words_count     = privPart->dictionaryWords->getWordsCount();
    unsigned int notifyStepCount = words_count / 100;
    unsigned int notifyStep      = 0;
    Q_EMIT       searchProgress(0, words_count);
    for (unsigned int index = 0; index < words_count; index++, notifyStep++) {
        std::string word = privPart->dictionaryWords->get_word_by_index(index);
        if (word.size() < 2)
            continue;
        privPart->testWord(word);
        if (notifyStep == notifyStepCount) {
            Q_EMIT searchProgress(index + 1, words_count);
            notifyStep = 0;
        }
    }
    Q_EMIT searchProgress(words_count, words_count);
    privPart->buildSearchResultsTree();
    Q_EMIT searchFinished(true);
    printLog(eDebug, eInfoLogLevel, str(boost::format("Searching substitute for number '%1%' finished") % number));
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
