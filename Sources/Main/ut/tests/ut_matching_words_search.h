#include <QtTest/QtTest>
#include <boost/shared_ptr.hpp>
#include <Data/CSystemDigitsConfiguration.h>
#include <TestRunner.h>

class MatchingWordsSearchPrivate;

class ut_matching_words_search : public QObject
{
	Q_OBJECT
	CSingleSubstituteDigitsConfiguration  singleSubstituteDigitsConfiguration;
	boost::shared_ptr<MatchingWordsSearchPrivate>	substituteSearchPrivate;
public:
private Q_SLOTS:
		void initTestCase();
		void init();
		void cleanup();
		void test_SubstituteSearch_WholeCorrectWord_data();
		void test_SubstituteSearch_WholeCorrectWord();
    /*
		void test_SubstituteSearch_WholeCorrectWord_02();
		void test_SubstituteSearch_WholeWordCoversPartOfNumber_01();
		void test_SubstituteSearch_WholeWordCoversPartOfNumber_02();
		void test_SubstituteSearch_WholeNumberCoversPartOfWord();*/
};
DECLARE_TEST(ut_matching_words_search)
