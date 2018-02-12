#include <QtTest/QtTest>
#include <TestRunner.h>

class DataThread;

class mt_data_thread : public QObject
{
	Q_OBJECT
private Q_SLOTS:
	void initTestCase();
	void test_serching_short_dict();
};

DECLARE_TEST(mt_data_thread)
