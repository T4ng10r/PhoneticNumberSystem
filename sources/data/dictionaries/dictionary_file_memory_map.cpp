#include "dictionary_file_memory_map.h"
#include <boost/filesystem.hpp>
#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>
#include <data/logging_base.h>
#include <loggers.h>

#define WINDOWS_EOF_1CHAR '\r'
#define WINDOWS_EOF_2CHAR '\n'
#define LINUX_EOF_CHAR '\n'

class dictionary_file_memory_map_private : public LoggingBase
{
  public:
    dictionary_file_memory_map_private();
};

dictionary_file_memory_map_private::dictionary_file_memory_map_private()
    : LoggingBase()
{
}

dictionary_file_memory_map::dictionary_file_memory_map(DictionaryData* ptrPublic)
    : _pimpl(new dictionary_file_memory_map_private())
{
    m_ptrPublic = ptrPublic;
}
//--------------------------------------------------------------------------------
bool dictionary_file_memory_map::openFile(const std::string& filePath)
{
    if (false == boost::filesystem::exists(filePath.c_str()))
        return false;
    dictionaryFileSize = boost::filesystem::file_size(filePath.c_str());
    return true;
}
//--------------------------------------------------------------------------------
void dictionary_file_memory_map::close_file(){};
//--------------------------------------------------------------------------------
void dictionary_file_memory_map::parseLineWordCounts(const char** memAddr, const char* const endMemPos)
{
    const char* startAddr = *memAddr;
    while (!isspace(**memAddr) && *memAddr < endMemPos) {
        (*memAddr)++;
    }
    if (*memAddr < endMemPos) {
        try {
            words_count_ = boost::lexical_cast<unsigned int>(std::string(startAddr, *memAddr - startAddr));
        } catch (boost::bad_lexical_cast e) {
          _pimpl->logger.log(log4cplus::ERROR_LOG_LEVEL,
            "Failed to convert word count");
        }
    }
}
//--------------------------------------------------------------------------------
void dictionary_file_memory_map::moveTillEndOfLine(const char** memAddr, const char* const endMemPos)
{
    while (*memAddr < endMemPos) {
        if (**memAddr == WINDOWS_EOF_1CHAR && *(*memAddr + 1) == WINDOWS_EOF_2CHAR) {
            (*memAddr)++;
            (*memAddr)++;
            return;
        }
        if (**memAddr == LINUX_EOF_CHAR) {
            (*memAddr)++;
            return;
        }
        (*memAddr)++;
    }
}
//--------------------------------------------------------------------------------
void dictionary_file_memory_map::parseLineWithWord(const char** memAddr, const char* const endMemPos)
{
    const char* startAddr = *memAddr;
    while (*memAddr < endMemPos) {
        if (**memAddr == '/') {
            dictionaryWords.push_back(std::make_pair(startAddr, *memAddr - startAddr));
            return;
        }
        if ((**memAddr == WINDOWS_EOF_1CHAR && *(*memAddr + 1) == WINDOWS_EOF_2CHAR) || **memAddr == LINUX_EOF_CHAR) {
            dictionaryWords.push_back(std::make_pair(startAddr, *memAddr - startAddr));
            return;
        }
        (*memAddr)++;
    }
}
//--------------------------------------------------------------------------------
void dictionary_file_memory_map::create_content_index_map()
{
    // process all entries into pair - start offset and length
    // get entries count
    const char* mem       = static_cast<char*>(fileMappedRegion.get_address());
    const char* endMemPos = mem + fileMappedRegion.get_size();
    parseLineWordCounts(&mem, endMemPos);
    while (mem < endMemPos) {
        moveTillEndOfLine(&mem, endMemPos);
        parseLineWithWord(&mem, endMemPos);
    }
}
//--------------------------------------------------------------------------------
void dictionary_file_memory_map::loadFileContent(const std::string& filePath)
{
    file_mapping     = boost::interprocess::file_mapping(filePath.c_str(), boost::interprocess::read_only);
    fileMappedRegion = boost::interprocess::mapped_region(file_mapping // Memory-mappable object
                                                          ,
                                                          boost::interprocess::read_only // Access mode
                                                          ,
                                                          0 // Offset from the beginning of shm
                                                          ,
                                                          dictionaryFileSize // Length of the region
                                                          );

    create_content_index_map();
}
//--------------------------------------------------------------------------------
std::string dictionary_file_memory_map::get_word_by_index(unsigned int index)
{
    return std::string(dictionaryWords[index].first, dictionaryWords[index].second);
}
//--------------------------------------------------------------------------------
void dictionary_file_memory_map::removeDictionary() { dictionaryWords.clear(); };
