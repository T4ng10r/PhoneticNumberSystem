#include <QtTest/QtTest>
#include <boost/shared_ptr.hpp>
#include <boost/property_tree/ptree.hpp>
#include <TestRunner.h>
#include <Data/DictionaryData.h>
#include <memory>

class ut_dictionary_data : public QObject
{
    Q_OBJECT
public:
	std::unique_ptr<DictionaryData> test_obj;

private Q_SLOTS:
	void init();
	void cleanup();
	void test_load_dictionary_wrong_path();
	void test_load_dictionary();
};

DECLARE_TEST(ut_dictionary_data)
