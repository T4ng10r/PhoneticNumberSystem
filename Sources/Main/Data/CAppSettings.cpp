#include <Data/CAppSettings.h>
#include <tools/loggers.h>
#include <boost/property_tree/xml_parser.hpp>

#define CONFIGURATION_FILE std::string("PhoneticNumberSystem.xml")
const std::string strConfigurationFileName("PhoneticNumberSystem.xml");

class CAppSettingsPrivate
{
public:
	CAppSettingsPrivate(CAppSettings * ptrPublic);
	~CAppSettingsPrivate();
	void loadSettings();
	void defaultValues();
public:
	CAppSettings *						m_ptrPublic;
};

CAppSettingsPrivate::CAppSettingsPrivate(CAppSettings * ptrPublic):m_ptrPublic(ptrPublic)
{
	int a=1;
	loadSettings();
}
CAppSettingsPrivate::~CAppSettingsPrivate(){}
void CAppSettingsPrivate::loadSettings()
{
	read_xml(CONFIGURATION_FILE, *(static_cast<boost::property_tree::ptree*>(m_ptrPublic)));
}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
CAppSettings::CAppSettings(void):m_ptrPriv(new CAppSettingsPrivate(this))
{
	int a=1;
}
CAppSettings::~CAppSettings(void){}
