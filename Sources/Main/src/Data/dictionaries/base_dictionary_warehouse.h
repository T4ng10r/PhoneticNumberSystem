#pragma once
#include <string>
#include <fstream>

#define BUFSIZE  65536

class BaseDictionaryWarehouse
{
public:
	virtual bool openFile(const std::string & filePath) = 0;
	virtual void loadFileContent() = 0;
	virtual void removeDictionary() = 0;
	virtual void close_file() = 0;
	virtual std::string getWordByNdex(unsigned int index) = 0;
	std::string get_file_codepage( const std::string & filePath );
	std::ifstream::pos_type fileSize(const char* filename);
	bool isFileExist(const char* filename);
public:
	char in[BUFSIZE + 50]; // input buffer
	std::string				fileCodepage;
	unsigned int			wordsCount;
};
