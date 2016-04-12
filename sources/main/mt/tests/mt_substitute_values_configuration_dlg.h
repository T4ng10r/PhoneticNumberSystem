#include <QtTest/QtTest>
#include <boost/shared_ptr.hpp>
#include <boost/property_tree/ptree.hpp>
#include <data/CSystemDigitsConfiguration.h>
#include <TestRunner.h>

class CSubstituteValuesConfigurationDlgTemp;

class mt_substitute_values_configuration_dlg  : public QObject
{
    Q_OBJECT
public:
	mt_substitute_values_configuration_dlg ();
public:
	boost::shared_ptr<CSubstituteValuesConfigurationDlgTemp>  dialog;
  boost::property_tree::ptree  consonants_values_set;
  boost::property_tree::ptree  digits_configuration_properties;
  CSingleSubstituteDigitsConfiguration	systemDigitsConfiguration;
  std::vector<CSingleSubstituteDigitsConfiguration>  configurations;
private Q_SLOTS:
	void init();
	void cleanup();
	void test_NoneInFirstMenusColumnSelected();
	void test_OtherMenusEntriesDisabled();
	void test_SecondMenusDisabled();
	void test_SelectNoneConfiguration();
protected:
  void createProperiestConfiguration();
  void createDigitsConfiguration();
};
DECLARE_TEST(mt_substitute_values_configuration_dlg )
