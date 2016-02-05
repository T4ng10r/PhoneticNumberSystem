#include <data/dictionaries/base_dictionary_warehouse.h>
#include <string>
#include <boost/format.hpp>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>
#include <Tools/loggers.h>

namespace constants
{
const std::string dictionary_ext(".dic");
const std::string aff_ext(".aff");
const std::string file_codepage_keyword("SET ");
}

void prepare_aff_file_path(std::string & file_path)
{
	boost::filesystem::path file(file_path);
	if (file.extension() == constants::aff_ext && boost::filesystem::exists(file_path))
	{
			return;
	}
	if (file.extension()==constants::dictionary_ext)
	{
		file.replace_extension(constants::aff_ext);
		if (boost::filesystem::exists(file))
		{
			file_path = file.string();
			return;
		}
	}

	if (!file.has_extension())
	{
		file+=constants::aff_ext;
		if (boost::filesystem::exists(file))
		{
			file_path = file.string();
			return;
		}
	}
  printLog(eDebug, eWarningLogLevel, str(boost::format("Lack of affinity file with dictionary file (%1%)") % file_path));
	file_path.clear();
}
base_dictionary_warehouse::base_dictionary_warehouse() :words_count_(0)
{
}
std::string base_dictionary_warehouse::get_file_codepage(std::string file_path )
{
	prepare_aff_file_path(file_path);
	if (file_path.empty())
	{
			return std::string();
	}

	std::ifstream ifs(file_path.c_str());
	if (!ifs.good())
	{
		printLog(eDebug, eWarningLogLevel,
		    str(boost::format("DictionaryData, can't open dictionary aff file %1%") % file_path));
		return std::string();
	}
	std::getline(ifs, fileCodepage);
	std::size_t pos;
	if ((pos=fileCodepage.find(constants::file_codepage_keyword))!=std::string::npos)
	{
		fileCodepage = fileCodepage.substr(pos+constants::file_codepage_keyword.size());
		boost::algorithm::trim(fileCodepage);
	} else {
		printLog(eDebug, eWarningLogLevel, str(boost::format("Lack of codepage definition")));
		fileCodepage.clear();
	}
	ifs.close();
	return fileCodepage;
}

std::ifstream::pos_type base_dictionary_warehouse::fileSize(const char* filename)
{
	return boost::filesystem::file_size(filename);
}

unsigned int base_dictionary_warehouse::words_count()
{
	return words_count_;
}

