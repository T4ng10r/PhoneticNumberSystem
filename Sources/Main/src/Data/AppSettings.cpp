#include <Data/AppSettings.h>
#include <Data/CAppSettingsKeywords.h>
#include <Tools/loggers.h>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/foreach.hpp>
#include <boost/format.hpp>
#include <boost/filesystem.hpp>
#include <iostream>
#include <sstream>

#define CONFIGURATION_FILE std::string("PhoneticNumberSystem.xml")
const std::string strConfigurationFileName("PhoneticNumberSystem.xml");

AppSettings::ptr AppSettings::_instance;

class AppSettingsPrivate
{
public:
	AppSettingsPrivate(AppSettings * ptrPublic);
	~AppSettingsPrivate();
	void loadSettings();
	void defaultValues();
	void getDigitsConfiguration();
	void saveSettings();
public:
	//boost::shared_ptr<CSubstituteValuesConfiguration>	m_ptrSubstValConf;
	AppSettings *							m_ptrPublic;
	boost::property_tree::ptree				m_ptrSubstValConf;
	std::vector<CSingleSubstituteDigitsConfiguration>	m_vDigitsConfiguration;
};
//////////////////////////////////////////////////////////////////////////
AppSettingsPrivate::AppSettingsPrivate(AppSettings * ptrPublic):m_ptrPublic(ptrPublic)
{
	loadSettings();
	getDigitsConfiguration();
	defaultValues();
}
AppSettingsPrivate::~AppSettingsPrivate()
{
}
void AppSettingsPrivate::saveSettings()
{
	boost::property_tree::xml_writer_settings<std::string> settings; ("\t", 1);
	write_xml(CONFIGURATION_FILE, *(static_cast<boost::property_tree::ptree*>(m_ptrPublic)),std::locale()/*, settings*/);
}
void AppSettingsPrivate::loadSettings()
{
	printLog(eDebug, eInfoLogLevel, "Loading properties file ("+CONFIGURATION_FILE+")");
	using boost::property_tree::ptree;
	try
	{
    std::basic_ifstream<char> stream(CONFIGURATION_FILE);
    boost::property_tree::ptree & tree = *m_ptrPublic;
		read_xml(stream, tree);
	}
	catch (boost::property_tree::xml_parser::xml_parser_error const&  /*ex*/)
	{
		printLog(eDebug, eErrorLogLevel, "Lack of properties file");
	}
	printLog(eDebug, eInfoLogLevel, "Loading properties file finished");
	m_ptrSubstValConf = m_ptrPublic->get_child(CONSONANTS_SETTINGS);
	printLog(eDebug, eDebugLogLevel, "AppSettings: loading settings from file finished");
}

void AppSettingsPrivate::getDigitsConfiguration()
{
	using boost::property_tree::ptree;
	CSingleSubstituteDigitsConfiguration  stSystemDigitsConfiguration;
	BOOST_FOREACH(const ptree::value_type &digitsConf, m_ptrSubstValConf)
	{
		if (digitsConf.first!="digits_configuration")
			continue;
		stSystemDigitsConfiguration.reset();
		BOOST_FOREACH(const ptree::value_type &digitsConfEntry, digitsConf.second)
		{
			if (digitsConfEntry.first=="name")
			{
				stSystemDigitsConfiguration.strName=digitsConfEntry.second.data();
				continue;
			}
			if (digitsConfEntry.first=="digit")
			{
				int iDigit(-1);
				std::pair<char,char>	stPair(' ',' ');
				BOOST_FOREACH(const ptree::value_type &digitsEntry, digitsConfEntry.second)
				{
					if (digitsEntry.first=="value")
					{
						iDigit=digitsEntry.second.get_value<int>();
					}
					if (digitsEntry.first=="substitue")
					{
						if (stPair.first==' ')
						{
							stPair.first = digitsEntry.second.data().at(0);
							stSystemDigitsConfiguration.allConsonants.push_back(stPair.first);
						}
						else if (stPair.second==' ')
						{
							stPair.second = digitsEntry.second.data().at(0);
							stSystemDigitsConfiguration.allConsonants.push_back(stPair.second);
						}
					}
				}
				stSystemDigitsConfiguration.mSystem[iDigit]=stPair;
				continue;
			}
		}
		stSystemDigitsConfiguration.createConsonantsDigitsMap();
		m_vDigitsConfiguration.push_back(stSystemDigitsConfiguration);
	}
}
void AppSettingsPrivate::defaultValues()
{
	try
	{
		m_ptrPublic->get<std::string>(DICTIONARIES_DIRECTORY);
	}
	catch (boost::property_tree::ptree_bad_path &/*e*/)
	{
		printLog(eDebug, eDebugLogLevel, str(boost::format("Lack of '%1% - adding default value") % DICTIONARIES_DIRECTORY));
		m_ptrPublic->put<std::string>(DICTIONARIES_DIRECTORY,DICTIONARIES_DIRECTORY_DEFAULT_VALUE);
	}
}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
AppSettings::AppSettings():pimpl(new AppSettingsPrivate(this))
{
	printLog(eDebug, eDebugLogLevel, "AppSettings created");
}
AppSettings::~AppSettings(){}
const boost::property_tree::ptree &AppSettings::getSubstituteValuesConfiguration()
{
	printLog(eDebug, eDebugLogLevel, "SubstituteValuesConfiguration provided");
	return pimpl->m_ptrSubstValConf;
}
AppSettings::ptr AppSettings::instance()
{
	if(!_instance)
	{
		if(!_instance)
		{
			_instance.reset(new AppSettings());
		}
	}
	return _instance;
}
void AppSettings::saveSettings()
{
	pimpl->saveSettings();
}
const std::vector<CSingleSubstituteDigitsConfiguration> & AppSettings::getDigitsConfiguraions()
{
	return pimpl->m_vDigitsConfiguration;
}
//std::string AppSettings::getCurrentDictPath()
//{
//	std::string strPath;
//	return strPath;
//}
void AppSettings::on_set_selected_consonant_system(const QString &)
{

}
//////////////////////////////////////////////////////////////////////////
CSubstituteValuesConfiguration::CSubstituteValuesConfiguration():
boost::property_tree::ptree(gAppSettings->getSubstituteValuesConfiguration())
{
}

