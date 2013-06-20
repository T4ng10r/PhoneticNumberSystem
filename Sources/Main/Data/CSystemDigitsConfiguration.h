#include <string>
#include <map>
#include <pair>

typedef std::map<int, std::pair<char,char> > SystemMap;

struct CSystemDigitsConfiguration
{
	std::string strName;
	SystemMap mSystem;
};