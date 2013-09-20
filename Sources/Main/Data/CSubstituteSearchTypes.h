#ifndef _CSUBSTITUTESEARCHTYPE_INCLUDE_
#define _CSUBSTITUTESEARCHTYPE_INCLUDE_
#include <string>
#include <list>
#include <map>
#include <boost/shared_ptr.hpp>

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
	bool bFullCoverage;
	//std::string  word;
	std::list<std::string>  words;
	//which and how many digits it can cover
	//each pair is a start and length value
	std::string  matchingLetters;
	//std::string  coveredDigits;
	std::list< MatchingPair > coveragePairs;
};

struct SearchResultTreeNode
{
public:
	SearchResultTreeNode():iCurrentIndex(-1){}

public:
	//second index of range defining covered part of number
	int iCurrentIndex;
	boost::shared_ptr<SearchResultTreeNode> parent;

	boost::shared_ptr<SearchResultTreeNode> left;
	boost::shared_ptr<SearchResultTreeNode> rigth;
	std::list<SuccessWord>	words;
};



typedef std::list< SuccessWord >				WordSearchResult;
typedef std::map<MatchingPair, WordSearchResult > 		FittingWordsMap;

#endif //_CSUBSTITUTESEARCHTYPE_INCLUDE_