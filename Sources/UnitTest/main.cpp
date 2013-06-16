#include "CSubstituteValuesConfigurationDlgTest.h"
#include <Data/CAppSettings.h>

CAppSettings	stAppSettings;

int main(int argc, char *argv[]) 
{ 
	QApplication app(argc, argv);
	
	CSubstituteValuesConfigurationDlgTest cSubstituteValuesConfigurationDlgTest;
	if (QTest::qExec(&cSubstituteValuesConfigurationDlgTest, argc, argv)!=0)
		return -1;  
}
