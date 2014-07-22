#include <Data/CAppSettings.h>
#include <Data/CAppSettingsKeywords.h>
#include <Tools/loggers.h>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/foreach.hpp>
#include <boost/format.hpp>
#include <boost/filesystem.hpp>

#define CONFIGURATION_FILE std::string("PhoneticNumberSystem.xml")
const std::string strConfigurationFileName("PhoneticNumberSystem.xml");

boost::shared_ptr<CAppSettings> CAppSettings::pInstance_;

class CAppSettingsPrivate
{
public:
	CAppSettingsPrivate(CAppSettings * ptrPublic);
	~CAppSettingsPrivate();
	void loadSettings();
	void defaultValues();
	void getDigitsConfiguration();
	void saveSettings();
public:
	//boost::shared_ptr<CSubstituteValuesConfiguration>	m_ptrSubstValConf;
	CAppSettings *							m_ptrPublic;
	boost::property_tree::ptree				m_ptrSubstValConf;
	std::vector<CSingleSubstituteDigitsConfiguration>	m_vDigitsConfiguration;
};
//////////////////////////////////////////////////////////////////////////
CAppSettingsPrivate::CAppSettingsPrivate(CAppSettings * ptrPublic):m_ptrPublic(ptrPublic)
{
	loadSettings();
	getDigitsConfiguration();
	defaultValues();
}
CAppSettingsPrivate::~CAppSettingsPrivate()
{
}
void CAppSettingsPrivate::saveSettings()
{
	boost::property_tree::xml_writer_settings<char> settings('\t', 1);
	write_xml(CONFIGURATION_FILE, *(static_cast<boost::property_tree::ptree*>(m_ptrPublic)),std::locale(), settings);
}
void CAppSettingsPrivate::loadSettings()
{
	printLog(eDebug, eInfoLogLevel, "Loading properties file");
	using boost::property_tree::ptree;
	try
	{
		read_xml(CONFIGURATION_FILE, *(static_cast<boost::property_tree::ptree*>(m_ptrPublic)), boost::property_tree::xml_parser::trim_whitespace);
	}
	catch (boost::property_tree::xml_parser::xml_parser_error const&  /*ex*/)
	{
		printLog(eDebug, eErrorLogLevel, "Lack of properties file");
	}
	printLog(eDebug, eInfoLogLevel, "Loading properties file finished");
	m_ptrSubstValConf = m_ptrPublic->get_child(CONSONANTS_SETTINGS);
	printLog(eDebug, eDebugLogLevel, "AppSettings: loading settings from file finished");
}

void CAppSettingsPrivate::getDigitsConfiguration()
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
void CAppSettingsPrivate::defaultValues()
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
CAppSettings::CAppSettings():m_ptrPriv(new CAppSettingsPrivate(this))
{
	printLog(eDebug, eDebugLogLevel, "AppSettings created");
}
CAppSettings::~CAppSettings(){}
const boost::property_tree::ptree &CAppSettings::getSubstituteValuesConfiguration()
{
	printLog(eDebug, eDebugLogLevel, "SubstituteValuesConfiguration provided");
	return m_ptrPriv->m_ptrSubstValConf;
}
boost::shared_ptr<CAppSettings> CAppSettings::getInstance()
{
	if(!pInstance_)
	{
		if(!pInstance_)
		{
			pInstance_.reset(new CAppSettings());
		}
	}
	return pInstance_;
}
void CAppSettings::saveSettings()
{
	m_ptrPriv->saveSettings();
}
const std::vector<CSingleSubstituteDigitsConfiguration> & CAppSettings::getDigitsConfiguraions()
{
	return m_ptrPriv->m_vDigitsConfiguration;
}
std::string CAppSettings::getCurrentDictPath()
{
	std::string strPath;
	return strPath;
}
void CAppSettings::on_set_selected_consonant_system(const QString &)
{

}
//////////////////////////////////////////////////////////////////////////
CSubstituteValuesConfiguration::CSubstituteValuesConfiguration():
boost::property_tree::ptree(gAppSettings->getSubstituteValuesConfiguration())
{
}

