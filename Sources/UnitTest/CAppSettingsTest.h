#include <QtTest/QtTest>
#include <boost/shared_ptr.hpp>
#include <wallaroo/catalog.h>

class CAppSettings;

class CAppSettingsTest : public QObject
{
    Q_OBJECT
public:
private slots:
	void init();
	void cleanup();
	void test_1();
};
