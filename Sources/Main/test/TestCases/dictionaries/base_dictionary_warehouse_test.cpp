#include <gtest/gtest.h>
#include <Data/dictionaries/base_dictionary_warehouse.cpp>

namespace constant
{
	const std::string test_filepath_1("testdir/testfile");
	const std::string test_filepath_2("testdir/testfile.aff");
	const std::string test_filepath_3("testdir/testfile.dic");
}

using namespace boost::filesystem;

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
};

TEST_F(ut_base_dictionary_warehouse_test, prepare_aff_file_empty)
{
	std::string file_path;
	EXPECT_TRUE(file_path.empty());
	prepare_aff_file_path(file_path);
	EXPECT_TRUE(file_path.empty());
}

TEST_F(ut_base_dictionary_warehouse_test, prepare_aff_file_fake_path)
{
	create_file(constant::test_filepath_1);
	std::string file_path(constant::test_filepath_1);
	prepare_aff_file_path(file_path);
	EXPECT_TRUE(file_path.empty());
	delete_file(constant::test_filepath_1);
}

TEST_F(ut_base_dictionary_warehouse_test, prepare_aff_file_aff_path)
{
	create_file(constant::test_filepath_2);
	std::string file_path(constant::test_filepath_2);
	prepare_aff_file_path(file_path);
	EXPECT_EQ(file_path, constant::test_filepath_2);
	delete_file(constant::test_filepath_2);
}

TEST_F(ut_base_dictionary_warehouse_test, prepare_aff_file_without_ext)
{
	create_file(constant::test_filepath_1);
	create_file(constant::test_filepath_2);
	std::string file_path(constant::test_filepath_1);
	prepare_aff_file_path(file_path);
	EXPECT_EQ(file_path, constant::test_filepath_2);
	delete_file(constant::test_filepath_2);
	delete_file(constant::test_filepath_1);
}

TEST_F(ut_base_dictionary_warehouse_test, prepare_aff_file_dic_ext)
{
	create_file(constant::test_filepath_2);
	create_file(constant::test_filepath_3);
	std::string file_path(constant::test_filepath_3);
	prepare_aff_file_path(file_path);
	EXPECT_EQ(file_path, constant::test_filepath_2);
	delete_file(constant::test_filepath_2);
	delete_file(constant::test_filepath_3);
}
