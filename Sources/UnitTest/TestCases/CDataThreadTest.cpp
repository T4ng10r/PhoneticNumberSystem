#include <Data/CDataThread.h>
#include <TestCases/CDataThreadTest.h>
#include <Data/CAppSettings.h>
#include <Data/CAppSettingsKeywords.h>

void CDataThreadTest::initTestCase()
{
	CDataThread::getInstance();
	gAppSettings->put(SELECTED_DICTIONARY,"");
	gAppSettings->put(DICTIONARIES_DIRECTORY,"");
}

void CDataThreadTest::test_1()
{
}
