#include "path_manager.h"
#include <boost/filesystem.hpp>
#ifdef __linux
#include <unistd.h>
#define PROC_PATH "/proc/self/exe"
#endif

std::string path_manager::executable_dir()
{
	std::string exec_path;
#ifdef __linux
	char path[65000]={0};
  readlink(PROC_PATH, path, 6500);
	exec_path=path;
#endif

	return exec_path;
}
std::string path_manager::current_dir()
{
	return boost::filesystem::current_path().string();
}
std::string path_manager::user_dir()
{
	return std::string();
}
std::string path_manager::user_settings_dir()
{
	return std::string();
}
std::string path_manager::global_setttings_dir()
{
	return std::string();
}

