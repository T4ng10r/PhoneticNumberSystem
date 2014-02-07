#include <QtTest/QtTest>
#include <Data/CSystemDigitsConfiguration.h>
#include <boost/shared_ptr.hpp>
#include <TestRunner.h>

class CSystemDigitsConfigurationTest : public QObject
{
	Q_OBJECT
public:
	CSingleSubstituteDigitsConfiguration	systemDigitsConfiguration;
	private Q_SLOTS:
		void init();
		void cleanup();
		void test_CreateConsonantsDigits_data();
		void test_CreateConsonantsDigits();
};
DECLARE_TEST(CSystemDigitsConfigurationTest)
