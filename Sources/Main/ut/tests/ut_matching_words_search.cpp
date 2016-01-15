#include "ut_matching_words_search.h"
#include <Data/MatchingWordsSearch.h>
#include <Data/MatchingWordsSearch.cpp>
#include <set>
#include <boost/foreach.hpp>
Q_DECLARE_METATYPE(std::string);

void ut_matching_words_search::initTestCase()
{
  std::string consonants("ZSTDN M R L J KGFWPB");
  for(int i=0;i<10;i++)
  {
    std::string temp(consonants);
    temp.erase(i*2,2);
    ConsonantsForDigit cons_set = {consonants.substr(i*2,2),temp};
  	singleSubstituteDigitsConfiguration.digitsConsonantsSetMap[i] = cons_set;
  }
}

void ut_matching_words_search::init()
{
	substituteSearchPrivate.reset(new MatchingWordsSearchPrivate(NULL));
	substituteSearchPrivate->digits_conf = singleSubstituteDigitsConfiguration;
}
void ut_matching_words_search::cleanup(){}

void ut_matching_words_search::test_SubstituteSearch_WholeCorrectWord_data()
{
	QTest::addColumn<std::string>("number_to_search");
	QTest::addColumn<std::string>("test_word");
	QTest::addColumn<size_t>("start_index");
	QTest::addColumn<size_t>("end_index");
	QTest::addColumn<size_t>("size");
	QTest::addColumn<std::string>("matching_letters");
	QTest::addColumn<bool>("full_coverage");

	QTest::newRow("whole_01") << std::string("99") << std::string("BABCIA") << size_t(0) << size_t(1) << size_t(1) << std::string("BB") << true;
	QTest::newRow("whole_02") << std::string("034") << std::string("SMAR") << size_t(0) << size_t(2) << size_t(1) << std::string("SMR") << true;
	//QTest::newRow("whole_03") << std::string("99") << std::string("AAP") << size_t(0) << size_t(1) << size_t(1) << std::string("BB") << true;
	QTest::newRow("partial_01") << std::string("885421866") << std::string("LERNED") << size_t(2) << size_t(5) << size_t(1) << std::string("LRND") << false;
	QTest::newRow("partial_02") << std::string("123") << std::string("ACETON") << size_t(0) << size_t(1) << size_t(1) << std::string("TN") << false;
	QTest::newRow("partial_03") << std::string("99") << std::string("AAP") << size_t(1) << size_t(2) << size_t(1) << std::string("P") << false;
}

void ut_matching_words_search::test_SubstituteSearch_WholeCorrectWord()
{
	QFETCH(std::string, number_to_search);
	QFETCH(std::string, test_word);
	QFETCH(size_t, start_index);
	QFETCH(size_t, end_index);
	QFETCH(size_t, size);
	QFETCH(std::string, matching_letters);
	QFETCH(bool, full_coverage);

	//substituteSearchPrivate->number = number_to_search;
  boost::optional<MatchingWord> result = substituteSearchPrivate->testWord(test_word, number_to_search);
	QVERIFY((bool)result);
	//FittingWordsMap::const_iterator iter = substituteSearchPrivate->searchResultMap.find(MatchingPair(start_index,end_index));
	//QVERIFY(iter!=substituteSearchPrivate->searchResultMap.end());
	//QCOMPARE(iter->second.size(), (std::size_t)size);
	QCOMPARE(result->matchingLetters, matching_letters);
	QCOMPARE(result->bFullCoverage, full_coverage);
	QCOMPARE(result->coveragePairs.front().first, (StartingIndex)start_index);
	QCOMPARE(result->coveragePairs.front().second, (StartingIndex)end_index);
}
