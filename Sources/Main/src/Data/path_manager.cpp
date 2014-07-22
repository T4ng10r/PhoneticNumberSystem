#include "path_manager.h"
#include <boost/filesystem.hpp>
#ifdef __linux
	#include <unistd.h>
	#include <stdlib.h>
	#define PROC_PATH "/proc/self/exe"
	#define ENV_HOME_VAR "HOME"
	#define USER_SETTINGS_FOLDER ".phoneticnumbersystem"
#elif __win
	#define ENV_HOME_VAR "HOMEPATH"
	#define USER_SETTINGS_FOLDER
#endif


std::string path_manager::executable_dir()
{
	std::string exec_path;
#ifdef __linux
	char path[65000]={0};
  readlink(PROC_PATH, path, 6500);
	exec_path = boost::filesystem::path(path).parent_path().string();
#endif
	return exec_path;
}
std::string path_manager::current_dir()
{
	return boost::filesystem::current_path().string();
}
std::string path_manager::user_dir()
{
#if __linux
	return std::string(getenv(ENV_HOME_VAR));
#elif __win
	char path[65000]={0};
	SHGetFolderPath(NULL, CSIDL_PROFILE, NULL,0, path);
	return std::string(path);
#endif
}
std::string path_manager::user_settings_dir()
{
	return std::string(user_dir()+USER_SETTINGS_FOLDER);
}
std::string path_manager::global_setttings_dir()
{
	return executable_dir();
}

