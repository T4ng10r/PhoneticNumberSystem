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

struct SuccessWord;
typedef std::list< SuccessWord >				WordSearchResult;
struct SuccessWord  
{
	SuccessWord(){}
	SuccessWord(const std::string & word)
	{
		words.push_back(word);
	}
	const std::string &getWord() const {return words.front(); }
	void appendWords( const std::string & wordToAppend)
	{
		words.push_back(wordToAppend);
	}
	bool bFullCoverage;
	std::list<std::string>  words;
	//which and how many digits it can cover
	//each pair is a start and length value
	std::string  matchingLetters;
	std::list< MatchingPair > coveragePairs;
};

typedef std::map<MatchingPair, WordSearchResult > 		FittingWordsMap;

#endif //_CSUBSTITUTESEARCHTYPE_INCLUDE_