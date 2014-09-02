#include <gtest/gtest.h>
#include <memory>
#include <Data/dictionaries/dictionary_file_memory_map.cpp>

namespace constants
{
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

TEST_F(ut_dictionary_file_memory_map_test, get_word)
{
	//Given
	char buffer[] = "Testowy string z danymi";
	std::string filepath("dictionaries/pl_PL.dic");
	const char * memAddr = buffer;
	const char * endMemPos = buffer + sizeof(buffer);
	uut->parseLineWordCounts(&memAddr, endMemPos);

	//When 
	//uut->loadFileContent(filepath);
	//Then
	//EXPECT_EQ( uut->get_word_by_index(0), "");
}

