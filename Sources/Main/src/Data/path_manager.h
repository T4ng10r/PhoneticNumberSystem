#ifndef _PATH_MANAGER_INCLUDE_
#define _PATH_MANAGER_INCLUDE_
#include <string>

class path_manager
{
public:
	static std::string executable_dir();
	static std::string current_dir();
	static std::string user_dir();
	static std::string user_settings_dir();
	static std::string global_setttings_dir();
};

#endif //_CAPPSETTINGS_INCLUDE_
