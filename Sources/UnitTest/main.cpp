#include "CSubstituteValuesConfigurationDlgTest.h"
#include "CAppSettingsTest.h"
#include <Data/CAppSettings.h>

int main(int argc, char *argv[]) 
{ 
	QApplication app(argc, argv);

	CAppSettingsTest cAppSettingsTest;
	if (QTest::qExec(&cAppSettingsTest, argc, argv)!=0)
		return -1;  
	
	CSubstituteValuesConfigurationDlgTest cSubstituteValuesConfigurationDlgTest;
	if (QTest::qExec(&cSubstituteValuesConfigurationDlgTest, argc, argv)!=0)
		return -1;  
}
