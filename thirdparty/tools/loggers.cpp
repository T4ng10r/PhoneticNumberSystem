#include <loggers.h>
#include <boost/filesystem.hpp>
#include <boost/format.hpp>
#include <iostream>
#include <log4cplus/configurator.h>
using namespace log4cplus;
bool bLoggersCreated(false);

#ifdef WIN32
#define SEPARATOR "/"
#else
#define SEPARATOR "\\"
#endif

#define LOG_DIR "logs"
#define DEBUG_LOGGER "debug"
#define SLOTS_LOGGER "slots"
#define GUI_LOGGER "gui"
#define LOG_FILE(X) str(boost::format("%1%%2%%3%.log") % LOG_DIR % SEPARATOR % X)
#define LOG4CPLUS_PROPERTIES_FILE LOG_DIR "/log4cplus.properties"

void deleteFileIfExist(const std::string& filePath)
{
  if (boost::filesystem::exists(filePath.c_str()))
    remove(filePath.c_str());
}
void cleanupLogsDir()
{
  // be sure that DIR is created
  boost::filesystem::create_directory(LOG_DIR);
  // cleanup logs dir content
  deleteFileIfExist(LOG_FILE(DEBUG_LOGGER));
  deleteFileIfExist(LOG_FILE(GUI_LOGGER));
  deleteFileIfExist(LOG_FILE(SLOTS_LOGGER));
}

void create_loggers(const std::string& strPluginLogName)
{
  cleanupLogsDir();
  if (!boost::filesystem::exists(LOG4CPLUS_PROPERTIES_FILE)) {
    std::cout << "Not existing log4cpp properties file" << std::endl;
  }
  log4cplus::PropertyConfigurator::doConfigure(LOG4CPLUS_PROPERTIES_FILE);
  bLoggersCreated = true;
  //
  printLog(eDebug, eInfoLogLevel, "Log created");
  printLog(eGUI, eInfoLogLevel, "Log created");
  printLog(eSlots, eInfoLogLevel, "Log created");
}

boost::optional<std::string> get_logger_name(eLoggerType loggerType)
{
  boost::optional<std::string> logger_name;
  switch (loggerType) {
  case eDebug:
    return logger_name = DEBUG_LOGGER;
    break;
  case eSlots:
    return logger_name = SLOTS_LOGGER;
    break;
  case eGUI:
    return logger_name = GUI_LOGGER;
    break;
  }
  return logger_name;
}

#ifdef USE_LOG4CPP
void printLog_log4cpp(boost::optional<std::string> logger_name, eLogLevel debugLevel, const std::string& strMsg)
{
  log4cpp::Category* ptrCategory(NULL);
  ptrCategory = log4cpp::Category::exists(logger_name.get());
  switch (debugLevel) {
  case eInfoLogLevel:
    ptrCategory->info(strMsg);
    break;
  case eWarningLogLevel:
    ptrCategory->warn(strMsg);
    break;
  case eDebugLogLevel:
    ptrCategory->debug(strMsg);
    break;
  case eErrorLogLevel:
    ptrCategory->error(strMsg);
    break;
  }
}
#endif

void printLog(eLoggerType loggerType, eLogLevel debugLevel, const std::string& strMsg)
{
  if (false == bLoggersCreated)
    create_loggers();

  boost::optional<std::string> logger_name = get_logger_name(loggerType);
  if (!logger_name)
    return;

#if defined(USE_LOG4CPP)
  printLog_log4cpp(logger_name, debugLevel, strMsg);
#endif
}

//////////////////////////////////////////////////////////////////////////
