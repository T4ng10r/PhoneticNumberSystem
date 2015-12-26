#ifndef _CSUBSTITUTESEARCHTYPE_INCLUDE_
#define _CSUBSTITUTESEARCHTYPE_INCLUDE_
#include <string>
#include <list>
#include <map>

typedef std::size_t StartingIndex;
typedef std::pair<StartingIndex,StartingIndex> UnsignedPair;

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

struct MatchingWord;
//typedef std::list< MatchingWord >				WordsList;
typedef std::list<MatchingWord>	WordsList;

typedef std::map<unsigned int, WordsList>				WordSearchResultMap;

struct MatchingWord  
{
public:
	MatchingWord(){}
	MatchingWord(std::string word)
	{
		words.push_back(word);
	}
	void appendWords( std::string wordToAppend)
	{
		words.push_back(wordToAppend);
	}
	operator std::string() const		{ return words.front(); }
	const std::string &getWord() const	{ return words.front(); }
	bool operator==(const MatchingWord & other)
	{
		if (getWord()==other.getWord())
			return true;
		else 
			return false;
	}
public:
	bool bFullCoverage;
	//which and how many digits it can cover
	//each pair is a start and length value
	std::string  matchingLetters;
	std::list< MatchingPair > coveragePairs;
	std::list<std::string>  words;
};

typedef std::map<MatchingPair, WordsList > 		FittingWordsMap;

#endif //_CSUBSTITUTESEARCHTYPE_INCLUDE_
