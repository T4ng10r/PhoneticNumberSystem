#include <gtest/gtest.h>
#include <Data/dictionaries/base_dictionary_warehouse.cpp>

namespace constant
{
}

using namespace boost::filesystem;

class ut_base_dictionary_warehouse_test : public ::testing::Test
{
public:
	ut_base_dictionary_warehouse_test()
	{
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
	std::string file_path("testdir/testfile");
	prepare_aff_file_path(file_path);
	EXPECT_TRUE(file_path.empty());
}

