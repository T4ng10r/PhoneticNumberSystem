#include <data/DictionaryData.h>
#include <Tools/loggers.h>
#include <vector>
#include <stdio.h>
#include <boost/format.hpp>
#include <boost/filesystem.hpp>
#include <data/dictionaries/dictionary_file_memory_map.h>

std::string emtpystring;

//////////////////////////////////////////////////////////////////////////
DictionaryData::DictionaryData(void):_pimpl(new dictionary_file_memory_map(this))
{}
DictionaryData::~DictionaryData(void){}
bool DictionaryData::loadDictionary(const std::string & filePath)
{
	printLog(eDebug, eInfoLogLevel, str(boost::format("Loading dictionary file %1%") % filePath));
	_pimpl->fileCodepage = _pimpl->get_file_codepage( filePath );
	std::string path_ = boost::filesystem::current_path().string();
	if (_pimpl->openFile(filePath))
	{
		_pimpl->loadFileContent(filePath);
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
	return _pimpl->words_count_;
}
std::string DictionaryData::get_word_by_index(unsigned int index)
{
	if (index >= _pimpl->words_count_)
	{
		printLog(eDebug, eWarningLogLevel, str(boost::format("DictionaryData, incorrect index value (%1%)") % index));
		return emtpystring;
	}
	return _pimpl->get_word_by_index(index);
}
std::string DictionaryData::get_file_codepage()
{
	return _pimpl->fileCodepage;
}
