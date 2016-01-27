#pragma once

#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <string>

class base_dictionary_warehouse;

// slownik
class DictionaryData
{
	friend class CDictionaryDataPrivate;
public:
  typedef boost::shared_ptr<DictionaryData> ptr;

	DictionaryData(void);
	~DictionaryData(void);
	bool loadDictionary(const std::string & filePath);
	unsigned int getWordsCount();
	std::string get_word_by_index(unsigned int index);
	std::string get_file_codepage();
protected:
	boost::scoped_ptr<base_dictionary_warehouse> privPart;
};

