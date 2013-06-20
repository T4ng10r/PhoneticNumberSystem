#include <Data/CAppSettings.h>
#include <tools/loggers.h>
#include <Data/CSystemDigitsConfiguration.h>
#include <boost/property_tree/xml_parser.hpp>
//#include <boost/foreach.hpp>

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
	std::vector<CSystemDigitsConfiguration>	m_vDigitsConfiguration;
};
//////////////////////////////////////////////////////////////////////////
CAppSettingsPrivate::CAppSettingsPrivate(CAppSettings * ptrPublic):m_ptrPublic(ptrPublic)
{
	loadSettings();
	getDigitsConfiguration();
}
CAppSettingsPrivate::~CAppSettingsPrivate(){}
void CAppSettingsPrivate::loadSettings()
{
	using boost::property_tree::ptree;
	read_xml(CONFIGURATION_FILE, *(static_cast<boost::property_tree::ptree*>(m_ptrPublic)));
	m_ptrSubstValConf = m_ptrPublic->get_child("settings.consonants");
	printLog(eDebugLogLevel, eDebug, "AppSettings: settiings loaded from file");
}
void CAppSettingsPrivate::getDigitsConfiguration()
{
	CSystemDigitsConfiguration  stSystemDigitsConfiguration;
	//BOOST_FOREACH(const ptree::value_type &singleEntry, m_ptrSubstValConf)
	//{
	//	if (singleEntry.first!="digits_configuration")
	//		continue;

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
//////////////////////////////////////////////////////////////////////////
CSubstituteValuesConfiguration::CSubstituteValuesConfiguration():
boost::property_tree::ptree(gAppSettings->getSubstituteValuesConfiguration())
{
	//boost::property_tree::ptree
	//this = stAppSettings.getSubstituteValuesConfiguration().get();
}
