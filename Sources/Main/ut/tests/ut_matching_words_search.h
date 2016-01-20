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
		void test_MatchingWordSearch_data();
		void test_MatchingWordSearch();
};
DECLARE_TEST(ut_matching_words_search)
