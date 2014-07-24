#include <Data/CDictionaryData.h>
#include <Tools/loggers.h>
#include <vector>
#include <stdio.h>
#include <boost/format.hpp>
#include <Data/dictionaries/dictionary_file_memory_map.h>

std::string emtpystring;

//////////////////////////////////////////////////////////////////////////
CDictionaryData::CDictionaryData(void):privPart(new dictionary_file_memory_map(this))
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
	return privPart->words_count_;
}
std::string CDictionaryData::getWordByNdex(unsigned int index)
{
	if (index >= privPart->words_count_)
	{
		printLog(eDebug, eWarningLogLevel, str(boost::format("CDictionaryData, incorrect index value (%1%)") % index));
		return emtpystring;
	}
	return privPart->getWordByNdex(index);
}
std::string CDictionaryData::get_file_codepage()
{
	return privPart->fileCodepage;
}
