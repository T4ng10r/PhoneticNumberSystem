#include <Data/CSystemDigitsConfiguration.h>
#include <algorithm>

void CSingleSubstituteDigitsConfiguration::createConsonantsDigitsMap()
{
	for(unsigned int iDigit=0;iDigit<10;iDigit++)
	{
		ForbidenConsonants	forbidenConsonants(allConsonants);
		std::pair<char,char> & digitChars = mSystem[iDigit];
		AcceptableConsonants acceptableConsonants;
		acceptableConsonants.push_back(digitChars.first);
		acceptableConsonants.push_back(digitChars.second);
		for (unsigned int i = 0; i < acceptableConsonants.length(); ++i)
			forbidenConsonants.erase (std::remove(forbidenConsonants.begin(), forbidenConsonants.end(), acceptableConsonants[i]), forbidenConsonants.end());
    ConsonantsForDigit consonats = {acceptableConsonants, forbidenConsonants};
		digitsConsonantsSetMap[iDigit]=consonats;
	}
}
