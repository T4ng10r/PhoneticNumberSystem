#include "matching_words_search.h"
#include <data/MatchingWordsSearch.h>
#include <data/MatchingWordsSearch.cpp>
#include <vector>
#include <boost/assign/list_of.hpp>

typedef std::pair<StartingIndex, StartingIndex> mindices;
typedef std::vector<mindices> match_indices;
Q_DECLARE_METATYPE(std::string);
Q_DECLARE_METATYPE(match_indices);

void ut_matching_words_search::initTestCase()
{
    std::string consonants("ZSTDN M R L J KGFWPB");
    for (int i = 0; i < 10; i++) {
        std::string temp(consonants);
        temp.erase(i * 2, 2);
        ConsonantsForDigit cons_set                                   = {consonants.substr(i * 2, 2), temp};
        singleSubstituteDigitsConfiguration.digitsConsonantsSetMap[i] = cons_set;
    }
    singleSubstituteDigitsConfiguration.allConsonants="ZSTDNMRLJKGFWPB";
}

void ut_matching_words_search::init()
{
    substituteSearchPrivate.reset(new MatchingWordsSearchPrivate(NULL));
    substituteSearchPrivate->digits_conf = singleSubstituteDigitsConfiguration;
}
void ut_matching_words_search::cleanup() {}

void ut_matching_words_search::test_test_word_data()
{
    QTest::addColumn<std::string>("number_to_search");
    QTest::addColumn<std::string>("test_word");
    QTest::addColumn<bool>("testing_result");
    QTest::addColumn<match_indices>("indices");
    QTest::addColumn<std::string>("matching_letters");
    QTest::addColumn<bool>("full_coverage");
    QTest::addColumn<size_t>("size");


    using namespace boost::assign;
    match_indices indices;
    //Failing cases
    indices.clear();
    QTest::newRow("fail_01") << std::string("123") << std::string("NERKA") << false << indices 
                              << std::string("") << false << (StartingIndex)0;

    indices.clear();
    QTest::newRow("fail_02") << std::string("6922298548")  << std::string("jaworowianin") << false << indices 
                                << std::string("JW") << false << (StartingIndex)0;

    indices.clear();
    indices.push_back(std::make_pair(0,1));
    QTest::newRow("whole_01") << std::string("99") << std::string("BABCIA") << true << indices 
                              << std::string("BB") << true << (StartingIndex)1;

    indices.clear();
    indices.push_back(std::pair<StartingIndex, StartingIndex>(0,2));
    QTest::newRow("whole_02") << std::string("034") << std::string("SMAR") << true << indices 
                              << std::string("SMR") << true << (StartingIndex)1;

    indices.clear();
    indices.push_back(std::make_pair(0,0));
    QTest::newRow("partial_01") << std::string("9") << std::string("AAP") << true << indices
                                << std::string("P") << true << (StartingIndex)0;

    indices.clear();
    indices.push_back(std::make_pair(2,5));
    QTest::newRow("partial_02") << std::string("885421866") << std::string("LERNED") << true << indices 
                                << std::string("LRND") << false << (StartingIndex)5;
    indices.clear();
    indices.push_back(std::make_pair(0,1));
    QTest::newRow("partial_03") << std::string("123") << std::string("ACETON") << true << indices
                                << std::string("TN") << false << (StartingIndex)0;
    indices.clear();
    indices.push_back(std::make_pair(0,0));
    indices.push_back(std::make_pair(1,1));
    QTest::newRow("partial_two_times") << std::string("99") << std::string("AAP") << true << indices
                                << std::string("P") << false << (StartingIndex)0;

}

void ut_matching_words_search::test_test_word()
{
    //Given
    QFETCH(std::string,number_to_search);
    QFETCH(std::string,test_word);
    QFETCH(bool,testing_result);
    QFETCH(match_indices,indices);
    QFETCH(std::string,matching_letters);
    QFETCH(bool,full_coverage);
    QFETCH(size_t,size);
    //When
    boost::optional<MatchingWord> result = substituteSearchPrivate->testWord(test_word, number_to_search);
    //Then
    QCOMPARE((bool)result, testing_result);
    if (!testing_result)
      return;
    QCOMPARE(result->matchingLetters.c_str(), matching_letters.c_str());
    QCOMPARE(result->bFullCoverage, full_coverage);
    std::list<MatchingPair>::const_iterator iter = result->coveredDigitsIndices.begin();
    for(int i=0;i<indices.size();i++)
    {
      QCOMPARE(iter->start_index, indices[i].first);
      QCOMPARE(iter->end_index, indices[i].second);
      iter++;
    }
}

void ut_matching_words_search::test_add_matching_word_1()
{
  //Given
  StartingIndex startIdx(0);
  MatchingWord word;
  word.coveredDigitsIndices.push_back(MatchingPair(startIdx,startIdx));
  word.words.push_back("test_word_1");
  QCOMPARE(substituteSearchPrivate->searchResult.size(),(size_t)0);
  //When
  substituteSearchPrivate->addMatchingWord(word);
  //Then
  QCOMPARE(substituteSearchPrivate->searchResult.size(),(size_t)1);
  QCOMPARE(substituteSearchPrivate->searchResult.count(startIdx),(size_t)1);
}

void ut_matching_words_search::test_add_matching_word_2()
{
  //Given
  StartingIndex startIdx1(0);
  StartingIndex startIdx2(3);
  MatchingWord word;
  word.coveredDigitsIndices.push_back(MatchingPair(startIdx1,startIdx2));
  word.coveredDigitsIndices.push_back(MatchingPair(startIdx2,startIdx2));
  word.words.push_back("test_word_1");
  QCOMPARE(substituteSearchPrivate->searchResult.size(),(size_t)0);
  //When
  substituteSearchPrivate->addMatchingWord(word);
  //Then
  QCOMPARE(substituteSearchPrivate->searchResult.size(),(size_t)2);
  QCOMPARE(substituteSearchPrivate->searchResult.count(startIdx1),(size_t)1);
  QCOMPARE(substituteSearchPrivate->searchResult.count(startIdx2),(size_t)1);
}

