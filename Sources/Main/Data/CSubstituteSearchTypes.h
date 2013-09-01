#ifndef _CSUBSTITUTESEARCHTYPE_INCLUDE_
#define _CSUBSTITUTESEARCHTYPE_INCLUDE_
#include <string>
#include <list>

struct SuccessWord  
{
	bool bFullCoverage;
	std::string  word;
	//which and how many digits it can cover
	//each pair is a start and length value
	std::string  matchingLetters;
	std::string  coveredDigits;
	std::list< std::pair<unsigned int,unsigned int> > coveragePairs;
};

typedef std::list< SuccessWord >				WordSearchResult;

#endif //_CSUBSTITUTESEARCHTYPE_INCLUDE_