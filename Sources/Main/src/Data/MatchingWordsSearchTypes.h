#ifndef _CSUBSTITUTESEARCHTYPE_INCLUDE_
#define _CSUBSTITUTESEARCHTYPE_INCLUDE_
#include <list>
#include <map>
#include <string>

typedef std::size_t StartingIndex;
// typedef std::pair<StartingIndex,StartingIndex> UnsignedPair;

struct MatchingPair {
    MatchingPair()
        : startIndex(std::string::npos)
        , endIndex(std::string::npos)
    {
    }
    MatchingPair(StartingIndex _start, StartingIndex _end)
        : startIndex(_start)
        , endIndex(_end)
    {
    }
    bool operator<(const MatchingPair& _other) const
    {
        if (startIndex < _other.startIndex)
            return true;
        if (startIndex == _other.startIndex && endIndex < _other.endIndex)
            return true;
        return false;
    }
    StartingIndex startIndex;
    StartingIndex endIndex;
};

struct MatchingWord;
// typedef std::list< MatchingWord >				MatchingWordsList;
typedef std::list<MatchingWord> MatchingWordsList;

typedef std::map<StartingIndex, MatchingWordsList> WordSearchResultMap;

struct MatchingWord {
  public:
    MatchingWord() {}
    MatchingWord(const MatchingWord& copy)
        : bFullCoverage(copy.bFullCoverage)
        , matchingLetters(copy.matchingLetters)
        , coveredDigitsIndices(copy.coveredDigitsIndices)
        , words(copy.words)
    {
    }
    MatchingWord(std::string word) { words.push_back(word); }
    void appendWords(std::string wordToAppend) { words.push_back(wordToAppend); }
    operator std::string() const { return words.front(); }
    const std::string& getWord() const { return words.front(); }
    bool operator==(const MatchingWord& other)
    {
        if (getWord() == other.getWord())
            return true;
        else
            return false;
    }

  public:
    bool bFullCoverage;
    // which and how many digits it can cover
    // each pair is a start and length value
    std::string             matchingLetters;
    std::list<MatchingPair> coveredDigitsIndices;
    std::list<std::string>  words;
};

typedef std::map<MatchingPair, MatchingWordsList> FittingWordsMap;

#endif //_CSUBSTITUTESEARCHTYPE_INCLUDE_
