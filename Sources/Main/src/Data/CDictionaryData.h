#ifndef _CDICTIONARYDATA_INCLUDE_
#define _CDICTIONARYDATA_INCLUDE_

#include <boost/scoped_ptr.hpp>
#include <string>

//class CDictionaryDataPrivate;
class base_dictionary_warehouse;

// s³ownik
class CDictionaryData
{
     friend class CDictionaryDataPrivate;
public:
     CDictionaryData(void);
	 ~CDictionaryData(void);
	 bool loadDictionary(const std::string & filePath);
	 unsigned int getWordsCount();
	 std::string getWordByNdex(unsigned int index);
   std::string get_file_codepage();
protected:
	//boost::scoped_ptr<CDictionaryDataPrivate> privPart;
	boost::scoped_ptr<base_dictionary_warehouse> privPart;
};
#endif //_CDICTIONARYDATA_INCLUDE_
