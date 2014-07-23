#include <Data/dictionaries/base_dictionary_warehouse.h>
#include <string>
#include <boost/filesystem.hpp>

const std::string dictionary_ext(".dic");
const std::string aff_ext(".aff");

void prepare_aff_file_path(std::string & file_path)
{
	boost::filesystem::path file(file_path);
	if (file.extension() == aff_ext && boost::filesystem::exists(file_path))
	{
			return;
	}
	if (file.extension()==dictionary_ext)
	{
		file.replace_extension(aff_ext);
		if (boost::filesystem::exists(file))
		{
			file_path = file.string();
			return;
		}
	}

	if (!file.has_extension())
	{
		file+=aff_ext;
		if (boost::filesystem::exists(file))
		{
			file_path = file.string();
			return;
		}
	}

	file_path.clear();
}
	

std::string BaseDictionaryWarehouse::get_file_codepage( const std::string & file_path )
{
/*	if (!boost::filesystem::exists(file_path))
	{
	}

	std::ifstream file(file_path, std::ifstream::in);


	process_file_path();
	size_t pos;
	std::string affFilePath(file_path);
	std::string fileCodepage;
	boost::filesystem::path(file_path).extension()
	if ((pos=affFilePath.find(file_dictionary_ext))!=std::string::npos)
	{
		affFilePath = affFilePath.substr(0,pos);
		affFilePath.append(file_dictionary_aff_ext);
	}
	file.setFileName(affFilePath.c_str());
	QTextStream stream(&file);
	if (!file.open(QIODevice::ReadOnly))
	{
		printLog(eDebug, eWarningLogLevel,
		    str(boost::format("CDictionaryData, can't open dictionary aff file %1%") % affFilePath));
		return fileCodepage;
	}
	fileCodepage = stream.readLine().toStdString();
	if ((pos=fileCodepage.find(file_codepage_keyword))!=std::string::npos)
	{
		fileCodepage = fileCodepage.substr(pos+file_codepage_keyword.size());
	}
	file.close();
	fileCodepage.erase(std::remove(fileCodepage.begin(), fileCodepage.end(), '\n'), fileCodepage.end());
	fileCodepage.erase(std::remove(fileCodepage.begin(), fileCodepage.end(), '\r'), fileCodepage.end());*/
	return fileCodepage;
}

std::ifstream::pos_type fileSize(const char* filename)
{
	if (false == boost::filesystem::exists(filename))
		return 0;
		std::ifstream in(filename, std::ifstream::in | std::ifstream::binary);
		in.seekg(0, std::ifstream::end);
		return in.tellg();
	}
	bool isFileExist(const char* filename)
	{
		return boost::filesystem::exists(filename);
	}
