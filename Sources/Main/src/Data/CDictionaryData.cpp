#include <Data/CDictionaryData.h>
#include <Tools/loggers.h>
#include <vector>
#include <stdio.h>
#include <algorithm>
#include <QTextCodec>
#include <QTextStream>
#include <QFile>
#include <boost/interprocess/file_mapping.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/format.hpp>
#include <boost/filesystem.hpp>
#include <iostream>
#include <fstream>
#include <Data/dictionaries/base_dictionary_warehouse.h>

#define BUFSIZE  65536
#define WINDOWS_EOF_1CHAR '\r'
#define WINDOWS_EOF_2CHAR '\n'
#define LINUX_EOF_CHAR '\n'
const std::string file_dictionary_ext(".dic");
const std::string file_dictionary_aff_ext(".aff");
const std::string file_codepage_keyword("SET ");
std::string emtpystring;

/////////////////////////////////////////////////////////////////////////
class CDictionaryDataFileMemoryMap : public BaseDictionaryWarehouse
{
public:
	CDictionaryData * m_ptrPublic;
public:
	CDictionaryDataFileMemoryMap(CDictionaryData * ptrPublic)
	{ m_ptrPublic = ptrPublic;}
	~CDictionaryDataFileMemoryMap(){};
	bool openFile(const std::string & filePath)
	{
		if (false==boost::filesystem::exists(filePath.c_str()))
			return false;
		dictionaryFileSize = boost::filesystem::file_size(filePath.c_str());
		fileMapping = boost::interprocess::file_mapping
			(filePath.c_str()
			,boost::interprocess::read_only
			);
		return true;
	}
	void close_file(){};
	void parseLineWordCounts(const char ** memAddr, const char * const endMemPos)
	{
		const char * startAddr = *memAddr;
		while(!isspace(**memAddr) && *memAddr<endMemPos)
		{
			(*memAddr)++;
		}
		if (*memAddr<endMemPos)
		{
			wordsCount = boost::lexical_cast<unsigned int>(std::string(startAddr,*memAddr-startAddr));
		}
	}
	void moveTillEndOfLine(const char ** memAddr, const char * const endMemPos)
	{
		while(*memAddr<endMemPos )
		{
			if (**memAddr==WINDOWS_EOF_1CHAR && *(*memAddr+1)==WINDOWS_EOF_2CHAR)
			{
				(*memAddr)++;
				(*memAddr)++;
				return;
			}
			if (**memAddr==LINUX_EOF_CHAR)
			{
				(*memAddr)++;
				return;
			}
			(*memAddr)++;
		}
	}
	void parseLineWithWord(const char ** memAddr, const char * const endMemPos)
	{
		const char * startAddr = *memAddr;
		while(*memAddr<endMemPos)
		{
			if (**memAddr=='/')
			{
				dictionaryWords.push_back(std::make_pair(startAddr,*memAddr-startAddr));
				return;
			}
			if ((**memAddr==WINDOWS_EOF_1CHAR && *(*memAddr+1)==WINDOWS_EOF_2CHAR) || **memAddr==LINUX_EOF_CHAR)
			{
				dictionaryWords.push_back(std::make_pair(startAddr,*memAddr-startAddr));
				return;
			}
			(*memAddr)++;
		}
	}

	void loadFileContent()
	{
		fileMappedRegion = boost::interprocess::mapped_region
			( fileMapping                   //Memory-mappable object
			, boost::interprocess::read_only               //Access mode
			, 0               //Offset from the beginning of shm
			, dictionaryFileSize      //Length of the region
			);

		//process all entries into pair - start offset and length
		//get entries count
		const char *mem = static_cast<char*>(fileMappedRegion.get_address());
		const char *endMemPos = mem+fileMappedRegion.get_size();
		parseLineWordCounts(&mem, endMemPos);
		while(mem<endMemPos)
		{
			moveTillEndOfLine(&mem, endMemPos);
			parseLineWithWord(&mem, endMemPos);
		}

	}
	std::string getWordByNdex(unsigned int index)
	{
		if (index >= wordsCount)
		{
			return std::string();
		}
		return std::string(dictionaryWords[index].first,dictionaryWords[index].second);
	}
	void removeDictionary()
	{
		dictionaryWords.clear();
	};
public:
	std::size_t dictionaryFileSize;
	boost::interprocess::file_mapping	fileMapping;
	boost::interprocess::mapped_region  fileMappedRegion;
	//vector of pairs - offset and length of each word
	std::vector< std::pair<const char *, long long int> >	dictionaryWords;
};

//////////////////////////////////////////////////////////////////////////
class CDictionaryDataPrivate : public BaseDictionaryWarehouse
{
public:
	CDictionaryData * m_ptrPublic;
public:
     CDictionaryDataPrivate(CDictionaryData * ptrPublic);
     ~CDictionaryDataPrivate();
	 bool openFile(const std::string & filePath);
	 void loadFileContent();
	 void close_file(){};
	 void removeDictionary();
	 std::string get_file_codepage( const std::string & filePath );
public:
	 FILE *					fileHandle;
	 QFile					dict_file;
	 int					linenum;
	 std::vector<std::string>	dictionaryWords;
};

CDictionaryDataPrivate::CDictionaryDataPrivate(CDictionaryData * ptrPublic):linenum(0),fileHandle(NULL)
#ifndef USE_STL_VECTOR
	//,dictionaryWordsArray(0)
#endif
{
	wordsCount = 0;
	m_ptrPublic = ptrPublic;
	linenum = 0;
}
CDictionaryDataPrivate::~CDictionaryDataPrivate()
{
	removeDictionary();
}
void CDictionaryDataPrivate::removeDictionary()
{
#ifdef USE_STL_VECTOR
	std::vector<std::string> cleanOne;
	dictionaryWords.swap(cleanOne);
#else
	//delete[] dictionaryWordsArray;
#endif
}
bool CDictionaryDataPrivate::openFile( const std::string & filePath )
{
	dict_file.setFileName(filePath.c_str());

	if (false==boost::filesystem::exists(filePath.c_str()) || dict_file.open(QIODevice::ReadOnly))
	{
		printLog(eDebug, eWarningLogLevel,str(boost::format("CDictionaryData, can't open dictionary file %1%") %filePath));
		dict_file.close();
		return false;
	}
	return true;
}
void CDictionaryDataPrivate::loadFileContent()
{
	removeDictionary();
	//set coding for 
	QTextStream stream(&dict_file);

	QString qLine;// = fgets(in, BUFSIZE - 1, fileHandle);
	//get words count
	unsigned int wordsCount;// = qLine.toUInt();
	stream >> wordsCount;
	if (wordsCount<=0)
		return;
	int pos;
	int index(0);
	this->wordsCount = wordsCount;
#ifdef USE_STL_VECTOR
	dictionaryWords.resize(wordsCount+2);
#else
	//dictionaryWordsArray = new std::string[wordsCount];
#endif

	//qLine = fgets(in, BUFSIZE - 1, fileHandle);
	stream >> qLine;
	//get single line
	do 
	{
		//remove every thing AFTER / letter
		if ((pos=qLine.indexOf('/'))>=0)
		{
			qLine=qLine.left(pos);
		}
		qLine = qLine.trimmed();
#ifdef USE_STL_VECTOR
		dictionaryWords[index++]=qLine.toStdString();
#else
		//dictionaryWordsArray[index++]=qLine.toStdString();
#endif
		//qLine = fgets(in, BUFSIZE - 1, fileHandle);
		stream >> qLine;
	}
	while(qLine.length()>0);
}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
CDictionaryData::CDictionaryData(void):privPart(new CDictionaryDataFileMemoryMap(this))
{}
CDictionaryData::~CDictionaryData(void){}
bool CDictionaryData::loadDictionary(const std::string & filePath)
{
	printLog(eDebug, eInfoLogLevel, str(boost::format("Loading dictionary file %1%") % filePath));
	privPart->fileCodepage = privPart->get_file_codepage( filePath );
	if (privPart->openFile(filePath))
	{
		privPart->loadFileContent();
		privPart->close_file();
	}
	else
		return false;
	printLog(eDebug,eDebugLogLevel,"Loading dictionary finished");
	return true;
}
unsigned int CDictionaryData::getWordsCount()
{
	return privPart->wordsCount;
}
std::string CDictionaryData::getWordByNdex(unsigned int index)
{
	if (index >= privPart->wordsCount)
	{
		printLog(eDebug, eWarningLogLevel, str(boost::format("CDictionaryData, incorrect index value (%1%)") % index));
		return emtpystring;
	}
	return privPart->getWordByNdex(index);
#ifdef USE_STL_VECTOR
	return privPart->dictionaryWords[index];
#else
	//return privPart->dictionaryWordsArray[index];
#endif
}
std::string CDictionaryData::get_file_codepage()
{
  return privPart->fileCodepage;
}
