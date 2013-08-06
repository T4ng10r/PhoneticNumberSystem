#ifndef _CAPPSETTINGS_INCLUDE_
#define _CAPPSETTINGS_INCLUDE_

//#include <QtCore/QObject>
#include <Data/CSystemDigitsConfiguration.h>
#include <vector>
#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/property_tree/ptree.hpp>
#define SELECTED_CONSONANTS_SYSTEM "settings.selected_consonants_system"

class CAppSettingsPrivate;

class CSubstituteDigitsConfiguration
{
	CSubstituteDigitsConfiguration();
	std::map<std::string, CSingleSubstituteDigitsConfiguration>	substitutes;
};

class CSubstituteValuesConfiguration : public boost::property_tree::ptree
{
public:
	CSubstituteValuesConfiguration();
};

class CAppSettings : public boost::property_tree::ptree //QObject
{
	//Q_OBJECT
	friend class CAppSettingsPrivate;
public:
	static CAppSettings* getInstance();
	~CAppSettings();
	const boost::property_tree::ptree & getSubstituteValuesConfiguration();
	const std::vector<CSingleSubstituteDigitsConfiguration> & getDigitsConfiguraions();
protected:
	CAppSettings();
	boost::scoped_ptr<CAppSettingsPrivate> m_ptrPriv;
	static CAppSettings* pInstance_;
};
#define gAppSettings CAppSettings::getInstance() 
#endif //_CAPPSETTINGS_INCLUDE_

