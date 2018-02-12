#include <QtTest/QtTest>
#include <data/CSystemDigitsConfiguration.h>
#include <boost/shared_ptr.hpp>
#include <TestRunner.h>

class ut_system_digits_configuration   : public QObject
{
	Q_OBJECT
public:
	CSingleSubstituteDigitsConfiguration	systemDigitsConfiguration;
	private Q_SLOTS:
		void init();
		void cleanup();
		void test_CreateConsonantsDigits_data();
		void test_CreateConsonantsDigits();
    void test_create_empty_system();
};
DECLARE_TEST(ut_system_digits_configuration  )
