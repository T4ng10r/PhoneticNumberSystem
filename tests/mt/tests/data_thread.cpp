#include <data/data_thread.h>
#include <tests/data_thread.h>
#include <data/settings.h>
#include <data/CSettingsKeywords.h>
#include <string>

const std::string short_dict("pl_PL_short");
const std::string standard_dict("pl_PL");
const std::string dict_directory("dict");

const unsigned int single_timeout(1000); //in s
const unsigned int timeouts_count(5); //in s

void mt_data_thread::initTestCase()
{
	gDataThread;
	gSettings->put(SELECTED_DICTIONARY,"");
	gSettings->put(DICTIONARIES_DIRECTORY,"");
}

void mt_data_thread::test_serching_short_dict()
{
	int iCount =0;
	gSettings->put(SELECTED_DICTIONARY,short_dict);
	gSettings->put(DICTIONARIES_DIRECTORY,dict_directory);

	QSignalSpy  dictionary_loaded_spy(gDataThread.get(), SIGNAL(dictionaryLoaded(bool)));
	gDataThread->onSetDictionary();
	while (dictionary_loaded_spy.count() == 0)
	{
		QTest::qWait(single_timeout);
		++iCount;
		if (iCount>timeouts_count)
		{
			QVERIFY2(false,"Timeout waiting for dictionaryLoaded signal");
			break;
		}
	}
	QList<QVariant> signal_resp = dictionary_loaded_spy.takeFirst();
	QCOMPARE(signal_resp.at(0).toBool(), true);
	QSignalSpy  search_finished_spy(gDataThread.get(), SIGNAL(searchFinished(bool)));
	gDataThread->onNumberSearchStarted("571");
	while (search_finished_spy.count() == 0)
	{
		QTest::qWait(single_timeout);
		++iCount;
		if (iCount>timeouts_count)
		{
			QVERIFY2(false,"Timeout waiting for searchFinished signal");
			break;
		}
	}
	signal_resp = search_finished_spy.takeFirst();
	QCOMPARE(signal_resp.at(0).toBool(), true);

	MatchingWordsList result = gDataThread->getSearchResult(0);
	QVERIFY(result.size()!=0);
	//signal_resp = search_finished_spy.takeFirst();
}
