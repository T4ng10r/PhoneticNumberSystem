#ifndef _CSUBSTITUTESEARCHTYPE_INCLUDE_
#define _CSUBSTITUTESEARCHTYPE_INCLUDE_
#include <list>
#include <map>
#include <string>

#define max_index 1000000
typedef std::size_t StartingIndex;
// typedef std::pair<StartingIndex,StartingIndex> UnsignedPair;

struct MatchingPair {
    MatchingPair()
        : start_index(std::string::npos)
        , end_index(std::string::npos)
    {
    }
    MatchingPair(StartingIndex _start, StartingIndex _end)
        : start_index(_start)
        , end_index(_end)
    {
    }
    bool operator<(const MatchingPair& _other) const
    {
        if (start_index < _other.start_index)
            return true;
        if (start_index == _other.start_index && end_index < _other.end_index)
            return true;
        return false;
    }
    bool operator==(const MatchingPair& _other) const
    {
      if (start_index == _other.start_index && end_index == _other.end_index)
        return true;
      return false;
    }
    StartingIndex start_index;
    StartingIndex end_index;
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
    bool matching_pair(StartingIndex start_index, StartingIndex end_index) const
    {
        for (std::list<MatchingPair>::const_iterator it = coveredDigitsIndices.begin();
                 it != coveredDigitsIndices.end(); it++)
          if (it->start_index == start_index && it->end_index <= end_index)
            return true;
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
