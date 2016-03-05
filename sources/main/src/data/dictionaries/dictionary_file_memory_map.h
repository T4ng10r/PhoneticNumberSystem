#pragma once
#include "base_dictionary_warehouse.h"
#include <boost/interprocess/file_mapping.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <vector>

class DictionaryData;
class dictionary_file_memory_map_private;

class dictionary_file_memory_map : public base_dictionary_warehouse
{
public:
	DictionaryData * m_ptrPublic;
public:
	dictionary_file_memory_map(DictionaryData * ptrPublic);
	~dictionary_file_memory_map();
	bool openFile(const std::string & filePath);
	void close_file();
	void parseLineWordCounts(const char ** memAddr, const char * const endMemPos);
	void moveTillEndOfLine(const char ** memAddr, const char * const endMemPos);
	void parseLineWithWord(const char ** memAddr, const char * const endMemPos);
	void loadFileContent(const std::string & filePath);
	std::string get_word_by_index(unsigned int index);
	void removeDictionary();
	void create_content_index_map();
public:
	std::size_t dictionaryFileSize;
	boost::interprocess::file_mapping   file_mapping;
	boost::interprocess::mapped_region  fileMappedRegion;
	//vector of pairs - offset and length of each word
	std::vector< std::pair<const char *, long long int> >  dictionaryWords;
protected:
  boost::scoped_ptr<dictionary_file_memory_map_private> _pimpl;
};


