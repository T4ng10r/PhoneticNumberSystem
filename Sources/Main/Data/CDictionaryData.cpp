#include <Data/CDictionaryData.h>
#include <tools/loggers.h>
#include <vector>
#include <stdio.h>
#include <algorithm>
#include <QtCore/QTextCodec>
#define BUFSIZE  65536
//#define USE_STL_VECTOR 

std::string emtpystring;
class CDictionaryDataPrivate
{
public:
     CDictionaryDataPrivate(CDictionaryData * ptrPublic);
     ~CDictionaryDataPrivate();
	 bool openFile(const std::string & filePath);
	 void loadFileContent();
	 void removeDictionary();
	 std::string CDictionaryDataPrivate::getFileCodepage( const std::string & filePath );
public:
     CDictionaryData *      m_ptrPublic;
	 std::string			fileCodepage;
	 FILE *					fileHandle;
	 int					linenum;
	 char in[BUFSIZE + 50]; // input buffer
	 unsigned int				wordsCount;
#ifdef USE_STL_VECTOR
	 std::vector<std::string>	dictionaryWords;
#else
	 std::string *				dictionaryWordsArray;
#endif
};

CDictionaryDataPrivate::CDictionaryDataPrivate(CDictionaryData * ptrPublic):m_ptrPublic(ptrPublic)
#ifndef USE_STL_VECTOR
	,dictionaryWordsArray(0)
#endif
{
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
	delete[] dictionaryWordsArray;
#endif
}
std::string CDictionaryDataPrivate::getFileCodepage( const std::string & filePath )
{
	//if path contain '.dic'
	size_t pos;
	std::string affFilePath(filePath);
	std::string fileCodepage;
	if ((pos=affFilePath.find(".dic"))!=std::string::npos)
	{
		affFilePath = affFilePath.substr(0,pos);
		affFilePath.append(".aff");
	}
	FILE * fh = fopen(affFilePath.c_str(), "r");
	if (!fh) 
	{
		printLog(eWarningLogLevel,eDebug,QString("CDictionaryData, can't open dictionary aff file (%1)").arg(affFilePath.c_str()));
		return fileCodepage;
	}
	fileCodepage = fgets(in, BUFSIZE - 1, fh);
	if ((pos=fileCodepage.find("SET "))!=std::string::npos)
	{
		fileCodepage = fileCodepage.substr(pos+4);
	}
	fclose(fh);
	fileCodepage.erase(std::remove(fileCodepage.begin(), fileCodepage.end(), '\n'), fileCodepage.end());
	fileCodepage.erase(std::remove(fileCodepage.begin(), fileCodepage.end(), '\r'), fileCodepage.end());
	return fileCodepage;
}
bool CDictionaryDataPrivate::openFile( const std::string & filePath )
{
	fileHandle = fopen(filePath.c_str(), "r");
	if (!fileHandle) 
	{
		printLog(eWarningLogLevel,eDebug,QString("CDictionaryData, can't open dictionary file (%1)").arg(filePath.c_str()));
		return false;
	}
	return true;
}
void CDictionaryDataPrivate::loadFileContent()
{
	removeDictionary();
	//set coding for 
	QTextCodec::setCodecForCStrings(QTextCodec::codecForName (fileCodepage.c_str())); 

	QString qLine = fgets(in, BUFSIZE - 1, fileHandle);
	//get words count
	unsigned int wordsCount = qLine.toUInt();
	if (wordsCount<=0)
		return;
	int pos;
	int index(0);
	this->wordsCount = wordsCount;
#ifdef USE_STL_VECTOR
	dictionaryWords.resize(wordsCount+2);
#else
	dictionaryWordsArray = new std::string[wordsCount];
#endif
	qLine = fgets(in, BUFSIZE - 1, fileHandle);
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
		dictionaryWordsArray[index++]=qLine.toStdString();
#endif
		qLine = fgets(in, BUFSIZE - 1, fileHandle);
	}
	while(qLine.length()>0);
}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
CDictionaryData::CDictionaryData(void):privPart(new CDictionaryDataPrivate(this))
{}
CDictionaryData::~CDictionaryData(void){}
void CDictionaryData::loadDictionary(const std::string & filePath)
{
	printLog(eInfoLogLevel,eDebug,QString("Loading dictionary file (%1)").arg(filePath.c_str()));
	privPart->fileCodepage = privPart->getFileCodepage( filePath );
	if (privPart->openFile(filePath))
		privPart->loadFileContent();
	printLog(eDebugLogLevel,eDebug,QString("Loading dictionary finished"));
}
unsigned int CDictionaryData::getWordsCount()
{
	return privPart->wordsCount;
}
const std::string & CDictionaryData::getWordByNdex(unsigned int index)
{
	if (index<0 || index >= privPart->wordsCount)
	{
		printLog(eWarningLogLevel,eDebug,QString("CDictionaryData, incorrect index value (%1)").arg(index));
		return emtpystring;
	}
#ifdef USE_STL_VECTOR
	return privPart->dictionaryWords[index];
#else
	return privPart->dictionaryWordsArray[index];
#endif
}
