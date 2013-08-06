#include <Data/CAppSettings.h>
#include <tools/loggers.h>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/foreach.hpp>

#define CONFIGURATION_FILE std::string("PhoneticNumberSystem.xml")
const std::string strConfigurationFileName("PhoneticNumberSystem.xml");

CAppSettings* CAppSettings::pInstance_=0;

class CAppSettingsPrivate
{
public:
	CAppSettingsPrivate(CAppSettings * ptrPublic);
	~CAppSettingsPrivate();
	void loadSettings();
	void defaultValues();
	void getDigitsConfiguration();
public:
	//Catalog catalog; // this is the container of your objects
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
}
CAppSettingsPrivate::~CAppSettingsPrivate()
{
	write_xml(CONFIGURATION_FILE, *(static_cast<boost::property_tree::ptree*>(m_ptrPublic)));
}
void CAppSettingsPrivate::loadSettings()
{
	using boost::property_tree::ptree;
	read_xml(CONFIGURATION_FILE, *(static_cast<boost::property_tree::ptree*>(m_ptrPublic)));
	m_ptrSubstValConf = m_ptrPublic->get_child("settings.consonants");
	printLog(eDebugLogLevel, eDebug, "AppSettings: settings loaded from file");
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
						}
						else if (stPair.second==' ')
						{
							stPair.second = digitsEntry.second.data().at(0);
						}
					}
				}
				stSystemDigitsConfiguration.mSystem[iDigit]=stPair;
				continue;
			}
		}
		m_vDigitsConfiguration.push_back(stSystemDigitsConfiguration);
	}
}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
CAppSettings::CAppSettings():m_ptrPriv(new CAppSettingsPrivate(this))
{
	printLog(eDebugLogLevel, eDebug, "AppSettings created");
}
CAppSettings::~CAppSettings(){}
const boost::property_tree::ptree &CAppSettings::getSubstituteValuesConfiguration()
{
	printLog(eDebugLogLevel, eDebug, "SubstituteValuesConfiguration provided");
	return m_ptrPriv->m_ptrSubstValConf;
}
CAppSettings* CAppSettings::getInstance()
{
	if(!pInstance_)
	{
		if(!pInstance_)
		{
			CAppSettings * p = new CAppSettings();
			pInstance_ = p;
		}
	}
	return pInstance_;
}
const std::vector<CSingleSubstituteDigitsConfiguration> & CAppSettings::getDigitsConfiguraions()
{
	return m_ptrPriv->m_vDigitsConfiguration;
}
//////////////////////////////////////////////////////////////////////////
CSubstituteValuesConfiguration::CSubstituteValuesConfiguration():
boost::property_tree::ptree(gAppSettings->getSubstituteValuesConfiguration())
{
}