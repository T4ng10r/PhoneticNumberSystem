#include <string>
#include <map>
//#include <pair>

typedef std::map<int, std::pair<char,char> > SystemMap;

struct CSystemDigitsConfiguration
{
	void reset()
	{
		strName.clear();
		mSystem.clear();
	}
	std::string strName;
	SystemMap mSystem;
};