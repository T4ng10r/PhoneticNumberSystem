#include "path_manager.h"
#include <boost/filesystem.hpp>
#ifdef __linux
	#include <unistd.h>
	#include <stdlib.h>
	#define PROC_PATH "/proc/self/exe"
	#define ENV_HOME_VAR "HOME"
	#define USER_SETTINGS_FOLDER ".phoneticnumbersystem"
#elif _WINDOWS
	#include <ShlObj.h>
	#include <stdlib.h>
	#include <comutil.h>
	#include <Windows.h>
	#pragma comment(lib, "comsuppw")
	#define ENV_HOME_VAR "HOMEPATH"
	#define USER_SETTINGS_FOLDER "PhoneticNumberSystem"
#endif
#define BUFSIZE 65000


std::string path_manager::executable_dir()
{
	std::string exec_path;
	char path[BUFSIZE] = { 0 };
#ifdef __linux
	readlink(PROC_PATH, path, BUFSIZE);
#elif _WINDOWS
	GetModuleFileName(NULL, path, BUFSIZE);
#endif
	exec_path = boost::filesystem::path(path).parent_path().string();
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
#elif _WINDOWS
	PWSTR path = NULL;
	SHGetKnownFolderPath(FOLDERID_Profile, 0, NULL, &path);
	_bstr_t bstrPath(path);
	return std::string((char*)bstrPath);
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

