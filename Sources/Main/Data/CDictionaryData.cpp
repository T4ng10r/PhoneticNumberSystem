#include <Data/CDictionaryData.h>
#include <tools/loggers.h>
#include <vector>
#include <stdio.h>
#include <algorithm>
#include <QTextCodec>
#include <QTextStream>
#include <QFile>
#include <boost/interprocess/file_mapping.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <boost/lexical_cast.hpp>
#include <iostream>
#include <fstream>

#define BUFSIZE  65536
#define WINDOWS_EOF_1CHAR '\r'
#define WINDOWS_EOF_2CHAR '\r'
#define LINUX_EOF_CHAR '\n'
const std::string file_dictionary_ext(".dic");
const std::string file_dictionary_aff_ext(".aff");
const std::string file_codepage_keyword("SET ");
std::string emtpystring;

class BaseDictionaryWarehouse
{
public:
	virtual bool openFile(const std::string & filePath)=0;
	virtual void loadFileContent()=0;
	virtual void removeDictionary()=0;
	virtual void close_file() = 0;
	virtual std::string getWordByNdex(unsigned int index)=0;
	std::string getFileCodepage( const std::string & filePath )
	{
		//if path contain '.dic'
		QFile file;
		size_t pos;
		std::string affFilePath(filePath);
		std::string fileCodepage;
		if ((pos=affFilePath.find(file_dictionary_ext))!=std::string::npos)
		{
			affFilePath = affFilePath.substr(0,pos);
			affFilePath.append(file_dictionary_aff_ext);
		}
		file.setFileName(filePath.c_str());
		QTextStream stream(&file);
		if (!file.open(QIODevice::ReadOnly))
		{
			printLog(eWarningLogLevel,eDebug,QString("CDictionaryData, can't open dictionary aff file (%1)").arg(affFilePath.c_str()));
			return fileCodepage;
		}
		fileCodepage = stream.readLine().toLatin1();
		if ((pos=fileCodepage.find(file_codepage_keyword))!=std::string::npos)
		{
			fileCodepage = fileCodepage.substr(pos+file_codepage_keyword.size());
		}
		file.close();
		fileCodepage.erase(std::remove(fileCodepage.begin(), fileCodepage.end(), '\n'), fileCodepage.end());
		fileCodepage.erase(std::remove(fileCodepage.begin(), fileCodepage.end(), '\r'), fileCodepage.end());
		QTextCodec::setCodecForLocale(QTextCodec::codecForName (fileCodepage.c_str())); 
		return fileCodepage;
	}		
	std::ifstream::pos_type fileSize(const char* filename)
	{
		if (false==isFileExist(filename))
			return 0;
		std::ifstream in(filename, std::ifstream::in | std::ifstream::binary);
		in.seekg(0, std::ifstream::end);
		return in.tellg();
	}
	bool isFileExist(const char* filename)
	{
		return QFile::exists(filename);
	}
public:
	char in[BUFSIZE + 50]; // input buffer
	CDictionaryData *		m_ptrPublic;
	std::string				fileCodepage;
	unsigned int			wordsCount;
};
//////////////////////////////////////////////////////////////////////////
class CDictionaryDataFileMemoryMap : public BaseDictionaryWarehouse
{
public:
	CDictionaryDataFileMemoryMap(CDictionaryData * ptrPublic)
	{ m_ptrPublic = ptrPublic;}
	~CDictionaryDataFileMemoryMap(){};
	bool openFile(const std::string & filePath)
	{
		if (false==isFileExist(filePath.c_str()))
			return false;
		dictionaryFileSize = fileSize(filePath.c_str());
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
		if (index<0 || index >= wordsCount)
		{
			return std::string();
		}
		//QString qWord = std::string(dictionaryWords[index].first,dictionaryWords[index].second).c_str();
		//return qWord.toStdString();
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
     CDictionaryDataPrivate(CDictionaryData * ptrPublic);
     ~CDictionaryDataPrivate();
	 bool openFile(const std::string & filePath);
	 void loadFileContent();
	 void close_file(){};
	 void removeDictionary();
	 std::string getFileCodepage( const std::string & filePath );
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
	//dict_file.open(filePath.c_str());
	//fileHandle = fopen(filePath.c_str(), "r");

	if (false==QFile::exists(filePath.c_str()) || dict_file.open(QIODevice::ReadOnly))
	{
		printLog(eWarningLogLevel,eDebug,QString("CDictionaryData, can't open dictionary file (%1)").arg(filePath.c_str()));
		dict_file.close();
		return false;
	}
	return true;
}
void CDictionaryDataPrivate::loadFileContent()
{
	removeDictionary();
	//set coding for 
	//QTextCodec::setCodecForCStrings(QTextCodec::codecForName (fileCodepage.c_str())); 
	QTextCodec::setCodecForLocale(QTextCodec::codecForName (fileCodepage.c_str())); 
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
	printLog(eInfoLogLevel,eDebug,QString("Loading dictionary file (%1)").arg(filePath.c_str()));
	privPart->fileCodepage = privPart->getFileCodepage( filePath );
	if (privPart->openFile(filePath))
	{
		privPart->loadFileContent();
		privPart->close_file();
	}
	else
		return false;
	printLog(eDebugLogLevel,eDebug,QString("Loading dictionary finished"));
	return true;
}
unsigned int CDictionaryData::getWordsCount()
{
	return privPart->wordsCount;
}
std::string CDictionaryData::getWordByNdex(unsigned int index)
{
	if (index<0 || index >= privPart->wordsCount)
	{
		printLog(eWarningLogLevel,eDebug,QString("CDictionaryData, incorrect index value (%1)").arg(index));
		return emtpystring;
	}
	return privPart->getWordByNdex(index);
#ifdef USE_STL_VECTOR
	return privPart->dictionaryWords[index];
#else
	//return privPart->dictionaryWordsArray[index];
#endif
}
