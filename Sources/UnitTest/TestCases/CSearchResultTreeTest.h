#include <QtTest/QtTest>
#include <boost/shared_ptr.hpp>

class CAppSettings;

class CSearchResultTreeTest : public QObject
{
    Q_OBJECT
public:
	//CAppSettings * m_ptrAppSettings;
private Q_SLOTS:
	void init();
	void cleanup();
	void test_SubstituteValuesConfiguration();
};
