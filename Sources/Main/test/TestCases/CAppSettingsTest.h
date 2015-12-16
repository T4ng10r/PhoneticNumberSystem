#include <QtTest/QtTest>
#include <boost/shared_ptr.hpp>
#include <boost/property_tree/ptree.hpp>
#include <TestRunner.h>

struct digit_entry
{
	digit_entry(unsigned int val, char subst_1, char subst_2):
		value(val),substitue_1(subst_1), substitue_2(subst_2){}

	unsigned int value;
	char substitue_1;
	char substitue_2;
};

class AppSettings;

class CAppSettingsTest : public QObject
{
    Q_OBJECT
public:
	AppSettings * m_ptrAppSettings;
	void testSingleDigitEntry(boost::property_tree::ptree & digit_tree, const digit_entry & to_match);

	private Q_SLOTS:
	void init();
	void cleanup();
	void test_SubstituteValuesConfiguration_Consonants();
	void test_SubstituteValuesConfiguration_PtreeConfiguration();
	void test_SubstituteValuesConfiguration_Configuration();
};

DECLARE_TEST(CAppSettingsTest)
