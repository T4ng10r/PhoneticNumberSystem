#include <data/dictionary_data.h>
//#include <tools/loggers.h>
#include <boost/filesystem.hpp>
#include <boost/format.hpp>
#include <data/dictionaries/dictionary_file_memory_map.h>
#include <log4cplus/logger.h>
#include <stdio.h>
#include <vector>

std::string emptystring;

//////////////////////////////////////////////////////////////////////////
DictionaryData::DictionaryData(void)
    : LoggingBase()
    , _pimpl(new dictionary_file_memory_map(this))
{
}
DictionaryData::~DictionaryData(void) {}
bool DictionaryData::loadDictionary(const std::string& filePath)
{
    logger.log(log4cplus::INFO_LOG_LEVEL, str(boost::format("Loading dictionary file %1%") % filePath));
    _pimpl->fileCodepage = _pimpl->get_file_codepage(filePath);
    std::string path_    = boost::filesystem::current_path().string();
    if (_pimpl->openFile(filePath)) {
        _pimpl->loadFileContent(filePath);
        logger.log(log4cplus::DEBUG_LOG_LEVEL, str(boost::format("Loading dictionary finished")));
    } else {
        logger.log(log4cplus::ERROR_LOG_LEVEL, str(boost::format("Loading dictionary failed")));
        return false;
    }
    return true;
}
unsigned int DictionaryData::getWordsCount() { return _pimpl->words_count_; }
std::string DictionaryData::get_word_by_index(unsigned int index)
{
    if (index >= _pimpl->words_count_) {
        logger.log(log4cplus::WARN_LOG_LEVEL, str(boost::format("DictionaryData, incorrect index value (%1%)") % index));

        return emptystring;
    }
    return _pimpl->get_word_by_index(index);
}
std::string DictionaryData::get_file_codepage() { return _pimpl->fileCodepage; }
