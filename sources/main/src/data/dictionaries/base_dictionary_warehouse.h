#pragma once
#include <string>
#include <fstream>
#include <boost/scoped_ptr.hpp>

#define BUFSIZE  65536

class base_dictionary_warehouse_private;

class base_dictionary_warehouse
{

public:
	base_dictionary_warehouse();
  ~base_dictionary_warehouse();
	virtual bool openFile(const std::string & filePath) = 0;
	virtual void loadFileContent(const std::string & filePath) = 0;
	virtual void removeDictionary() = 0;
	virtual void close_file() = 0;
	virtual std::string get_word_by_index(unsigned int index) = 0;
	unsigned int words_count();
	std::string get_file_codepage(std::string filePath );
	std::ifstream::pos_type fileSize(const char* filename);
public:
	char in[BUFSIZE + 50]; // input buffer
	std::string    fileCodepage;
	unsigned int   words_count_;
protected:
	boost::scoped_ptr<base_dictionary_warehouse_private> _pimpl;
};
