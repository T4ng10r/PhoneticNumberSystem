#include <QtTest/QtTest>
#include <boost/shared_ptr.hpp>
#include <boost/property_tree/ptree.hpp>

class CSubstituteValuesConfigurationDlgTemp;

class CSubstituteValuesConfigurationDlgTest : public QObject
{
    Q_OBJECT
public:
	CSubstituteValuesConfigurationDlgTest();
public:
	boost::shared_ptr<CSubstituteValuesConfigurationDlgTemp>  m_ptrDialog;
	boost::property_tree::ptree  m_stConfiguration;
private Q_SLOTS:
	void init();
	void cleanup();
	void test_OtherMenusEntriesDisabled();
	void test_SecondMenusDisabled();
};
