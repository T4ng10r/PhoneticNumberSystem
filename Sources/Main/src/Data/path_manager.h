#ifndef _PATH_MANAGER_INCLUDE_
#define _PATH_MANAGER_INCLUDE_
#include <string>

class path_manager
{
public:
	std::string executable_dir();
	std::string current_dir();
	std::string user_dir();
	std::string user_settings_dir();
	std::string global_setttings_dir();
};

#endif //_CAPPSETTINGS_INCLUDE_
