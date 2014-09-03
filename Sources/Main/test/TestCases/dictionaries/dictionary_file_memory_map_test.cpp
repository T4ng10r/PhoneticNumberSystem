#include <gtest/gtest.h>
#include <memory>
#include <Data/dictionaries/dictionary_file_memory_map.cpp>
#include <boost/filesystem.hpp>

namespace constants
{
	const std::string dict_dir("dict");
	const std::string dict_file_name("pl_PL_short.dic");
	boost::filesystem::path dictionary(boost::filesystem::path(dict_dir) / boost::filesystem::path(dict_file_name));
	const std::string searched_word_01("abdominoplastyka");
	const std::string searched_word_02("aeroplanktonowy");
	const std::string code_page_word_01("abadañczyk"); 
	const unsigned int code_page_word_idx_01(22);
	const std::string code_page_word_02("adoptuj¹c"); //
	const unsigned int code_page_word_idx_02(804);
	const std::string code_page_word_03("afunkcjonalnoœæ"); //1240
	const unsigned int code_page_word_idx_03(1240);
}

using namespace boost::filesystem;

class ut_dictionary_file_memory_map_test : public ::testing::Test
{
public:
	ut_dictionary_file_memory_map_test ()
	{
		uut.reset(new dictionary_file_memory_map(NULL));
	}
	~ut_dictionary_file_memory_map_test(){}

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

public:
	std::unique_ptr<dictionary_file_memory_map> uut;
};

TEST_F(ut_dictionary_file_memory_map_test, empty_file_path)
{
	//Given
	std::string filepath;
	//When //Then
	EXPECT_FALSE(uut->openFile(filepath));
}

TEST_F(ut_dictionary_file_memory_map_test, parse_word_count_fail)
{
	//Given
	char buffer[] = "Testowy string z danymi";
	std::string filepath("dictionaries/pl_PL.dic");
	const char * memAddr = buffer;
	const char * endMemPos = buffer + sizeof(buffer);
	EXPECT_EQ(uut->words_count(),0);
	//When 
	uut->parseLineWordCounts(&memAddr, endMemPos);
	//Then
	EXPECT_EQ(uut->words_count(), 0);
	EXPECT_EQ(buffer + sizeof("Testowy") - 1, memAddr);
}

TEST_F(ut_dictionary_file_memory_map_test, parse_word_count)
{
	//Given
	char buffer[] = "1231 asd";
	std::string filepath("dictionaries/pl_PL.dic");
	const char * memAddr = buffer;
	const char * endMemPos = buffer + sizeof(buffer);
	EXPECT_EQ(uut->words_count(), 0);
	//When 
	uut->parseLineWordCounts(&memAddr, endMemPos);
	//Then
	EXPECT_EQ(uut->words_count(), 1231);
	EXPECT_EQ(buffer + sizeof("1231") - 1, memAddr);
}

TEST_F(ut_dictionary_file_memory_map_test, load_content)
{
	//Given
	EXPECT_EQ(uut->words_count(), 0);
	uut->openFile(constants::dictionary.string());
	//When 
	uut->loadFileContent(constants::dictionary.string());
	//Then
	EXPECT_EQ(uut->words_count(), 1245);
	EXPECT_EQ(uut->get_word_by_index(78), constants::searched_word_01);
	EXPECT_EQ(uut->get_word_by_index(1030), constants::searched_word_02);
}

TEST_F(ut_dictionary_file_memory_map_test, load_content_code_page)
{
	//Given
	EXPECT_EQ(uut->words_count(), 0);
	uut->openFile(constants::dictionary.string());
	//When 
	uut->loadFileContent(constants::dictionary.string());
	//Then
	EXPECT_EQ(uut->words_count(), 1245);
	EXPECT_EQ(uut->get_word_by_index(constants::code_page_word_idx_01), constants::code_page_word_01);
	EXPECT_EQ(uut->get_word_by_index(constants::code_page_word_idx_02), constants::code_page_word_02);
	EXPECT_EQ(uut->get_word_by_index(constants::code_page_word_idx_03), constants::code_page_word_03);
}

