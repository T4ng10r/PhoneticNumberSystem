#ifndef _CSINGLE_SUBSTITUTE_DIGITS_CONF_INCLUDE_
#define _CSINGLE_SUBSTITUTE_DIGITS_CONF_INCLUDE_

#include <string>
#include <map>

typedef std::map<unsigned int, std::pair<char,char> > SystemMap;
typedef std::string AcceptableConsonants;
typedef std::string ForbidenConsonants;
typedef std::map<unsigned int, std::pair<AcceptableConsonants, ForbidenConsonants> > OneDigitConsonantsSet;

struct CSingleSubstituteDigitsConfiguration
{
public:
	void reset()
	{
		strName.clear();
		mSystem.clear();
		allConsonants.clear();
		digitsConsonantsSetMap.clear();
	}
	void create_empty_system()
	{
		std::pair<char,char>	stPair(' ',' ');
		for(int i=0;i<10;i++)
		{
			mSystem[i]=stPair;
		}

	}
	std::string strName;
	SystemMap mSystem; 
	std::string allConsonants;
	OneDigitConsonantsSet	digitsConsonantsSetMap;
public:
	void createConsonantsDigitsMap();
};
#endif //_CSINGLE_SUBSTITUTE_DIGITS_CONF_INCLUDE_