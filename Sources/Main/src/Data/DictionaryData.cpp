#include <Data/DictionaryData.h>
#include <Tools/loggers.h>
#include <vector>
#include <stdio.h>
#include <boost/format.hpp>
#include <boost/filesystem.hpp>
#include <Data/dictionaries/dictionary_file_memory_map.h>

std::string emtpystring;

//////////////////////////////////////////////////////////////////////////
DictionaryData::DictionaryData(void):privPart(new dictionary_file_memory_map(this))
{}
DictionaryData::~DictionaryData(void){}
bool DictionaryData::loadDictionary(const std::string & filePath)
{
	printLog(eDebug, eInfoLogLevel, str(boost::format("Loading dictionary file %1%") % filePath));
	privPart->fileCodepage = privPart->get_file_codepage( filePath );
	std::string path_ = boost::filesystem::current_path().string();
	if (privPart->openFile(filePath))
	{
		privPart->loadFileContent(filePath);
		printLog(eDebug, eDebugLogLevel, "Loading dictionary finished");
	}
	else
	{
		printLog(eDebug, eErrorLogLevel, "Loading dictionary failed");
		return false;
	}
	return true;
}
unsigned int DictionaryData::getWordsCount()
{
	return privPart->words_count_;
}
std::string DictionaryData::get_word_by_index(unsigned int index)
{
	if (index >= privPart->words_count_)
	{
		printLog(eDebug, eWarningLogLevel, str(boost::format("DictionaryData, incorrect index value (%1%)") % index));
		return emtpystring;
	}
	return privPart->get_word_by_index(index);
}
std::string DictionaryData::get_file_codepage()
{
	return privPart->fileCodepage;
}
