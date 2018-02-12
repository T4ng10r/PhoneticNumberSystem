#include <QtTest/QtTest>
#include <boost/shared_ptr.hpp>
#include <TestRunner.h>

class ut_combobox_matching_words_result_delegate : public QObject
{
	Q_OBJECT
private Q_SLOTS:
		void init();
		void cleanup();
		void test_test_word_data();
		void test_test_word();
};
DECLARE_TEST(ut_combobox_matching_words_result_delegate)
