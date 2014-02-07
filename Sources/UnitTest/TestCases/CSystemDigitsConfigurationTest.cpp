#include <TestCases/CSystemDigitsConfigurationTest.h>
#include <map>
#include <boost/foreach.hpp>
Q_DECLARE_METATYPE(std::string);

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
void CSystemDigitsConfigurationTest::test_CreateConsonantsDigits_data()
{
	QTest::addColumn<int>("consonant_index");
	QTest::addColumn<std::string>("expected_acceptableConsonants");
	QTest::addColumn<std::string>("expected_forbidenConsonants");

	QTest::newRow("digit_01") << 0 << std::string("ZS") << std::string("TDN M R L J KGFWPB");
	QTest::newRow("digit_02") << 1 << std::string("TD") << std::string("ZSN M R L J KGFWPB");
	QTest::newRow("digit_03") << 2 << std::string("N ") << std::string("ZSTDMRLJKGFWPB");
	QTest::newRow("digit_04") << 3 << std::string("M ") << std::string("ZSTDNRLJKGFWPB");

	QTest::newRow("digit_05") << 4 << std::string("R ") << std::string("ZSTDNMLJKGFWPB");
	QTest::newRow("digit_06") << 5 << std::string("L ") << std::string("ZSTDNMRJKGFWPB");
	QTest::newRow("digit_07") << 6 << std::string("J ") << std::string("ZSTDNMRLKGFWPB");
	QTest::newRow("digit_08") << 7 << std::string("KG") << std::string("ZSTDN M R L J FWPB");
	QTest::newRow("digit_09") << 8 << std::string("FW") << std::string("ZSTDN M R L J KGPB");
	QTest::newRow("digit_10") << 9 << std::string("PB") << std::string("ZSTDN M R L J KGFW");
}

void CSystemDigitsConfigurationTest::test_CreateConsonantsDigits()
{
	QFETCH(int, consonant_index);
	QFETCH(std::string, expected_acceptableConsonants);
	QFETCH(std::string, expected_forbidenConsonants);

	systemDigitsConfiguration.createConsonantsDigitsMap();
	AcceptableConsonants acceptableConsonants = systemDigitsConfiguration.digitsConsonantsSetMap[consonant_index].first;
	ForbidenConsonants forbidenConsonants = systemDigitsConfiguration.digitsConsonantsSetMap[consonant_index].second;
	QCOMPARE(acceptableConsonants.c_str(), expected_acceptableConsonants.c_str());
	QCOMPARE(forbidenConsonants.c_str(), expected_forbidenConsonants.c_str());
}
