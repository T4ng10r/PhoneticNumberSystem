#include <tools/loggers.h>
#include <Data/CSubstituteSearch.h>
#include <Data/CAppSettings.h>
#include <Data/SearchResultTreeNode.h>
#include <boost/algorithm/string.hpp>	//boost::to_upper_copy
#include <boost/foreach.hpp>
#include <boost/bind.hpp>

class CSubstituteSearchPrivate
{
public:
     CSubstituteSearchPrivate(CSubstituteSearch * ptrPublic);
     ~CSubstituteSearchPrivate();
	 bool testWord(const std::string & ,const CSingleSubstituteDigitsConfiguration & digitConf);
	 void buildSearchResultsTree();

	 void clearSearchResult();
	 void prepareSearchResults();
public:
	CSubstituteSearch *                    m_ptrPublic;
	boost::shared_ptr<CDictionaryData>		dictionaryWords;
	std::string			number;
	WordSearchResult				searchResult;
	WordSearchResult				searchCandidates;
	FittingWordsMap					searchResultMap;
	SharedTreeNodes					searchResultTreeRoot;
};

CSubstituteSearchPrivate::CSubstituteSearchPrivate(CSubstituteSearch * ptrPublic):m_ptrPublic(ptrPublic),
	searchResultTreeRoot(new SearchResultTreeNode(0))
{

}
CSubstituteSearchPrivate::~CSubstituteSearchPrivate(){}
bool CSubstituteSearchPrivate::testWord(const std::string & wordToTest,const CSingleSubstituteDigitsConfiguration & digitConf)
{
	SuccessWord	result;
	MatchingPair matchingPair;
	std::string  coveredDigits;
	size_t searchStartPos(0);
	StartingIndex digitIndex(0);
	std::string word = boost::to_upper_copy(wordToTest);
	result.bFullCoverage=true;
	size_t acceptPos;
	for(;digitIndex<number.size();digitIndex++)
	{
		unsigned int digit = number[digitIndex]-'0';
		//test 
		OneDigitConsonantsSet::const_iterator iter = digitConf.digitsConsonantsSetMap.find(digit);
		acceptPos = word.find_first_of(iter->second.first,searchStartPos);
		size_t forbPos = word.find_first_of(iter->second.second,searchStartPos);
		//if both result are end - we reached end of word or no chars in 
		if (acceptPos == std::string::npos && forbPos == std::string::npos)
		{
			digitIndex--;
			break;
		}
		//if forbidden char is before accept char - we have rejection candidate
		if (acceptPos>forbPos)
		{
			result.matchingLetters.clear();
			coveredDigits.clear();
			result.bFullCoverage=false;
			//
			matchingPair = MatchingPair();
		}
		//ok, expect subset char found
		if (acceptPos<forbPos)
		{
			if (result.matchingLetters.empty())
				matchingPair.first=digitIndex;
			result.matchingLetters.push_back(word.at(acceptPos));
			coveredDigits.push_back(number[digitIndex]);
			searchStartPos = acceptPos+1;
		}
	}
	result.bFullCoverage=(coveredDigits==number);
	if (word.find_first_of(digitConf.allConsonants,searchStartPos)==std::string::npos && coveredDigits.size())
	{
		result.words.push_back(wordToTest);
		if (digitIndex>=number.size())
			digitIndex=number.size()-1;
		matchingPair.second=digitIndex;
		result.coveragePairs.push_back(matchingPair);
		searchResultMap[matchingPair].push_back(result);
		if (!result.bFullCoverage)
		{
			while((acceptPos = number.find(coveredDigits,matchingPair.second+1))!=std::string::npos);
			{
				matchingPair.first=acceptPos;
				matchingPair.second=acceptPos+coveredDigits.size();
				result.coveragePairs.front()=matchingPair;
				searchResultMap[matchingPair].push_back(result);
			}
		}
		return true;
	}
	return false;
}
void CSubstituteSearchPrivate::clearSearchResult() 
{
	searchResult.clear();
	searchResultTreeRoot->clear();
	searchResultTreeRoot->iCurrentIndex=0;
}
void CSubstituteSearchPrivate::buildSearchResultsTree()
{
	clearSearchResult();

	BOOST_FOREACH(const FittingWordsMap::value_type & resultItem, searchResultMap)
	{
		const WordSearchResult & searchResult = resultItem.second;
		StartingIndex start=resultItem.first.first;
		StartingIndex end=resultItem.first.second;
		std::for_each(searchResult.begin(),searchResult.end(),
      //boost::bind(&SearchResultTreeNode::addNode, searchResultTreeRoot, start,end, boost::bind(&SuccessWord::getWord, _1)));
      boost::bind(&SearchResultTreeNode::addNode, searchResultTreeRoot, start,end, _1));
	}
}
void CSubstituteSearchPrivate::prepareSearchResults()
{
	printLog(eInfoLogLevel,eDebug,QString("Building tree of search results"));
	buildSearchResultsTree();
	printLog(eInfoLogLevel,eDebug,QString("Gathering processed search results"));
	//searchResult = searchResultTreeRoot->parseDFS(number.length());
}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
CSubstituteSearch::CSubstituteSearch():privPart(new CSubstituteSearchPrivate(this))
{
}
CSubstituteSearch::~CSubstituteSearch(void){}
void CSubstituteSearch::setDictionaryWords(boost::shared_ptr<CDictionaryData> dictionaryWords)
{
	privPart->searchResultMap.clear();
	privPart->dictionaryWords = dictionaryWords;
}
void CSubstituteSearch::startSearchForNumber(const std::string & number)
{
	printLog(eInfoLogLevel,eDebug,QString("Searching substitute for number '%1' started").arg(number.c_str()));
	privPart->number = number;
	const std::vector<CSingleSubstituteDigitsConfiguration> & digitConf = gAppSettings->getDigitsConfiguraions();

	unsigned int wordsCount = privPart->dictionaryWords->getWordsCount();
	unsigned int notifyStepCount = wordsCount/100;
	unsigned int notifyStep = 0;
	Q_EMIT searchProgress(0, wordsCount);
	for(unsigned int index=0;index<wordsCount;index++,notifyStep++)
	{
		std::string word = privPart->dictionaryWords->getWordByNdex(index);
		if (word.size()<2)
			continue;
		privPart->testWord(word,digitConf[0]);
		if (notifyStep==notifyStepCount)
		{
			Q_EMIT searchProgress(index+1,wordsCount);
			notifyStep=0;
		}
	}
	Q_EMIT searchProgress(wordsCount,wordsCount);
	privPart->buildSearchResultsTree();
	Q_EMIT searchFinished();
	printLog(eInfoLogLevel,eDebug,QString("Searching substitute for number '%1' finished").arg(number.c_str()));
}
WordSearchResult CSubstituteSearch::getSearchResult(int start_index)
{
	return WordSearchResult();
	if (start_index == 0)
	{
		//return privPart->searchResult;
	}
}
