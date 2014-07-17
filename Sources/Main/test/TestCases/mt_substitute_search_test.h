#include <QtTest/QtTest>
#include <boost/shared_ptr.hpp>
#include <Data/CSystemDigitsConfiguration.h>
#include <TestRunner.h>

class CSubstituteSearch;
class CDictionaryData;

class mt_substitute_search_test : public QObject
{
	Q_OBJECT
	CSingleSubstituteDigitsConfiguration  single_substitute_digits_configuration;
	boost::shared_ptr<CSubstituteSearch>	substitute_search;
	boost::shared_ptr<CDictionaryData>	dictionary_data;
private:
	void create_system_digits_configuration();
	void create_dictionary_data();
private Q_SLOTS:
	void initTestCase();
	void init();
	void test_SubstituteSearch_WholeCorrectWord_01();
};
DECLARE_TEST(mt_substitute_search_test)
