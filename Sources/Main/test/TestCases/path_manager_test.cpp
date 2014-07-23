#include <gtest/gtest.h>
#include <Data/path_manager.h>
#include <boost/filesystem.hpp>

namespace constant
{
#ifdef __linux
	const std::string ut_exe_name("UnitTest");
#elif _WINDOWS
	const std::string ut_exe_name("UnitTest.exe");
#endif
}

using namespace boost::filesystem;

class ut_path_manager_test : public ::testing::Test
{
public:
	ut_path_manager_test()
	{
	}
};

TEST_F(ut_path_manager_test, current_program_path)
{
	std::string directory = path_manager::current_dir();
	EXPECT_EQ(directory, current_path().string());
}

TEST_F(ut_path_manager_test, executable_path)
{
	std::string directory = path_manager::executable_dir();
	EXPECT_FALSE(directory.empty());
	EXPECT_TRUE(is_directory(directory));
	
	path filepath(directory);
	filepath/=constant::ut_exe_name;
	EXPECT_TRUE(is_regular_file(filepath));
	EXPECT_TRUE(exists(filepath));
}

TEST_F(ut_path_manager_test, user_home_dir)
{
	std::string directory = path_manager::user_dir();
	EXPECT_FALSE(directory.empty());
}
