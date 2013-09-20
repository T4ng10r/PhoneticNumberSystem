#include <Data/CSubstituteSearch.h>
#include <Data/CAppSettings.h>
#include <Data/CAppSettings.h>
#include <tools/loggers.h>
#include <boost/algorithm/string.hpp>
#include <boost/foreach.hpp>
#include <Data/SearchResultTreeNode.h>

class CSubstituteSearchPrivate
{
public:
     CSubstituteSearchPrivate(CSubstituteSearch * ptrPublic);
     ~CSubstituteSearchPrivate();
	 bool testWord(const std::string & ,const CSingleSubstituteDigitsConfiguration & digitConf);
	 void prepareSearchResults();
public:
	CSubstituteSearch *                    m_ptrPublic;
	boost::shared_ptr<CDictionaryData>		dictionaryWords;
	std::string			number;
	WordSearchResult				searchResult;
	WordSearchResult				searchCandidates;
	FittingWordsMap					searchResultMap;
	SearchResultTreeNode			searchResultTreeRoot;
};

CSubstituteSearchPrivate::CSubstituteSearchPrivate(CSubstituteSearch * ptrPublic):m_ptrPublic(ptrPublic)
{}
CSubstituteSearchPrivate::~CSubstituteSearchPrivate(){}
bool CSubstituteSearchPrivate::testWord(const std::string & wordToTest,const CSingleSubstituteDigitsConfiguration & digitConf)
{
	SuccessWord	result;
	MatchingPair matchingPair;
	std::string  coveredDigits;
	size_t searchStartPos(0);
	unsigned int digitIndex(0);
	std::string word = boost::to_upper_copy(wordToTest);
	result.bFullCoverage=true;
	for(;digitIndex<number.size();digitIndex++)
	{
		unsigned int digit = number[digitIndex]-'0';
		//test 
		OneDigitConsonantsSet::const_iterator iter = digitConf.digitsConsonantsSetMap.find(digit);
		size_t acceptPos = word.find_first_of(iter->second.first,searchStartPos);
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
		//ok, expect subst char found
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
	if (word.find_first_of(digitConf.allConsonants,searchStartPos)==std::string::npos)
	{
		result.words.push_back(wordToTest);
		if (digitIndex>=number.size())
			digitIndex=number.size()-1;
		matchingPair.second=digitIndex;
		result.coveragePairs.push_back(matchingPair);
		searchResultMap[matchingPair].push_back(result);
		return true;
	}
	return false;
}
void CSubstituteSearchPrivate::prepareSearchResults()
{
	std::size_t searchedNumberLength = number.size();
	searchResult.clear();
	searchResultTreeRoot.clear();

	BOOST_FOREACH(const FittingWordsMap::value_type & resultItem, searchResultMap)
	{
		std::list<boost::shared_ptr<SearchResultTreeNode> > foundNodes;
		searchResultTreeRoot.find_node(resultItem.first.first, foundNodes);
		for( boost::shared_ptr<SearchResultTreeNode> foundNode : foundNodes)
		{
			boost::shared_ptr<SearchResultTreeNode> nodeToAdd(new SearchResultTreeNode);
			nodeToAdd->iCurrentIndex=resultItem.first.second;
			nodeToAdd->parent=foundNode;
			foundNode->children[nodeToAdd->iCurrentIndex.get()]=nodeToAdd;
		}
	}
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
	Q_EMIT searchProgress(0);
	for(unsigned int index=0;index<wordsCount;index++,notifyStep++)
	{
		std::string word = privPart->dictionaryWords->getWordByNdex(index);
		privPart->testWord(word,digitConf[0]);
		if (notifyStep==notifyStepCount)
		{
			Q_EMIT searchProgress(index+1);
			notifyStep=0;
		}
	}
	Q_EMIT searchProgress(wordsCount);
	privPart->prepareSearchResults();
	Q_EMIT searchFinished();
	printLog(eInfoLogLevel,eDebug,QString("Searching substitute for number '%1' finished").arg(number.c_str()));
}
const WordSearchResult & CSubstituteSearch::getSearchResult()
{
	return privPart->searchResult;
}
