#include <Tools/loggers.h>
#include <QtCore/QFile>
#include <QtCore/QDir>
#ifdef USE_LOG4QT

#include <log4qt/logmanager.h>
#include <log4qt/consoleappender.h>
#include <log4qt/fileappender.h>
#include <log4qt/logger.h>
#include <log4qt/ttcclayout.h>
#include <log4qt/patternlayout.h>
using namespace Log4Qt;

#else if USE_LOG4CPP
#include	<log4cpp/Category.hh>
#include	<log4cpp/FileAppender.hh>
#include	<log4cpp/Win32DebugAppender.hh>
#include	<log4cpp/PatternLayout.hh>

using namespace log4cpp;

#endif
bool bLoggersCreated(false);

#define DEBUG_LOGGER		"debug"
#define SLOTS_LOGGER		"slots"
#define NETWORK_LOGGER		"network"
#define GUI_LOGGER			"gui"
#define LOG_DIR				"logs"
#define LOG_FILE(X)		QString("%1%2%3.log").arg(LOG_DIR).arg(QDir::separator()).arg(X)

void createLoggers(const QString &strPluginLogName /*= QString()*/)
{
#ifdef USE_LOG4QT
	PatternLayout *		p_PatternLayout(NULL);
	p_PatternLayout = new PatternLayout;
	//need timestamp, Level, potential Network/debug/slots
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
#else if USE_LOG4CPP
	log4cpp::PatternLayout* layout1 = new log4cpp::PatternLayout();
	layout1->setConversionPattern("%d %p %c %x: %m%n");

	log4cpp::Appender* debugAppender = new log4cpp::FileAppender("DebugAppender", LOG_FILE(DEBUG_LOGGER).toStdString(),false);
	debugAppender->setLayout(layout1);

	log4cpp::Category &debugCategory = log4cpp::Category::getInstance(DEBUG_LOGGER);
	debugCategory.setAdditivity(false);
	debugCategory.setAppender(debugAppender);
	debugCategory.setPriority(log4cpp::Priority::INFO);

	//////////////////////////////////////////////////////////////////////////
	//log4cpp::PatternLayout* layout = new log4cpp::PatternLayout();
	//layout->setConversionPattern("%d %p %c %x: %m%n");
	log4cpp::Appender* GUICreationAppender = new log4cpp::FileAppender("GUICreationAppender", LOG_FILE(GUI_LOGGER).toStdString(),false);
	GUICreationAppender->setLayout(layout1);
	log4cpp::Category &GUICreationCategory = log4cpp::Category::getInstance(GUI_LOGGER);
	GUICreationCategory.setAdditivity(false);
	GUICreationCategory.setAppender(GUICreationAppender);
	GUICreationCategory.setPriority(log4cpp::Priority::INFO);

	//////////////////////////////////////////////////////////////////////////
	log4cpp::Appender* slotsConnectionAppender = new log4cpp::FileAppender(SLOTS_LOGGER, LOG_FILE(SLOTS_LOGGER).toStdString(),false);
	slotsConnectionAppender->setLayout(layout1);

	log4cpp::Category &slotsConnectionCategory = log4cpp::Category::getInstance(SLOTS_LOGGER);
	slotsConnectionCategory.setAdditivity(false);
	slotsConnectionCategory.setAppender(slotsConnectionAppender);
	slotsConnectionCategory.setPriority(log4cpp::Priority::INFO);
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

void printLog(eLogLevel debugLevel, eLoggerType loggerType, const QString &strMsg)
{
	if (false==bLoggersCreated)
		createLoggers();

#ifdef USE_LOG4QT
	Log4Qt::Logger * ptrLogger(NULL);
	Log4Qt::Logger * ptrLogger(NULL);
	Log4Qt::LogError  stLogError(strMsg);
#elif defined(USE_LOG4CPP)
	log4cpp::Category * ptrCategory(NULL);
#endif
	switch(loggerType)
	{
		case eDebug:	
#ifdef USE_LOG4QT
			ptrLogger = Log4Qt::LogManager::logger(DEBUG_LOGGER);			break;
#elif defined(USE_LOG4CPP)
			ptrCategory = log4cpp::Category::exists(DEBUG_LOGGER);
#endif
		case eSlots:	
#ifdef USE_LOG4QT
			ptrLogger = Log4Qt::LogManager::logger(SLOTS_LOGGER);	break;
#elif defined(USE_LOG4CPP)
			ptrCategory = log4cpp::Category::exists(SLOTS_LOGGER);
#endif
		default: return;
	}
	switch(debugLevel)
	{
		case eInfoLogLevel:
#ifdef USE_LOG4QT
			ptrLogger->info(stLogError); 	break;
#elif defined(USE_LOG4CPP)
			ptrCategory->info(strMsg.toStdString());
#endif
		case eWarningLogLevel:	
#ifdef USE_LOG4QT
			ptrLogger->warn(stLogError);	break;
#elif defined(USE_LOG4CPP)
			ptrCategory->warn(strMsg.toStdString());
#endif
		case eDebugLogLevel:	
#ifdef USE_LOG4QT
			ptrLogger->debug(stLogError);	break;
#elif defined(USE_LOG4CPP)
			ptrCategory->debug(strMsg.toStdString());
#endif
		case eErrorLogLevel:
#ifdef USE_LOG4QT
			ptrLogger->error(stLogError);	break;
#elif defined(USE_LOG4CPP)
			ptrCategory->error(strMsg.toStdString());
#endif
	}
}
//#else
//void createLoggers(const QString &strPluginLogName){};
//void destroyLoggers(){};
//void printLog(eLogLevel, eLoggerType, const QString &strMsg){};
//#endif

//////////////////////////////////////////////////////////////////////////
