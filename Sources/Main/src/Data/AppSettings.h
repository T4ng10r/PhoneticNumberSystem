#ifndef _APPSETTINGS_INCLUDE_
#define _APPSETTINGS_INCLUDE_

#include <QtCore/QObject>
#include <Data/CSystemDigitsConfiguration.h>
#include <vector>
#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/property_tree/ptree.hpp>

class AppSettingsPrivate;

// Set of consonants to use in substitution
// set of possible substitute configurations
class CSubstituteValuesConfiguration : public boost::property_tree::ptree
{
public:
	CSubstituteValuesConfiguration();
};

class AppSettings : public QObject, public boost::property_tree::ptree
{
	Q_OBJECT
	friend class AppSettingsPrivate;
public:
  typedef boost::shared_ptr<AppSettings> ptr;

	static ptr instance();
	~AppSettings();
	void saveSettings();
	const boost::property_tree::ptree & getSubstituteValuesConfiguration();
	const std::vector<CSingleSubstituteDigitsConfiguration> & getDigitsConfiguraions();
	//std::string getCurrentDictPath();
public Q_SLOTS:
  void on_set_selected_consonant_system(const QString &);
protected:
	AppSettings();
	boost::scoped_ptr<AppSettingsPrivate> pimpl;
	static ptr _instance;
};
#define gAppSettings AppSettings::instance() 
#endif //_APPSETTINGS_INCLUDE_

