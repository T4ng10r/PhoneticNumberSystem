#ifndef _CAPPSETTINGS_INCLUDE_
#define _CAPPSETTINGS_INCLUDE_

#include <QtCore/QObject>
#include <Data/CSystemDigitsConfiguration.h>
#include <vector>
#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/property_tree/ptree.hpp>

class CAppSettingsPrivate;

class CSubstituteDigitsConfiguration
{
	CSubstituteDigitsConfiguration();
	std::map<std::string, CSingleSubstituteDigitsConfiguration>	substitutes;
};

// Set of consonants to use in substitution
// set of possible substitute configurations
class CSubstituteValuesConfiguration : public boost::property_tree::ptree
{
public:
	CSubstituteValuesConfiguration();
};

class CAppSettings : public QObject, public boost::property_tree::ptree
{
	Q_OBJECT
	friend class CAppSettingsPrivate;
public:
	static boost::shared_ptr<CAppSettings> getInstance();
	~CAppSettings();
	void saveSettings();
	const boost::property_tree::ptree & getSubstituteValuesConfiguration();
	const std::vector<CSingleSubstituteDigitsConfiguration> & getDigitsConfiguraions();
	std::string getCurrentDictPath();
public Q_SLOTS:
  void on_set_selected_consonant_system(const QString &);
protected:
	CAppSettings();
	boost::scoped_ptr<CAppSettingsPrivate> m_ptrPriv;
	static boost::shared_ptr<CAppSettings> pInstance_;
};
#define gAppSettings CAppSettings::getInstance() 
#endif //_CAPPSETTINGS_INCLUDE_

