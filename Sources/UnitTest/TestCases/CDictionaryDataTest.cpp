#include "CDictionaryDataTest.h"

const unsigned int ciSingleTimeout(1000); //in s
const unsigned int ciTimeoutsCount(5); //in s

void CDictionaryDataTest::init()
{
	test_obj.reset(new CDictionaryData);
}
void CDictionaryDataTest::cleanup()
{
	test_obj.reset();
}
void CDictionaryDataTest::test_load_dictionary_wrong_path()
{
	QCOMPARE(test_obj->loadDictionary("aa"), false);
	QCOMPARE(test_obj->loadDictionary("dict/aa"), false);
};
void CDictionaryDataTest::test_load_dictionary_lack_of_affFile()
{
	QCOMPARE(test_obj->loadDictionary("dict/pl_PL_short.dic"), false);
}
