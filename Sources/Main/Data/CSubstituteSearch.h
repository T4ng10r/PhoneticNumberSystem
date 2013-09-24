#ifndef _CSUBSTITUTESEARCH_INCLUDE_
#define _CSUBSTITUTESEARCH_INCLUDE_

#include <QtCore/QObject>
#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <Data/CDictionaryData.h>
#include <Data/CSubstituteSearchTypes.h>

class CSubstituteSearchPrivate;

class CSubstituteSearch : public QObject
{
     Q_OBJECT
     friend class CSubstituteSearchPrivate;
public:
     CSubstituteSearch();
     ~CSubstituteSearch(void);
	 void setDictionaryWords(boost::shared_ptr<CDictionaryData>);
	 void startSearchForNumber(const std::string & number);
	 const WordSearchResult & getSearchResult();
Q_SIGNALS:
	 void searchProgress(int current, int max);
	 void searchFinished();
protected:
     boost::scoped_ptr<CSubstituteSearchPrivate> privPart;
};
#endif //_CSUBSTITUTESEARCH_INCLUDE_