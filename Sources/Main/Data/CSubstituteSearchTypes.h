#ifndef _CSUBSTITUTESEARCHTYPE_INCLUDE_
#define _CSUBSTITUTESEARCHTYPE_INCLUDE_
#include <string>
#include <list>
#include <map>

typedef std::pair<unsigned int,unsigned int> UnsignedPair;

struct MatchingPair : public UnsignedPair
{
	MatchingPair(){}
	MatchingPair(UnsignedPair::first_type firstVal, UnsignedPair::second_type secondVal){
		first = firstVal;
		second = secondVal;
	}

	bool operator()(const MatchingPair& rightVal)
	{
		if (first<rightVal.first)
			return true;
		if (first==rightVal.first && second<rightVal.second)
			return true;
		return false;
	}
};

struct SuccessWord  
{
	std::string getWord() {return words.front(); }
	bool bFullCoverage;
	std::list<std::string>  words;
	//which and how many digits it can cover
	//each pair is a start and length value
	std::string  matchingLetters;
	std::list< MatchingPair > coveragePairs;
};

typedef std::list< SuccessWord >				WordSearchResult;
typedef std::map<MatchingPair, WordSearchResult > 		FittingWordsMap;

#endif //_CSUBSTITUTESEARCHTYPE_INCLUDE_