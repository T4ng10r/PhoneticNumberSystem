#include "ut_matching_words_search.h"
#include <Data/MatchingWordsSearch.cpp>
#include <vector>

typedef std::vector<std::pair<StartingIndex, StartingIndex> > match_indices;
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

void ut_matching_words_search::test_MatchingWordSearch_data()
{
    QTest::addColumn<std::string>("number_to_search");
    QTest::addColumn<std::string>("test_word");
    QTest::addColumn<bool>("testing_result");
    QTest::addColumn<match_indices>("indices");
    QTest::addColumn<std::string>("matching_letters");
    QTest::addColumn<bool>("full_coverage");
    QTest::addColumn<size_t>("size");


    match_indices indices;
    //Failing cases
    indices.clear();
    QTest::newRow("fail_01") << std::string("123") << std::string("NERKA") << false << indices 
                              << std::string("") << false << (StartingIndex)0;

    /*indices.clear();
    indices.push_back(std::make_pair(0,1));
    QTest::newRow("whole_01") << std::string("99") << std::string("BABCIA") << true << indices 
                              << std::string("BB") << true << (StartingIndex)1;

    indices.clear();
    indices.push_back(std::make_pair(0,2));
    QTest::newRow("whole_02") << std::string("034") << std::string("SMAR") << true << indices 
                              << std::string("SMR") << true << (StartingIndex)1;
    
    indices.clear();
    indices.push_back(std::make_pair(2,5));
    QTest::newRow("partial_01") << std::string("885421866") << std::string("LERNED") << true << indices 
                                << std::string("LRND") << false << (StartingIndex)5;

    indices.clear();
    indices.push_back(std::make_pair(0,1));
    QTest::newRow("partial_02") << std::string("123") << std::string("ACETON") << true << indices
                                << std::string("TN") << false << (StartingIndex)0;
   */
    indices.clear();
    indices.push_back(std::make_pair(0,0));
    QTest::newRow("partial_01") << std::string("9") << std::string("AAP") << true << indices
                                << std::string("P") << true << (StartingIndex)0;
    indices.clear();
    indices.push_back(std::make_pair(0,0));
    indices.push_back(std::make_pair(1,1));
    QTest::newRow("partial_03") << std::string("99") << std::string("AAP") << true << indices
                                << std::string("P") << false << (StartingIndex)0;
}

void ut_matching_words_search::test_MatchingWordSearch()
{
    QFETCH(std::string,number_to_search);
    QFETCH(std::string,test_word);
    QFETCH(bool,testing_result);
    QFETCH(match_indices,indices);
    QFETCH(std::string,matching_letters);
    QFETCH(bool,full_coverage);
    QFETCH(size_t,size);

    boost::optional<MatchingWord> result = substituteSearchPrivate->testWord(test_word, number_to_search);
    QCOMPARE((bool)result, testing_result);
    if (!testing_result)
      return;
    // FittingWordsMap::const_iterator iter =
    // substituteSearchPrivate->searchResultMap.find(MatchingPair(start_index,end_index));
    // QVERIFY(iter!=substituteSearchPrivate->searchResultMap.end());
    // QCOMPARE(iter->second.size(), (std::size_t)size);
    QCOMPARE(result->matchingLetters, matching_letters);
    QCOMPARE(result->bFullCoverage, full_coverage);
    std::list<MatchingPair>::const_iterator iter = result->coveredDigitsIndices.begin();
    for(int i=0;i<indices.size();i++)
    {
      QCOMPARE(iter->startIndex, indices[i].first);
      QCOMPARE(iter->endIndex, indices[i].second);
      iter++;
    }
}
