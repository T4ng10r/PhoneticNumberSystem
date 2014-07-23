#include <gtest/gtest.h>
#include <Data/dictionaries/base_dictionary_warehouse.cpp>

namespace constants
{
	const std::string test_filepath_1("testdir/testfile");
	const std::string test_filepath_2("testdir/testfile.aff");
	const std::string test_filepath_3("testdir/testfile.dic");
	const std::string codepage_iso("ISO8859-2");
}

using namespace boost::filesystem;

class base_dictonary_warehouse_tmp : public base_dictionary_warehouse
{
public:
	bool openFile(const std::string & filePath) { return true; }
	void loadFileContent(){}
	void removeDictionary(){}
	void close_file(){};
	std::string getWordByNdex(unsigned int index){return std::string(); }
};

class ut_base_dictionary_warehouse_test : public ::testing::Test
{
public:
	ut_base_dictionary_warehouse_test()
	{
	}

	void create_file(const std::string & filepath)
	{
		path dir(filepath);
		create_directory(dir.parent_path().string());
		std::ofstream ofs(filepath.c_str(), std::ofstream::out);
		ofs.close();
	}

	void delete_file(const std::string & filepath)
	{
		remove(filepath);
	}
	void create_aff_file(const std::string & filepath)
	{
		create_file(filepath);
		std::ofstream ofs(filepath.c_str(), std::ofstream::out);
		ofs<<constants::file_codepage_keyword<<"    "<<constants::codepage_iso<<std::endl;
		ofs.close();
	}
public:
	base_dictonary_warehouse_tmp	uut;	
};

TEST_F(ut_base_dictionary_warehouse_test, prepare_aff_file_empty)
{
	//Given
	std::string filepath;
	EXPECT_TRUE(filepath.empty());
	//When
	prepare_aff_file_path(filepath);
	//Then
	EXPECT_TRUE(filepath.empty());
}

TEST_F(ut_base_dictionary_warehouse_test, prepare_aff_file_fake_path)
{
	//Given
	create_file(constants::test_filepath_1);
	std::string filepath(constants::test_filepath_1);
	//When
	prepare_aff_file_path(filepath);
	//Then
	EXPECT_TRUE(filepath.empty());
	delete_file(constants::test_filepath_1);
}

TEST_F(ut_base_dictionary_warehouse_test, prepare_aff_file_aff_path)
{
	//Given
	create_file(constants::test_filepath_2);
	std::string filepath(constants::test_filepath_2);
	//When
	prepare_aff_file_path(filepath);
	//Then
	EXPECT_EQ(filepath, constants::test_filepath_2);
	delete_file(constants::test_filepath_2);
}

TEST_F(ut_base_dictionary_warehouse_test, prepare_aff_file_without_ext)
{
	//Given
	create_file(constants::test_filepath_1);
	create_file(constants::test_filepath_2);
	std::string filepath(constants::test_filepath_1);
	//When
	prepare_aff_file_path(filepath);
	//Then
	EXPECT_EQ(filepath, constants::test_filepath_2);
	delete_file(constants::test_filepath_2);
	delete_file(constants::test_filepath_1);
}

TEST_F(ut_base_dictionary_warehouse_test, prepare_aff_file_dic_ext)
{
	//Given
	create_file(constants::test_filepath_2);
	create_file(constants::test_filepath_3);
	std::string filepath(constants::test_filepath_3);
	//When
	prepare_aff_file_path(filepath);
	//Then
	EXPECT_EQ(filepath, constants::test_filepath_2);
	delete_file(constants::test_filepath_2);
	delete_file(constants::test_filepath_3);
}

TEST_F(ut_base_dictionary_warehouse_test, get_file_codepage_empty_path)
{
	//Given
	std::string filepath;
	//When
	std::string file_codepage = uut.get_file_codepage(filepath);
	//Then
	EXPECT_TRUE(file_codepage.empty());
}

TEST_F(ut_base_dictionary_warehouse_test, get_file_codepage_proper_file)
{
	//Given
	std::string filepath(constants::test_filepath_2);
	create_file(constants::test_filepath_1);
	create_aff_file(filepath);
	//When
	std::string file_codepage = uut.get_file_codepage(filepath);
	//Then
	EXPECT_FALSE(file_codepage.empty());
	EXPECT_EQ(file_codepage, constants::codepage_iso);
	delete_file(constants::test_filepath_1);
	delete_file(constants::test_filepath_2);
}
