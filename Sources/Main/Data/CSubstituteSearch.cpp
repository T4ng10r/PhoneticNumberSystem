#include <Data/CSubstituteSearch.h>
#include <Data/CAppSettings.h>
#include <Data/CAppSettings.h>
#include <tools/loggers.h>
#include <boost/algorithm/string.hpp>

class CSubstituteSearchPrivate
{
public:
     CSubstituteSearchPrivate(CSubstituteSearch * ptrPublic);
     ~CSubstituteSearchPrivate();
	 bool testWord(const std::string & ,const CSingleSubstituteDigitsConfiguration & digitConf);
public:
     CSubstituteSearch *                    m_ptrPublic;
	 boost::shared_ptr<CDictionaryData>		dictionaryWords;
	 std::string			number;
	 WordSearchResult				searchResult;
};

CSubstituteSearchPrivate::CSubstituteSearchPrivate(CSubstituteSearch * ptrPublic):m_ptrPublic(ptrPublic)
{}
CSubstituteSearchPrivate::~CSubstituteSearchPrivate(){}
bool CSubstituteSearchPrivate::testWord(const std::string & wordToTest,const CSingleSubstituteDigitsConfiguration & digitConf)
{
	SuccessWord	result;
	size_t startPos(0);
	unsigned int digitIndex(0);
	std::string word = boost::to_upper_copy(wordToTest);
	result.bFullCoverage=true;
	for(;digitIndex<number.size();digitIndex++)
	{
		unsigned int digit = number[digitIndex]-'0';
		//test 
		OneDigitConsonantsSet::const_iterator iter = digitConf.digitsConsonantsSetMap.find(digit);
		size_t acceptPos = word.find_first_of(iter->second.first,startPos);
		size_t forbPos = word.find_first_of(iter->second.second,startPos);
		//if both result are end - we reached end of word or no chars in 
		if (acceptPos == std::string::npos && forbPos == std::string::npos)
		{
			break;
		}
		//if forbidden char is before accept char - we have rejection candidate
		if (acceptPos>forbPos)
		{
			result.matchingLetters.clear();
			result.bFullCoverage=false;
		}
		//ok, expect subst char found
		if (acceptPos<forbPos)
		{
			result.matchingLetters.push_back(word.at(acceptPos));
			result.coveredDigits.push_back(number[digitIndex]);
			startPos = acceptPos+1;
		}
	}
	result.bFullCoverage=result.coveredDigits==number;
	if (word.find_first_of(digitConf.allConsonants,startPos)==std::string::npos)
	{
		result.word=wordToTest;
		searchResult.push_back(result);
		return true;
	}
	return false;
}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
CSubstituteSearch::CSubstituteSearch():privPart(new CSubstituteSearchPrivate(this))
{
}
CSubstituteSearch::~CSubstituteSearch(void){}
void CSubstituteSearch::setDictionaryWords(boost::shared_ptr<CDictionaryData> dictionaryWords)
{
	privPart->searchResult.clear();
	privPart->dictionaryWords = dictionaryWords;
}
void CSubstituteSearch::startSearchForNumber(const std::string & number)
{
	printLog(eInfoLogLevel,eDebug,QString("Searching substitute for number '%1' started").arg(number.c_str()));
	privPart->number = number;
	const std::vector<CSingleSubstituteDigitsConfiguration> & digitConf = gAppSettings->getDigitsConfiguraions();

	unsigned int wordsCount = privPart->dictionaryWords->getWordsCount();
	Q_EMIT searchProgress(0);
	unsigned int notifyStepCount = wordsCount/100;
	unsigned int notifyStep = 0;
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
	Q_EMIT searchFinished();
	printLog(eInfoLogLevel,eDebug,QString("Searching substitute for number '%1' finished").arg(number.c_str()));
}
const WordSearchResult & CSubstituteSearch::getSearchResult()
{
	return privPart->searchResult;
}
