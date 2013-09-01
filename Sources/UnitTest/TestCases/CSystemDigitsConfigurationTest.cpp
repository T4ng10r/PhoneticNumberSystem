#include <TestCases/CSystemDigitsConfigurationTest.h>
#include <map>
#include <boost/foreach.hpp>

void CSystemDigitsConfigurationTest::init()
{
	systemDigitsConfiguration.reset();
	systemDigitsConfiguration.mSystem[0] = std::make_pair('Z','S');
	systemDigitsConfiguration.mSystem[1] = std::make_pair('T','D');
	systemDigitsConfiguration.mSystem[2] = std::make_pair('N',' ');
	systemDigitsConfiguration.mSystem[3] = std::make_pair('M',' ');
	systemDigitsConfiguration.mSystem[4] = std::make_pair('R',' ');
	systemDigitsConfiguration.mSystem[5] = std::make_pair('L',' ');
	systemDigitsConfiguration.mSystem[6] = std::make_pair('J',' ');
	systemDigitsConfiguration.mSystem[7] = std::make_pair('K','G');
	systemDigitsConfiguration.mSystem[8] = std::make_pair('F','W');
	systemDigitsConfiguration.mSystem[9] = std::make_pair('P','B');

	for(SystemMap::const_iterator iter=systemDigitsConfiguration.mSystem.begin();
		iter!=systemDigitsConfiguration.mSystem.end();iter++)
	{
		systemDigitsConfiguration.allConsonants.push_back(iter->second.first);
		systemDigitsConfiguration.allConsonants.push_back(iter->second.second);
	}
}
void CSystemDigitsConfigurationTest::cleanup()
{
	//m_ptrAppSettings.reset();
}
void CSystemDigitsConfigurationTest::test_CreateConsonantsDigits_00()
{
	systemDigitsConfiguration.createConsonantsDigitsMap();
	AcceptableConsonants acceptableConsonants = systemDigitsConfiguration.digitsConsonantsSetMap[0].first;
	ForbidenConsonants forbidenConsonants = systemDigitsConfiguration.digitsConsonantsSetMap[0].second;
	QCOMPARE(acceptableConsonants.c_str(),"ZS");
	QCOMPARE(forbidenConsonants.c_str(),"TDN M R L J KGFWPB");
}
void CSystemDigitsConfigurationTest::test_CreateConsonantsDigits_01()
{
	systemDigitsConfiguration.createConsonantsDigitsMap();
	AcceptableConsonants acceptableConsonants = systemDigitsConfiguration.digitsConsonantsSetMap[1].first;
	ForbidenConsonants forbidenConsonants = systemDigitsConfiguration.digitsConsonantsSetMap[1].second;
	QCOMPARE(acceptableConsonants.c_str(),"TD");
	QCOMPARE(forbidenConsonants.c_str(),"ZSN M R L J KGFWPB");
}
void CSystemDigitsConfigurationTest::test_CreateConsonantsDigits_02()
{
	systemDigitsConfiguration.createConsonantsDigitsMap();
	AcceptableConsonants acceptableConsonants = systemDigitsConfiguration.digitsConsonantsSetMap[2].first;
	ForbidenConsonants forbidenConsonants = systemDigitsConfiguration.digitsConsonantsSetMap[2].second;
	QCOMPARE(acceptableConsonants.c_str(),"N ");
	QCOMPARE(forbidenConsonants.c_str(),"ZSTDMRLJKGFWPB");
}
void CSystemDigitsConfigurationTest::test_CreateConsonantsDigits_03()
{
	systemDigitsConfiguration.createConsonantsDigitsMap();
	AcceptableConsonants acceptableConsonants = systemDigitsConfiguration.digitsConsonantsSetMap[3].first;
	ForbidenConsonants forbidenConsonants = systemDigitsConfiguration.digitsConsonantsSetMap[3].second;
	QCOMPARE(acceptableConsonants.c_str(),"M ");
	QCOMPARE(forbidenConsonants.c_str(),"ZSTDNRLJKGFWPB");
}
void CSystemDigitsConfigurationTest::test_CreateConsonantsDigits_04()
{
	systemDigitsConfiguration.createConsonantsDigitsMap();
	AcceptableConsonants acceptableConsonants = systemDigitsConfiguration.digitsConsonantsSetMap[4].first;
	ForbidenConsonants forbidenConsonants = systemDigitsConfiguration.digitsConsonantsSetMap[4].second;
	QCOMPARE(acceptableConsonants.c_str(),"R ");
	QCOMPARE(forbidenConsonants.c_str(),"ZSTDNMLJKGFWPB");
}
void CSystemDigitsConfigurationTest::test_CreateConsonantsDigits_05()
{
	systemDigitsConfiguration.createConsonantsDigitsMap();
	AcceptableConsonants acceptableConsonants = systemDigitsConfiguration.digitsConsonantsSetMap[5].first;
	ForbidenConsonants forbidenConsonants = systemDigitsConfiguration.digitsConsonantsSetMap[5].second;
	QCOMPARE(acceptableConsonants.c_str(),"L ");
	QCOMPARE(forbidenConsonants.c_str(),"ZSTDNMRJKGFWPB");
}
void CSystemDigitsConfigurationTest::test_CreateConsonantsDigits_06()
{
	systemDigitsConfiguration.createConsonantsDigitsMap();
	AcceptableConsonants acceptableConsonants = systemDigitsConfiguration.digitsConsonantsSetMap[6].first;
	ForbidenConsonants forbidenConsonants = systemDigitsConfiguration.digitsConsonantsSetMap[6].second;
	QCOMPARE(acceptableConsonants.c_str(),"J ");
	QCOMPARE(forbidenConsonants.c_str(),"ZSTDNMRLKGFWPB");
}
void CSystemDigitsConfigurationTest::test_CreateConsonantsDigits_07()
{
	systemDigitsConfiguration.createConsonantsDigitsMap();
	AcceptableConsonants acceptableConsonants = systemDigitsConfiguration.digitsConsonantsSetMap[7].first;
	ForbidenConsonants forbidenConsonants = systemDigitsConfiguration.digitsConsonantsSetMap[7].second;
	QCOMPARE(acceptableConsonants.c_str(),"KG");
	QCOMPARE(forbidenConsonants.c_str(),"ZSTDN M R L J FWPB");
}
void CSystemDigitsConfigurationTest::test_CreateConsonantsDigits_08()
{
	systemDigitsConfiguration.createConsonantsDigitsMap();
	AcceptableConsonants acceptableConsonants = systemDigitsConfiguration.digitsConsonantsSetMap[8].first;
	ForbidenConsonants forbidenConsonants = systemDigitsConfiguration.digitsConsonantsSetMap[8].second;
	QCOMPARE(acceptableConsonants.c_str(),"FW");
	QCOMPARE(forbidenConsonants.c_str(),"ZSTDN M R L J KGPB");
}
void CSystemDigitsConfigurationTest::test_CreateConsonantsDigits_09()
{
	systemDigitsConfiguration.createConsonantsDigitsMap();
	AcceptableConsonants acceptableConsonants = systemDigitsConfiguration.digitsConsonantsSetMap[9].first;
	ForbidenConsonants forbidenConsonants = systemDigitsConfiguration.digitsConsonantsSetMap[9].second;
	QCOMPARE(acceptableConsonants.c_str(),"PB");
	QCOMPARE(forbidenConsonants.c_str(),"ZSTDN M R L J KGFW");
}

