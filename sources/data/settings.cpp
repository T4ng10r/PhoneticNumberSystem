#include <boost/filesystem.hpp>
#include <boost/foreach.hpp>
#include <boost/format.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <data/CSettingsKeywords.h>
#include <data/logging_base.h>
#include <data/settings.h>
#include <iostream>
#include <sstream>
#include <loggers.h>

#define CONFIGURATION_FILE std::string("PhoneticNumberSystem.xml")
const std::string strConfigurationFileName("PhoneticNumberSystem.xml");

Settings::ptr Settings::_instance;

class SettingsPrivate : public LoggingBase
{
  public:
    SettingsPrivate(Settings* ptrPublic);
    ~SettingsPrivate();
    void loadSettings();
    void defaultValues();
    void getDigitsConfiguration();
    void saveSettings();

  public:
    // boost::shared_ptr<CSubstituteValuesConfiguration>	m_ptrSubstValConf;
    Settings*                                         _ppub;
    boost::property_tree::ptree                       m_ptrSubstValConf;
    std::vector<CSingleSubstituteDigitsConfiguration> m_vDigitsConfiguration;
};
//////////////////////////////////////////////////////////////////////////
SettingsPrivate::SettingsPrivate(Settings* ptrPublic)
    : _ppub(ptrPublic)
{
    loadSettings();
    getDigitsConfiguration();
    defaultValues();
}
SettingsPrivate::~SettingsPrivate() {}
void SettingsPrivate::saveSettings()
{
#if (BOOST_VERSION <= 105500)
    boost::property_tree::xml_writer_settings<char> settings;
#else
    boost::property_tree::xml_writer_settings<std::string> settings;
#endif
    ("\t", 1);
    write_xml(CONFIGURATION_FILE, *(static_cast<boost::property_tree::ptree*>(_ppub)),
              std::locale() /*, settings*/);
}

void SettingsPrivate::loadSettings()
{
    logger.log(log4cplus::INFO_LOG_LEVEL, "Loading properties file (" + CONFIGURATION_FILE + ")");
    using boost::property_tree::ptree;

    try {
        std::basic_ifstream<char>    stream(CONFIGURATION_FILE);
        boost::property_tree::ptree& tree = *_ppub;
        read_xml(stream, tree);
    } catch (boost::property_tree::xml_parser::xml_parser_error const& /*ex*/) {
        logger.log(log4cplus::ERROR_LOG_LEVEL, "Lack of properties file");
    }
    logger.log(log4cplus::INFO_LOG_LEVEL, "Loading properties file finished");
    m_ptrSubstValConf = _ppub->get_child(CONSONANTS_SETTINGS);
    logger.log(log4cplus::DEBUG_LOG_LEVEL, "Settings: loading settings from file finished");
}

void SettingsPrivate::getDigitsConfiguration()
{
    using boost::property_tree::ptree;
    CSingleSubstituteDigitsConfiguration stSystemDigitsConfiguration;
    BOOST_FOREACH (const ptree::value_type& digitsConf, m_ptrSubstValConf) {
        if (digitsConf.first != "digits_configuration")
            continue;
        stSystemDigitsConfiguration.reset();
        BOOST_FOREACH (const ptree::value_type& digitsConfEntry, digitsConf.second) {
            if (digitsConfEntry.first == "name") {
                stSystemDigitsConfiguration.strName = digitsConfEntry.second.data();
                continue;
            }
            if (digitsConfEntry.first == "digit") {
                int iDigit(-1);
                std::pair<char, char> stPair(' ', ' ');
                BOOST_FOREACH (const ptree::value_type& digitsEntry, digitsConfEntry.second) {
                    if (digitsEntry.first == "value") {
                        iDigit = digitsEntry.second.get_value<int>();
                    }
                    if (digitsEntry.first == "substitue") {
                        if (stPair.first == ' ') {
                            stPair.first = digitsEntry.second.data().at(0);
                            stSystemDigitsConfiguration.allConsonants.push_back(stPair.first);
                        } else if (stPair.second == ' ') {
                            stPair.second = digitsEntry.second.data().at(0);
                            stSystemDigitsConfiguration.allConsonants.push_back(stPair.second);
                        }
                    }
                }
                stSystemDigitsConfiguration.mSystem[iDigit] = stPair;
                continue;
            }
        }
        stSystemDigitsConfiguration.createConsonantsDigitsMap();
        m_vDigitsConfiguration.push_back(stSystemDigitsConfiguration);
    }
}
void SettingsPrivate::defaultValues()
{
    try {
        _ppub->get<std::string>(DICTIONARIES_DIRECTORY);
    } catch (boost::property_tree::ptree_bad_path& /*e*/) {
      logger.log(log4cplus::DEBUG_LOG_LEVEL,
                 str(boost::format("Lack of '%1% - adding default value") % DICTIONARIES_DIRECTORY));
        _ppub->put<std::string>(DICTIONARIES_DIRECTORY, DICTIONARIES_DIRECTORY_DEFAULT_VALUE);
    }
}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
Settings::Settings()
    : _pimpl(new SettingsPrivate(this))
{
  _pimpl->logger.log(log4cplus::DEBUG_LOG_LEVEL, "Settings created");
}
Settings::~Settings() {}
const boost::property_tree::ptree& Settings::getSubstituteValuesConfiguration()
{
  _pimpl->logger.log(log4cplus::DEBUG_LOG_LEVEL, "SubstituteValuesConfiguration provided");
    return _pimpl->m_ptrSubstValConf;
}
Settings::ptr Settings::instance()
{
    if (!_instance) {
        if (!_instance) {
            _instance.reset(new Settings());
        }
    }
    return _instance;
}
void                                                     Settings::saveSettings() { _pimpl->saveSettings(); }
const std::vector<CSingleSubstituteDigitsConfiguration>& Settings::getDigitsConfiguraions()
{
    return _pimpl->m_vDigitsConfiguration;
}
// std::string Settings::getCurrentDictPath()
//{
//	std::string strPath;
//	return strPath;
//}
void Settings::on_set_selected_consonant_system(const QString&) {}
//////////////////////////////////////////////////////////////////////////
CSubstituteValuesConfiguration::CSubstituteValuesConfiguration()
    : boost::property_tree::ptree(gSettings->getSubstituteValuesConfiguration())
{
}
