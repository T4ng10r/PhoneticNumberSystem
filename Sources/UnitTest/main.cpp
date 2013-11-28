#include "TestCases/CSubstituteValuesConfigurationDlgTest.h"
#include "TestCases/CAppSettingsTest.h"
#include "TestCases/CSystemDigitsConfigurationTest.h"
#include "TestCases/CSearchResultTreeTest.h"
#include "TestCases/CSubstituteSearchTest.h"
#include <Data/CAppSettings.h>

int main(int argc, char *argv[]) 
{ 
	QApplication app(argc, argv);

	CAppSettingsTest cAppSettingsTest;
	if (QTest::qExec(&cAppSettingsTest, argc, argv)!=0)
		return -1;  
	//
	CSubstituteValuesConfigurationDlgTest cSubstituteValuesConfigurationDlgTest;
	if (QTest::qExec(&cSubstituteValuesConfigurationDlgTest, argc, argv)!=0)
		return -1;  

	CSearchResultTreeNodeTest searchResultTreeTest;
	if (QTest::qExec(&searchResultTreeTest, argc, argv)!=0)
		return -1;  

	CSubstituteSearchTest cSubstituteSearchTest;
	if (QTest::qExec(&cSubstituteSearchTest, argc, argv)!=0)
		return -1;  

}
