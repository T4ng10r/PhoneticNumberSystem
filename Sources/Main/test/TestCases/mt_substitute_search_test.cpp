#include <TestCases/mt_substitute_search_test.h>
#include <Data/MatchingWordsSearch.h>
#include <Data/CSettingsKeywords.h>
//#include <Data/MatchingWordsSearch.cpp>
#include <set>
#include <boost/foreach.hpp>
Q_DECLARE_METATYPE(std::string);

const std::string short_dict("pl_PL_short");
const std::string standard_dict("pl_PL");
const std::string dict_directory("dict");

const unsigned int single_timeout(1000); //in s
const unsigned int timeouts_count(5); //in s

void mt_substitute_search_test::create_system_digits_configuration()
{
	single_substitute_digits_configuration.digitsConsonantsSetMap.clear();
	single_substitute_digits_configuration.digitsConsonantsSetMap[0]=std::make_pair("ZS","TDN M R L J KGFWPB");
	single_substitute_digits_configuration.digitsConsonantsSetMap[1]=std::make_pair("TD","ZSN M R L J KGFWPB");
	single_substitute_digits_configuration.digitsConsonantsSetMap[2]=std::make_pair("N ","ZSTDMRLJKGFWPB");
	single_substitute_digits_configuration.digitsConsonantsSetMap[3]=std::make_pair("M ","ZSTDNRLJKGFWPB");
	single_substitute_digits_configuration.digitsConsonantsSetMap[4]=std::make_pair("R ","ZSTDNMLJKGFWPB");
	single_substitute_digits_configuration.digitsConsonantsSetMap[5]=std::make_pair("L ","ZSTDNMRJKGFWPB");
	single_substitute_digits_configuration.digitsConsonantsSetMap[6]=std::make_pair("J ","ZSTDNMRLKGFWPB");
	single_substitute_digits_configuration.digitsConsonantsSetMap[7]=std::make_pair("KG","ZSTDN M R L J FWPB");
	single_substitute_digits_configuration.digitsConsonantsSetMap[8]=std::make_pair("FW","ZSTDN M R L J KGPB");
	single_substitute_digits_configuration.digitsConsonantsSetMap[9]=std::make_pair("PB","ZSTDN M R L J KGFW");
	single_substitute_digits_configuration.allConsonants="ZSTDN M R L J KGFWPB";
}
void mt_substitute_search_test::create_dictionary_data()
{
	std::string dictionaryPath = dict_directory + QDir::separator().toLatin1() + short_dict+DICTIONARY_FILE_EXTENSION;
	dictionary_data->loadDictionary(dictionaryPath);
}

void mt_substitute_search_test::initTestCase()
{
	create_system_digits_configuration();
	dictionary_data.reset(new DictionaryData());
	create_dictionary_data();
}

void mt_substitute_search_test::init()
{
	substitute_search.reset(new MatchingWordsSearch());
	substitute_search->setSubstituteDigitsConfiguration(single_substitute_digits_configuration);
	substitute_search->setDictionaryWords(dictionary_data);
}

void mt_substitute_search_test::test_SubstituteSearch_WholeCorrectWord_01()
{
	int count =0;
	QSignalSpy  search_finished_spy(substitute_search.get(), SIGNAL(searchFinished(bool)));
	substitute_search->startSearchForNumber("99");
	while (search_finished_spy.count() == 0)
	{
		QTest::qWait(single_timeout);
		++count;
		if (count>timeouts_count)
		{
			QVERIFY2(false,"Timeout waiting for dictionaryLoaded signal");
			break;
		}
	}
	QList<QVariant> signal_resp = search_finished_spy.takeFirst();
	QCOMPARE(signal_resp.at(0).toBool(), true);

	MatchingWordsList search_result = substitute_search->getSearchResult(0);
	MatchingWordsList::const_iterator iter = search_result.begin();
	QVERIFY(search_result.size()>0);
	QCOMPARE(iter->getWord(),std::string("AAP"));
	QCOMPARE(iter->bFullCoverage,false);
	QCOMPARE(iter->matchingLetters,std::string("P"));
	QCOMPARE(*(iter->coveragePairs.begin()),MatchingPair(0,0));
	iter++;
	QCOMPARE(iter->getWord(),std::string("ab"));
	QCOMPARE(iter->bFullCoverage,false);
	QCOMPARE(iter->matchingLetters,std::string("B"));
	QCOMPARE(*(iter->coveragePairs.begin()),MatchingPair(0,0));
	iter++;
	QCOMPARE(iter->getWord(),std::string("Ab"));
	QCOMPARE(iter->bFullCoverage,false);
	QCOMPARE(iter->matchingLetters,std::string("B"));
	QCOMPARE(*(iter->coveragePairs.begin()),MatchingPair(0,0));
	iter++;
	QCOMPARE(iter->getWord(),std::string("abc"));
	QCOMPARE(iter->bFullCoverage,false);
	QCOMPARE(iter->matchingLetters,std::string("B"));
	QCOMPARE(*(iter->coveragePairs.begin()),MatchingPair(0,0));
	iter++;
	QCOMPARE(iter->getWord(),std::string("ABC"));
	QCOMPARE(iter->bFullCoverage,false);
	QCOMPARE(iter->matchingLetters,std::string("B"));
	QCOMPARE(*(iter->coveragePairs.begin()),MatchingPair(0,0));
	iter++;
	QCOMPARE(iter->getWord(),std::string("Abe"));
	QCOMPARE(iter->bFullCoverage,false);
	QCOMPARE(iter->matchingLetters,std::string("B"));
	QCOMPARE(*(iter->coveragePairs.begin()),MatchingPair(0,0));
	iter++;
	QCOMPARE(iter->getWord(),std::string("Abihu"));
	QCOMPARE(iter->bFullCoverage,false);
	QCOMPARE(iter->matchingLetters,std::string("B"));
	QCOMPARE(*(iter->coveragePairs.begin()),MatchingPair(0,0));
	iter++;
	QCOMPARE(iter->getWord(),std::string("abo"));
	QCOMPARE(iter->bFullCoverage,false);
	QCOMPARE(iter->matchingLetters,std::string("B"));
	QCOMPARE(*(iter->coveragePairs.begin()),MatchingPair(0,0));
	iter++;
	QCOMPARE(iter->getWord(),std::string("Abu"));
	QCOMPARE(iter->bFullCoverage,false);
	QCOMPARE(iter->matchingLetters,std::string("B"));
	QCOMPARE(*(iter->coveragePairs.begin()),MatchingPair(0,0));
	iter++;
	QCOMPARE(iter->getWord(),std::string("aby"));
	QCOMPARE(iter->bFullCoverage,false);
	QCOMPARE(iter->matchingLetters,std::string("B"));
	QCOMPARE(*(iter->coveragePairs.begin()),MatchingPair(0,0));
}
