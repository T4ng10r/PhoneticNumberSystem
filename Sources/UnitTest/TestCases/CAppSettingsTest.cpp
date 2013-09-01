#include <TestCases/CAppSettingsTest.h>
#include <Data/CAppSettings.h>
#include <set>
#include <boost/foreach.hpp>

void CAppSettingsTest::init()
{
	m_ptrAppSettings = gAppSettings.get();
}
void CAppSettingsTest::cleanup()
{
	//m_ptrAppSettings.reset();
}
void CAppSettingsTest::test_SubstituteValuesConfiguration()
{
	char tabConsonants[] = {'B', 'C', 'D', 'F', 'G', 'H', 'J', 'K', 'L', 'M', 'N', 'P', 'Q', 'R', 
		'S', 'T', 'V', 'X', 'Z', 'W'};
	std::set<char> sConsonants(tabConsonants, tabConsonants+sizeof(tabConsonants)/sizeof(char));
	using boost::property_tree::ptree;
	CSubstituteValuesConfiguration	stConfSubst;

	boost::property_tree::ptree stConf = stConfSubst;
	QCOMPARE(stConf.size(),ptree::size_type(22));
	QCOMPARE(stConf.get_child("count").get_value<int>(),20);
	QCOMPARE(sConsonants.size(),std::size_t(20));
	BOOST_FOREACH(ptree::value_type &val, stConf)
	{
		if (val.first!="consonant")
			continue;
		char cChar = val.second.data().at(0);
		QCOMPARE(sConsonants.erase(cChar),std::size_t(1));
	}
	QCOMPARE(sConsonants.size(),std::size_t(0));
}
