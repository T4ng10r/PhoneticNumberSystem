#include <gtest/gtest.h>
#include <Data/path_manager.h>
#include <boost/filesystem.hpp>

namespace constant
{
}

class ut_path_manager_test : public ::testing::Test
{
public:
	ut_path_manager_test()
	{
	}
	path_manager uut;
};

TEST_F(ut_path_manager_test, current_program_path)
{
	std::string directory = uut.current_dir();
	EXPECT_EQ(directory, boost::filesystem::current_path().string());
}

TEST_F(ut_path_manager_test, executable_path)
{
	std::string directory = uut.executable_dir();
	EXPECT_FALSE(directory.empty());
}
