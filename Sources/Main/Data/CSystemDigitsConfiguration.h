#include <string>
#include <map>
//#include <pair>

typedef std::map<int, std::pair<char,char> > SystemMap;

struct CSingleSubstituteDigitsConfiguration
{
	void reset()
	{
		strName.clear();
		mSystem.clear();
	}
	std::string strName;
	SystemMap mSystem;
};