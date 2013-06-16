#include <QtTest/QtTest>
#include <boost/shared_ptr.hpp>

class CSubstituteValuesConfigurationDlg;

class CSubstituteValuesConfigurationDlgTest : public QObject
{
    Q_OBJECT
public:
	CSubstituteValuesConfigurationDlgTest();
public:
	boost::shared_ptr<CSubstituteValuesConfigurationDlg>  m_ptrDialog;
private slots:
	void init();
	void cleanup();
	void test_1();
};
