#include "path_manager.h"
#include <boost/filesystem.hpp>

std::string path_manager::executable_dir()
{
	std::string exec_path;
#if UNIX
#endif


	return std::string();
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

