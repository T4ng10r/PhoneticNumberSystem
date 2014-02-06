#include <QtTest/QtTest>
#include <boost/shared_ptr.hpp>
#include <boost/property_tree/ptree.hpp>
#include <TestRunner.h>
#include <Data/CDictionaryData.h>
#include <memory>

class CDictionaryDataTest : public QObject
{
    Q_OBJECT
public:
	std::unique_ptr<CDictionaryData> test_obj;

private Q_SLOTS:
	void init();
	void cleanup();
	void test_load_dictionary_wrong_path();
	void test_load_dictionary_lack_of_affFile();
};

DECLARE_TEST(CDictionaryDataTest)
