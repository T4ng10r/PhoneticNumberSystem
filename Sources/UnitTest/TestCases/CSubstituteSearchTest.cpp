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
	QVERIFY(substituteSearchPrivate->searchCandidatesResult.find(MatchingPair(0,1))!=substituteSearchPrivate->searchCandidatesResult.end());
	SuccessWord fittingWord = substituteSearchPrivate->searchCandidatesResult[MatchingPair(0,1)].front();
	QCOMPARE(fittingWord.coveragePair, MatchingPair(0,1));
	QCOMPARE(fittingWord.coveragePairs.front(), MatchingPair(0,1));
	QCOMPARE(fittingWord.coveredDigits.c_str(), "BB");
	QCOMPARE(fittingWord.bFullCoverage, true);
	QCOMPARE(fittingWord.word.c_str(), "BABCIA");
}
void CSubstituteSearchTest::test_SubstituteSearch_WholeCorrectWord_02()
{
	substituteSearchPrivate->number = "034";
	QCOMPARE(substituteSearchPrivate->testWord("SMAR",singleSubstituteDigitsConfiguration), true);
	QCOMPARE(substituteSearchPrivate->searchResult.front().bFullCoverage, true);
}
void CSubstituteSearchTest::test_SubstituteSearch_WholeWordCoversPartOfNumber()
{
	substituteSearchPrivate->number = "885421866";
	QCOMPARE(substituteSearchPrivate->testWord("LERNED",singleSubstituteDigitsConfiguration), true);
	QCOMPARE(substituteSearchPrivate->searchResult.front().coveredDigits.c_str(), "5421");
	QCOMPARE(substituteSearchPrivate->searchResult.front().matchingLetters.c_str(), "LRND");
	QCOMPARE(substituteSearchPrivate->searchResult.front().bFullCoverage, false);
	substituteSearchPrivate->searchResult.clear();
	substituteSearchPrivate->number = "123";
	QCOMPARE(substituteSearchPrivate->testWord("ACETON",singleSubstituteDigitsConfiguration), true);
	QCOMPARE(substituteSearchPrivate->searchResult.front().coveredDigits.c_str(), "12");
	QCOMPARE(substituteSearchPrivate->searchResult.front().matchingLetters.c_str(), "TN");
	QCOMPARE(substituteSearchPrivate->searchResult.front().bFullCoverage, false);
}
void CSubstituteSearchTest::test_SubstituteSearch_WholeNumberCoversPartOfWord()
{
	//substituteSearchPrivate->number = "233";
	//QCOMPARE(substituteSearchPrivate->testWord("ANEMIA",singleSubstituteDigitsConfiguration), false);
	substituteSearchPrivate->searchResult.clear();
	substituteSearchPrivate->number = "233";
	QCOMPARE(substituteSearchPrivate->testWord("ANEMOMETRIA",singleSubstituteDigitsConfiguration), false);
}
