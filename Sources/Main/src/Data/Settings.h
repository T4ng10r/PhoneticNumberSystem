#ifndef _SETTINGS_INCLUDE_
#define _SETTINGS_INCLUDE_

#include <QtCore/QObject>
#include <Data/CSystemDigitsConfiguration.h>
#include <vector>
#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/property_tree/ptree.hpp>

class SettingsPrivate;

// Set of consonants to use in substitution
// set of possible substitute configurations
class CSubstituteValuesConfiguration : public boost::property_tree::ptree
{
public:
	CSubstituteValuesConfiguration();
};

class Settings : public QObject, public boost::property_tree::ptree
{
	Q_OBJECT
	friend class SettingsPrivate;
public:
  typedef boost::shared_ptr<Settings> ptr;

	static ptr instance();
	~Settings();
	void saveSettings();
	const boost::property_tree::ptree & getSubstituteValuesConfiguration();
	const std::vector<CSingleSubstituteDigitsConfiguration> & getDigitsConfiguraions();
	//std::string getCurrentDictPath();
public Q_SLOTS:
  void on_set_selected_consonant_system(const QString &);
protected:
	Settings();
	boost::scoped_ptr<SettingsPrivate> pimpl;
	static ptr _instance;
};
#define gSettings Settings::instance() 
#endif //_SETTINGS_INCLUDE_

