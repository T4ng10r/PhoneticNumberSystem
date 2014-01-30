#include <Data/CDataThread.h>
#include <TestCases/CDataThreadTest.h>
#include <Data/CAppSettings.h>
#include <Data/CAppSettingsKeywords.h>
#include <string>

const std::string short_dict("pl_PL_short.dic");
const std::string standard_dict("pl_PL.dic");
const std::string dict_directory("dict");

const unsigned int ciSingleTimeout(1000); //in s
const unsigned int ciTimeoutsCount(5); //in s

void CDataThreadTest::initTestCase()
{
	CDataThread::getInstance();
	gAppSettings->put(SELECTED_DICTIONARY,"");
	gAppSettings->put(DICTIONARIES_DIRECTORY,"");
}

void CDataThreadTest::test_1()
{
	int iCount =0;
	gAppSettings->put(SELECTED_DICTIONARY,short_dict);
	gAppSettings->put(DICTIONARIES_DIRECTORY,dict_directory);

	QSignalSpy  dictionary_loaded_spy(CDataThread::getInstance().get(), SIGNAL(dictionaryLoaded(bool)));
	CDataThread::getInstance()->onSetDictionary();
	while (dictionary_loaded_spy.count() == 0)
	{
		QTest::qWait(ciSingleTimeout);
		++iCount;
		if (iCount>ciTimeoutsCount)
		{
			QVERIFY2(false,"Timeout waiting for dictionaryLoaded signal");
			break;
		}
	}
	QSignalSpy  search_finished_spy(CDataThread::getInstance().get(), SIGNAL(searchFinished()));
	CDataThread::getInstance()->onNumberSearchStarted("571");
	while (search_finished_spy.count() == 0)
	{
		QTest::qWait(ciSingleTimeout);
		++iCount;
		if (iCount>ciTimeoutsCount)
		{
			QVERIFY2(false,"Timeout waiting for searchFinished signal");
			break;
		}
	}

}
