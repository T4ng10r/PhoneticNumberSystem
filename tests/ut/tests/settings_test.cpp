#include "settings_test.h"
#include <data/settings.h>
#include <iterator>	 //for distance
#include <set>
#include <boost/foreach.hpp>

#define CONSONANTS_KEYWORD "consonant"
#define COUNT_KEYWORD "count"
#define DIGITAL_CONF_KEYWORD "digits_configuration"
#define DIGIT_KEYWORD "digit"
#define XMLATTR_KEYWORD "<xmlattr>"


using boost::property_tree::ptree;

ptree getChildTree( ptree & item )
{
	boost::optional<ptree &> child_opt = item.get_child_optional(XMLATTR_KEYWORD);
	if (child_opt)
	{
		return item.get_child(XMLATTR_KEYWORD);
	}
	return item;
}

void CSettingsTest::init()
{
	m_ptrSettings = gSettings.get();
}
void CSettingsTest::cleanup()
{
	//m_ptrSettings.reset();
}
void CSettingsTest::test_SubstituteValuesConfiguration_Consonants()
{
	char tabConsonants[] = {'B', 'C', 'D', 'F', 'G', 'H', 'J', 'K', 'L', 'M', 'N', 'P', 'Q', 'R', 
		'S', 'T', 'V', 'X', 'Z', 'W'};
	std::set<char> sConsonants(tabConsonants, tabConsonants+sizeof(tabConsonants)/sizeof(char));
	CSubstituteValuesConfiguration	stConfSubst;

	QCOMPARE(stConfSubst.count(CONSONANTS_KEYWORD),ptree::size_type(20));
	QCOMPARE(stConfSubst.count(COUNT_KEYWORD),ptree::size_type(1));
	QCOMPARE(stConfSubst.get_child(COUNT_KEYWORD).get_value<int>(),20);
	QCOMPARE(sConsonants.size(),std::size_t(20));
  std::pair<ptree::assoc_iterator, ptree::assoc_iterator> consonants_set = stConfSubst.equal_range(CONSONANTS_KEYWORD);
	QCOMPARE(std::distance(consonants_set.first, consonants_set.second),(std::iterator_traits<ptree::assoc_iterator>::difference_type)20);
	for(ptree::assoc_iterator iter = consonants_set.first; iter != consonants_set.second; ++iter)
	{
		char cChar = iter->second.data().at(0);
		QCOMPARE(sConsonants.erase(cChar),std::size_t(1));
	}
	QCOMPARE(sConsonants.size(),std::size_t(0));
}

void CSettingsTest::testSingleDigitEntry( boost::property_tree::ptree & digit_tree, const digit_entry & to_match)
{
	ptree child = getChildTree(digit_tree);
	ptree::const_iterator iter = child.begin();

	QCOMPARE(iter->first,std::string("value"));
	QCOMPARE(iter->second.get_value<unsigned int>(),to_match.value);
	++iter;
	QCOMPARE(iter->first,std::string("substitue"));
	QCOMPARE(iter->second.get_value<char>(),to_match.substitue_1);
	if (to_match.substitue_2==' ')
		return;
	++iter;
	QCOMPARE(iter->first,std::string("substitue"));
	boost::optional<char> val = iter->second.get_value_optional<char>();
	if (val)
		QCOMPARE(val.get(),to_match.substitue_2);
}
void CSettingsTest::test_SubstituteValuesConfiguration_PtreeConfiguration()
{
	CSubstituteValuesConfiguration	stConfSubst;
	std::pair<ptree::assoc_iterator, ptree::assoc_iterator> digital_conf_set = stConfSubst.equal_range(DIGITAL_CONF_KEYWORD);
	QCOMPARE(std::distance(digital_conf_set.first, digital_conf_set.second),(std::iterator_traits<ptree::assoc_iterator>::difference_type)1);

	ptree digit_conf = stConfSubst.get_child(DIGITAL_CONF_KEYWORD);
	QCOMPARE(digit_conf.count(DIGIT_KEYWORD),ptree::size_type(10));
	std::pair<ptree::assoc_iterator, ptree::assoc_iterator> digits_set = digit_conf.equal_range(DIGIT_KEYWORD);
	QCOMPARE(std::distance(digits_set.first, digits_set.second),(std::iterator_traits<ptree::assoc_iterator>::difference_type)10);

	QCOMPARE(digit_conf.get_child("name").get_value<std::string>(),std::string("basic"));
	ptree::assoc_iterator digits_iter = digits_set.first;
	testSingleDigitEntry(digits_iter->second, digit_entry(0,'Z','S'));
	digits_iter++;
	testSingleDigitEntry(digits_iter->second, digit_entry(1,'T','D'));
	digits_iter++;
	testSingleDigitEntry(digits_iter->second, digit_entry(2,'N',' '));
	digits_iter++;
	testSingleDigitEntry(digits_iter->second, digit_entry(3,'M',' '));
	digits_iter++;
	testSingleDigitEntry(digits_iter->second, digit_entry(4,'R',' '));
	digits_iter++;
	testSingleDigitEntry(digits_iter->second, digit_entry(5,'L',' '));
	digits_iter++;
	testSingleDigitEntry(digits_iter->second, digit_entry(6,'J',' '));
	digits_iter++;
	testSingleDigitEntry(digits_iter->second, digit_entry(7,'K','G'));
	digits_iter++;
	testSingleDigitEntry(digits_iter->second, digit_entry(8,'F','W'));
	digits_iter++;
	testSingleDigitEntry(digits_iter->second, digit_entry(9,'P','B'));

}
void CSettingsTest::test_SubstituteValuesConfiguration_Configuration()
{
	const std::vector<CSingleSubstituteDigitsConfiguration> & vDigitsConf = gSettings->getDigitsConfiguraions();
	QCOMPARE(vDigitsConf.size(), std::size_t(1));
	const CSingleSubstituteDigitsConfiguration & digits_conf = vDigitsConf[0];
	QCOMPARE(digits_conf.strName,std::string("basic"));
	QCOMPARE(digits_conf.allConsonants,std::string("ZSTDNMRLJKGFWPB"));
}
