#include <Data/DataThread.h>
#include <TestCases/CDataThreadTest.h>
#include <Data/AppSettings.h>
#include <Data/CAppSettingsKeywords.h>
#include <string>

const std::string short_dict("pl_PL_short");
const std::string standard_dict("pl_PL");
const std::string dict_directory("dict");

const unsigned int single_timeout(1000); //in s
const unsigned int timeouts_count(5); //in s

void CDataThreadTest::initTestCase()
{
	DataThread::getInstance();
	gAppSettings->put(SELECTED_DICTIONARY,"");
	gAppSettings->put(DICTIONARIES_DIRECTORY,"");
}

void CDataThreadTest::test_serching_short_dict()
{
	int iCount =0;
	gAppSettings->put(SELECTED_DICTIONARY,short_dict);
	gAppSettings->put(DICTIONARIES_DIRECTORY,dict_directory);

	QSignalSpy  dictionary_loaded_spy(DataThread::getInstance().get(), SIGNAL(dictionaryLoaded(bool)));
	DataThread::getInstance()->onSetDictionary();
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
	QSignalSpy  search_finished_spy(DataThread::getInstance().get(), SIGNAL(searchFinished(bool)));
	DataThread::getInstance()->onNumberSearchStarted("571");
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

	WordsList result = DataThread::getInstance()->getSearchResult(0);
	QVERIFY(result.size()!=0);
	//signal_resp = search_finished_spy.takeFirst();
}
