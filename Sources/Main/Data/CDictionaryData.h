#ifndef _CDICTIONARYDATA_INCLUDE_
#define _CDICTIONARYDATA_INCLUDE_

#include <boost/scoped_ptr.hpp>
#include <string>

//class CDictionaryDataPrivate;
class BaseDictionaryWarhouse;

// s³ownik
class CDictionaryData
{
     friend class CDictionaryDataPrivate;
public:
     CDictionaryData(void);
	 ~CDictionaryData(void);
	 void loadDictionary(const std::string & filePath);
	 unsigned int getWordsCount();
	 std::string getWordByNdex(unsigned int index);
protected:
	//boost::scoped_ptr<CDictionaryDataPrivate> privPart;
	boost::scoped_ptr<BaseDictionaryWarhouse> privPart;
};
#endif //_CDICTIONARYDATA_INCLUDE_