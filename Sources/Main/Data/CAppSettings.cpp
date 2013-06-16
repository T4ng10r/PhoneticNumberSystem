#include <Data/CAppSettings.h>
#include <tools/loggers.h>
#include <boost/property_tree/xml_parser.hpp>
//#include <boost/foreach.hpp>

#define CONFIGURATION_FILE std::string("PhoneticNumberSystem.xml")
const std::string strConfigurationFileName("PhoneticNumberSystem.xml");
extern CAppSettings	stAppSettings;

class CAppSettingsPrivate
{
public:
	CAppSettingsPrivate(CAppSettings * ptrPublic);
	~CAppSettingsPrivate();
	void loadSettings();
	void defaultValues();
public:
	//Catalog catalog; // this is the container of your objects
	//boost::shared_ptr<CSubstituteValuesConfiguration>	m_ptrSubstValConf;
	CAppSettings *							m_ptrPublic;
	boost::property_tree::ptree				m_ptrSubstValConf;
};
//////////////////////////////////////////////////////////////////////////
CAppSettingsPrivate::CAppSettingsPrivate(CAppSettings * ptrPublic):m_ptrPublic(ptrPublic)
{
	loadSettings();
}
CAppSettingsPrivate::~CAppSettingsPrivate(){}
void CAppSettingsPrivate::loadSettings()
{
	using boost::property_tree::ptree;
	read_xml(CONFIGURATION_FILE, *(static_cast<boost::property_tree::ptree*>(m_ptrPublic)));
	m_ptrSubstValConf = m_ptrPublic->get_child("settings.consonants");
	ptree::iterator iter1 = m_ptrSubstValConf.begin();
	iter1->first.data();
	iter1->second.data();
	//BOOST_FOREACH(const ptree::value_type &v, m_ptrPublic)
	//{
	//	std::string st = v.second.data();
	//	int a=1;
	//}
}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
CAppSettings::CAppSettings(void):m_ptrPriv(new CAppSettingsPrivate(this))
{
	printLog(eDebugLogLevel, eDebug, "AppSettings created");
}
CAppSettings::~CAppSettings(void){}
const boost::property_tree::ptree &CAppSettings::getSubstituteValuesConfiguration()
{
	return m_ptrPriv->m_ptrSubstValConf;
}
//////////////////////////////////////////////////////////////////////////
CSubstituteValuesConfiguration::CSubstituteValuesConfiguration():
boost::property_tree::ptree(stAppSettings.getSubstituteValuesConfiguration())
{
	//boost::property_tree::ptree
	//this = stAppSettings.getSubstituteValuesConfiguration().get();
}
