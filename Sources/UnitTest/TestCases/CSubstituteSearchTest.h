#include <QtTest/QtTest>
#include <boost/shared_ptr.hpp>
#include <Data/CSystemDigitsConfiguration.h>

class CSubstituteSearchPrivate;

class CSubstituteSearchTest : public QObject
{
	Q_OBJECT
	CSingleSubstituteDigitsConfiguration  singleSubstituteDigitsConfiguration;
	boost::shared_ptr<CSubstituteSearchPrivate>	substituteSearchPrivate;
public:
private:
	void createSystemDigitsConfiguration();
private Q_SLOTS:
		void initTestCase();
		void init();
		void cleanup();
		void test_SubstituteSearch_WholeCorrectWord_01();
		void test_SubstituteSearch_WholeCorrectWord_02();
		void test_SubstituteSearch_WholeWordCoversPartOfNumber();
		void test_SubstituteSearch_WholeNumberCoversPartOfWord();
};
