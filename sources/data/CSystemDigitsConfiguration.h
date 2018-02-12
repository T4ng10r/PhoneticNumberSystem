#ifndef _CSINGLE_SUBSTITUTE_DIGITS_CONF_INCLUDE_
#define _CSINGLE_SUBSTITUTE_DIGITS_CONF_INCLUDE_

#include <map>
#include <string>
#include <vector>

//
typedef std::vector<std::pair<char, char>> SystemMap;
typedef std::string AcceptableConsonants;
typedef std::string ForbidenConsonants;
struct ConsonantsForDigit {
    std::string acceptable;
    std::string forbiden;
};
// typedef std::map<unsigned int, std::pair<AcceptableConsonants, ForbidenConsonants> > OneDigitConsonantsSet;
typedef std::map<unsigned int, ConsonantsForDigit> OneDigitConsonantsSet;

struct CSingleSubstituteDigitsConfiguration {
  public:
    void reset()
    {
        strName.clear();
        mSystem.clear();
        allConsonants.clear();
        digitsConsonantsSetMap.clear();
        mSystem.resize(10);
    }
    void create_empty_system()
    {
        std::pair<char, char> stPair(' ', ' ');
        mSystem.resize(10);
        for (int i = 0; i < 10; i++) {
            mSystem[i] = stPair;
        }
    }
    std::string           strName;
    SystemMap             mSystem;
    std::string           allConsonants; //all acceptable consonants
    OneDigitConsonantsSet digitsConsonantsSetMap;

  public:
    void createConsonantsDigitsMap();
};
#endif //_CSINGLE_SUBSTITUTE_DIGITS_CONF_INCLUDE_
