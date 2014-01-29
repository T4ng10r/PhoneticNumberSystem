#include <QtTest/QtTest>
#include <TestRunner.h>

class CDataThread;

class CDataThreadTest : public QObject
{
	Q_OBJECT
private Q_SLOTS:
	void initTestCase();
	void test_1();
};

DECLARE_TEST(CDataThreadTest)