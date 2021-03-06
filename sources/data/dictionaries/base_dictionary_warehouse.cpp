#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include <boost/format.hpp>
#include <data/dictionaries/base_dictionary_warehouse.h>
#include <data/logging_base.h>
#include <string>

namespace constants
{
const std::string dictionary_ext(".dic");
const std::string aff_ext(".aff");
const std::string file_codepage_keyword("SET ");
}

void prepare_aff_file_path(std::string& file_path)
{
  log4cplus::Logger logger = log4cplus::Logger::getInstance("DEBUG");
    boost::filesystem::path file(file_path);
    if (file.extension() == constants::aff_ext && boost::filesystem::exists(file_path)) {
        return;
    }
    if (file.extension() == constants::dictionary_ext) {
        file.replace_extension(constants::aff_ext);
        if (boost::filesystem::exists(file)) {
            file_path = file.string();
            return;
        }
    }

    if (!file.has_extension()) {
        file += constants::aff_ext;
        if (boost::filesystem::exists(file)) {
            file_path = file.string();
            return;
        }
    }
    logger.log(log4cplus::WARN_LOG_LEVEL,
             str(boost::format("Lack of affinity file with dictionary file (%1%)") % file_path));
    file_path.clear();
}

class base_dictionary_warehouse_private : public LoggingBase
{
  public:
    base_dictionary_warehouse_private();
    ~base_dictionary_warehouse_private();
};
base_dictionary_warehouse_private::base_dictionary_warehouse_private()
    : LoggingBase()
{
}
base_dictionary_warehouse_private::~base_dictionary_warehouse_private() {}

base_dictionary_warehouse::base_dictionary_warehouse()
    : words_count_(0)
    , _pimpl(new base_dictionary_warehouse_private())
{
}
base_dictionary_warehouse::~base_dictionary_warehouse() {}
std::string base_dictionary_warehouse::get_file_codepage(std::string file_path)
{
    prepare_aff_file_path(file_path);
    if (file_path.empty()) {
        return std::string();
    }

    std::ifstream ifs(file_path.c_str());
    if (!ifs.good()) {
        _pimpl->logger.log(log4cplus::WARN_LOG_LEVEL,
                           str(boost::format("DictionaryData, can't open dictionary aff file %1%") % file_path));
        return std::string();
    }
    std::getline(ifs, fileCodepage);
    std::size_t pos;
    if ((pos = fileCodepage.find(constants::file_codepage_keyword)) != std::string::npos) {
        fileCodepage = fileCodepage.substr(pos + constants::file_codepage_keyword.size());
        boost::algorithm::trim(fileCodepage);
    } else {
        _pimpl->logger.log(log4cplus::WARN_LOG_LEVEL, str(boost::format("Lack of codepage definition")));
        fileCodepage.clear();
    }
    ifs.close();
    return fileCodepage;
}

std::ifstream::pos_type base_dictionary_warehouse::fileSize(const char* filename)
{
    return boost::filesystem::file_size(filename);
}

unsigned int base_dictionary_warehouse::words_count() { return words_count_; }
