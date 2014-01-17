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
		void test_CreateConsonantsDigits_00();
		void test_CreateConsonantsDigits_01();
		void test_CreateConsonantsDigits_02();
		void test_CreateConsonantsDigits_03();
		void test_CreateConsonantsDigits_04();
		void test_CreateConsonantsDigits_05();
		void test_CreateConsonantsDigits_06();
		void test_CreateConsonantsDigits_07();
		void test_CreateConsonantsDigits_08();
		void test_CreateConsonantsDigits_09();
};
DECLARE_TEST(CSystemDigitsConfigurationTest)
