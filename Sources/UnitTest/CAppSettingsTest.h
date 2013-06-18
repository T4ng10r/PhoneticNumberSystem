#include <QtTest/QtTest>
#include <boost/shared_ptr.hpp>

class CAppSettings;

class CAppSettingsTest : public QObject
{
    Q_OBJECT
public:
	boost::shared_ptr<CAppSettings>	m_ptrAppSettings;
private slots:
	void init();
	void cleanup();
	void test_SubstituteValuesConfiguration();
};
