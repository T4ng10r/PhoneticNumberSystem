#ifndef _CSUBSTITUTESEARCH_INCLUDE_
#define _CSUBSTITUTESEARCH_INCLUDE_

#include <QtCore/QObject>
#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <Data/CDictionaryData.h>
#include <Data/CSubstituteSearchTypes.h>
#include <Data/CSystemDigitsConfiguration.h>

class CSubstituteSearchPrivate;

class CSubstituteSearch : public QObject
//execute "!find . \( -name '*.h' -o -name '*.cpp' \) -exec sed 's|\\bCSubstituteSearch\\b|MatchingWordsSearch|g' {} \\;"

                          
{
     Q_OBJECT
     friend class CSubstituteSearchPrivate;
public:
    CSubstituteSearch();
    ~CSubstituteSearch(void);
    void setSubstituteDigitsConfiguration(CSingleSubstituteDigitsConfiguration conf);
    void setDictionaryWords(boost::shared_ptr<CDictionaryData>);
    void startSearchForNumber(const std::string & number);
    //const WordsList & getSearchResult();
    WordsList getSearchResult(StartingIndex start_index = 0);
Q_SIGNALS:
    void searchProgress(int current, int max);
    void searchFinished(bool);
protected:
    boost::scoped_ptr<CSubstituteSearchPrivate> privPart;
};
#endif //_CSUBSTITUTESEARCH_INCLUDE_
