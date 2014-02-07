#include "CDictionaryDataTest.h"

const unsigned int ciSingleTimeout(1000); //in s
const unsigned int ciTimeoutsCount(5); //in s

const std::string short_dict("pl_PL_short.dic");
const std::string standard_dict("pl_PL.dic");
const std::string dict_directory("dict");
const std::string non_existing_dict("aaa");

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
	QCOMPARE(test_obj->loadDictionary(non_existing_dict), false);
	QCOMPARE(test_obj->loadDictionary(dict_directory+QDir::separator().toLatin1()+non_existing_dict), false);
};
void CDictionaryDataTest::test_load_dictionary()
{
	QCOMPARE(test_obj->loadDictionary(dict_directory+QDir::separator().toLatin1()+short_dict), true);
	QCOMPARE(test_obj->getWordsCount(), (unsigned int)1245);

	QCOMPARE(test_obj->loadDictionary(dict_directory+QDir::separator().toLatin1()+standard_dict), true);
	QCOMPARE(test_obj->getWordsCount(), (unsigned int)280962);
}
