#include <QtTest/QtTest>
#include <TestRunner.h>

class DataThread;

class DataThreadTest : public QObject
{
	Q_OBJECT
private Q_SLOTS:
	void initTestCase();
	void test_serching_short_dict();
};

DECLARE_TEST(DataThreadTest)
