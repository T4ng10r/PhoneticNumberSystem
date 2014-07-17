#include <Tools/loggers.h>
//#include <io.h>
#include <boost/filesystem.hpp>
#include <boost/format.hpp>
#ifdef USE_LOG4QT

#include <log4qt/logmanager.h>
#include <log4qt/consoleappender.h>
#include <log4qt/fileappender.h>
#include <log4qt/logger.h>
#include <log4qt/ttcclayout.h>
#include <log4qt/patternlayout.h>
using namespace Log4Qt;

#elif USE_LOG4CPP
#include	<log4cpp/Category.hh>
#include	<log4cpp/FileAppender.hh>
#include	<log4cpp/PatternLayout.hh>
#include	<log4cpp/Category.hh>
#include	<log4cpp/PropertyConfigurator.hh>

using namespace log4cpp;

#endif
bool bLoggersCreated(false);

#ifdef WIN32
#define SEPARATOR "/"
#else
#define SEPARATOR "\\"
#endif

#define DEBUG_LOGGER		"debug"
#define SLOTS_LOGGER		"slots"
#define GUI_LOGGER			"gui"
#define LOG_DIR				"logs"
#define LOG_FILE(X)		str(boost::format("%1%%2%%3%.log") % LOG_DIR % SEPARATOR % X)

void deleteFileIfExist(const std::string & filePath)
{
	struct stat   buffer;
	if (stat(filePath.c_str(), &buffer) == 0)
		remove(filePath.c_str());
}
void cleanupLogsDir()
{
	//be sure that DIR is created
	boost::filesystem::create_directory(LOG_DIR);
	//cleanup logs dir content
	deleteFileIfExist(LOG_FILE(DEBUG_LOGGER));
	deleteFileIfExist(LOG_FILE(GUI_LOGGER));
	deleteFileIfExist(LOG_FILE(SLOTS_LOGGER));
}
void createLoggers(const std::string &strPluginLogName)
{
	cleanupLogsDir();

#ifdef USE_LOG4QT
	PatternLayout *		p_PatternLayout(NULL);
	p_PatternLayout = new PatternLayout;
	//need timestamp, Level, potential Network/debug/Q_SLOTS
	p_PatternLayout->setConversionPattern("%-8r %-5p %c %m%n");
	p_PatternLayout->activateOptions();

	// Create an appender
	if (strPluginLogName.isEmpty())
	{
		ConsoleAppender * p_ConsoleAppender = new ConsoleAppender(p_PatternLayout, ConsoleAppender::STDOUT_TARGET);
		p_ConsoleAppender->setName(QLatin1String("Console Appender"));
		p_ConsoleAppender->activateOptions();
		FileAppender * p_FileDebugAppender = new FileAppender(p_PatternLayout, LOG_FILE(DEBUG_LOGGER));
		p_FileDebugAppender->setName(QLatin1String("Debug Appender"));
		p_FileDebugAppender->activateOptions();
		//////////////////////////////////////////////////////////////////////////
		FileAppender * p_FileSlotConnAppender = new FileAppender(p_PatternLayout, LOG_FILE(SLOTS_LOGGER));
		p_FileSlotConnAppender->setName(QLatin1String("Slots Appender"));
		p_FileSlotConnAppender->activateOptions();
		//////////////////////////////////////////////////////////////////////////
		LogManager::logger(DEBUG_LOGGER)->addAppender(p_ConsoleAppender);
		LogManager::logger(DEBUG_LOGGER)->addAppender(p_FileDebugAppender);
		LogManager::logger(SLOTS_LOGGER)->addAppender(p_FileSlotConnAppender);
	}
	else
	{
		FileAppender * p_FileNetworkAppender = new FileAppender(p_PatternLayout, LOG_FILE(strPluginLogName));
		p_FileNetworkAppender->setName(QLatin1String("Debug Appender"));
		p_FileNetworkAppender->activateOptions();
		//////////////////////////////////////////////////////////////////////////
		LogManager::logger(DEBUG_LOGGER)->addAppender(p_FileNetworkAppender);
	}
#elif USE_LOG4CPP
	std::string initFileName = "logs/log4cpp.properties";
		log4cpp::PropertyConfigurator::configure(initFileName);
#endif
	bLoggersCreated=true;
}

void destroyLoggers()
{
#ifdef USE_LOG4QT
	QList<Appender*> lApplist = LogManager::logger(DEBUG_LOGGER)->appenders();
	LogManager::logger("debug")->removeAllAppenders();

	lApplist = LogManager::logger(SLOTS_LOGGER)->appenders();
	LogManager::logger("slotsConnection")->removeAllAppenders();
#elif defined(USE_LOG4CPP)
#endif
}

boost::optional<std::string> get_logger_name(eLoggerType loggerType)
{
	boost::optional<std::string> logger_name;
	switch(loggerType)
	{
		case eDebug:	return logger_name = DEBUG_LOGGER; break;
		case eSlots:	return logger_name = SLOTS_LOGGER;	break;
		case eGUI:		return logger_name = GUI_LOGGER;	break;
	}
	return logger_name;
}

#ifdef USE_LOG4QT
void printLog_log4qt(boost::optional<std::string> logger_name, eLogLevel debugLevel, const std::string &strMsg)
{
	Log4Qt::LogError  stLogError(strMsg);
	Log4Qt::Logger * ptrLogger(Log4Qt::LogManager::logger(logger_name));
	switch(debugLevel)
	{
		case eInfoLogLevel:
			ptrLogger->info(stLogError); 	break;
		case eWarningLogLevel:	
			ptrLogger->warn(stLogError);	break;
		case eDebugLogLevel:	
			ptrLogger->debug(stLogError);	break;
		case eErrorLogLevel:
			ptrLogger->error(stLogError);	break;
	}
}
#endif

#ifdef USE_LOG4CPP
void printLog_log4cpp(boost::optional<std::string> logger_name, eLogLevel debugLevel, const std::string &strMsg)
{
	log4cpp::Category * ptrCategory(NULL);
	switch(debugLevel)
	{
		case eInfoLogLevel:
			ptrCategory->info(strMsg);	break;
		case eWarningLogLevel:	
			ptrCategory->warn(strMsg);	break;
		case eDebugLogLevel:	
			ptrCategory->debug(strMsg);	break;
		case eErrorLogLevel:
			ptrCategory->error(strMsg);	break;
	}
}
#endif

void printLog(eLoggerType loggerType, eLogLevel debugLevel, const std::string &strMsg)
{
	if (false==bLoggersCreated)
		createLoggers();

	boost::optional<std::string> logger_name = get_logger_name(loggerType);
	if (!!logger_name)
		return;

#ifdef USE_LOG4QT
	printLog_log4qt(logger_name, debugLevel, strMsg);
#elif defined(USE_LOG4CPP)
	printLog_log4cpp(logger_name, debugLevel, strMsg);
#endif

}

//////////////////////////////////////////////////////////////////////////
