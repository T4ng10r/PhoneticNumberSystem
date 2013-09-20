#include <TestCases/CSubstituteSearchTest.h>
#include <Data/CSubstituteSearch.h>
#include <Data/CSubstituteSearch.cpp>
#include <set>
#include <boost/foreach.hpp>

void CSubstituteSearchTest::createSystemDigitsConfiguration(){}
void CSubstituteSearchTest::initTestCase()
{
	singleSubstituteDigitsConfiguration.digitsConsonantsSetMap.clear();
	singleSubstituteDigitsConfiguration.digitsConsonantsSetMap[0]=std::make_pair("ZS","TDN M R L J KGFWPB");
	singleSubstituteDigitsConfiguration.digitsConsonantsSetMap[1]=std::make_pair("TD","ZSN M R L J KGFWPB");
	singleSubstituteDigitsConfiguration.digitsConsonantsSetMap[2]=std::make_pair("N ","ZSTDMRLJKGFWPB");
	singleSubstituteDigitsConfiguration.digitsConsonantsSetMap[3]=std::make_pair("M ","ZSTDNRLJKGFWPB");
	singleSubstituteDigitsConfiguration.digitsConsonantsSetMap[4]=std::make_pair("R ","ZSTDNMLJKGFWPB");
	singleSubstituteDigitsConfiguration.digitsConsonantsSetMap[5]=std::make_pair("L ","ZSTDNMRJKGFWPB");
	singleSubstituteDigitsConfiguration.digitsConsonantsSetMap[6]=std::make_pair("J ","ZSTDNMRLKGFWPB");
	singleSubstituteDigitsConfiguration.digitsConsonantsSetMap[7]=std::make_pair("KG","ZSTDN M R L J FWPB");
	singleSubstituteDigitsConfiguration.digitsConsonantsSetMap[8]=std::make_pair("FW","ZSTDN M R L J KGPB");
	singleSubstituteDigitsConfiguration.digitsConsonantsSetMap[9]=std::make_pair("PB","ZSTDN M R L J KGFW");
	singleSubstituteDigitsConfiguration.allConsonants="ZSTDN M R L J KGFWPB";
}

void CSubstituteSearchTest::init()
{
	substituteSearchPrivate.reset(new CSubstituteSearchPrivate(NULL));
}
void CSubstituteSearchTest::cleanup()
{
}
void CSubstituteSearchTest::test_SubstituteSearch_WholeCorrectWord_01()
{
	substituteSearchPrivate->number = "99";
	QVERIFY(substituteSearchPrivate->testWord("BABCIA",singleSubstituteDigitsConfiguration));
	FittingWordsMap::const_iterator iter = substituteSearchPrivate->searchResultMap.find(MatchingPair(0,1));
	QVERIFY(iter!=substituteSearchPrivate->searchResultMap.end());
	QCOMPARE(iter->second.size(), (std::size_t)1);
	SuccessWord result = iter->second.front();
	QCOMPARE(result.matchingLetters.c_str(), "BB");
	QCOMPARE(result.bFullCoverage, true);
	QCOMPARE(result.coveragePairs.front().first, (unsigned int)0);
	QCOMPARE(result.coveragePairs.front().second, (unsigned int)1);
}
void CSubstituteSearchTest::test_SubstituteSearch_WholeCorrectWord_02()
{
	substituteSearchPrivate->number = "034";
	QVERIFY(substituteSearchPrivate->testWord("SMAR",singleSubstituteDigitsConfiguration));
	FittingWordsMap::const_iterator iter = substituteSearchPrivate->searchResultMap.find(MatchingPair(0,2));
	QVERIFY(iter!=substituteSearchPrivate->searchResultMap.end());
	QCOMPARE(iter->second.size(), (std::size_t)1);
	SuccessWord result = iter->second.front();
	QCOMPARE(result.bFullCoverage, true);
	QCOMPARE(result.matchingLetters.c_str(), "SMR");
	QCOMPARE(result.coveragePairs.front().first, (unsigned int)0);
	QCOMPARE(result.coveragePairs.front().second, (unsigned int)2);
}
void CSubstituteSearchTest::test_SubstituteSearch_WholeWordCoversPartOfNumber_01()
{
	substituteSearchPrivate->number = "885421866";
	QVERIFY(substituteSearchPrivate->testWord("LERNED",singleSubstituteDigitsConfiguration));
	FittingWordsMap::const_iterator iter = substituteSearchPrivate->searchResultMap.find(MatchingPair(2,5));
	QVERIFY(iter!=substituteSearchPrivate->searchResultMap.end());
	QCOMPARE(iter->second.size(), (std::size_t)1);
	SuccessWord result = iter->second.front();
	QCOMPARE(result.matchingLetters.c_str(), "LRND");
	QCOMPARE(result.bFullCoverage, false);
	QCOMPARE(result.coveragePairs.front().first, (unsigned int)2);
	QCOMPARE(result.coveragePairs.front().second, (unsigned int)5);
}
void CSubstituteSearchTest::test_SubstituteSearch_WholeWordCoversPartOfNumber_02()
{
	substituteSearchPrivate->number = "123";
	QVERIFY(substituteSearchPrivate->testWord("ACETON",singleSubstituteDigitsConfiguration));
	FittingWordsMap::const_iterator iter = substituteSearchPrivate->searchResultMap.find(MatchingPair(0,1));
	QVERIFY(iter!=substituteSearchPrivate->searchResultMap.end());
	QCOMPARE(iter->second.size(), (std::size_t)1);
	SuccessWord result = iter->second.front();
	QCOMPARE(result.matchingLetters.c_str(), "TN");
	QCOMPARE(result.bFullCoverage, false);
	QCOMPARE(result.coveragePairs.front().first, (unsigned int)0);
	QCOMPARE(result.coveragePairs.front().second, (unsigned int)1);
}
void CSubstituteSearchTest::test_SubstituteSearch_WholeNumberCoversPartOfWord()
{
	//substituteSearchPrivate->number = "233";
	//QCOMPARE(substituteSearchPrivate->testWord("ANEMIA",singleSubstituteDigitsConfiguration), false);
	substituteSearchPrivate->searchResult.clear();
	substituteSearchPrivate->number = "233";
	QCOMPARE(substituteSearchPrivate->testWord("ANEMOMETRIA",singleSubstituteDigitsConfiguration), false);
}
