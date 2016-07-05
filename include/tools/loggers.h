#ifndef _LOGGERS_INCLUDE_
#define _LOGGERS_INCLUDE_
#include <string>

enum eLogLevel
{
	eInfoLogLevel=0,
	eWarningLogLevel,
	eDebugLogLevel,
	eErrorLogLevel
};

enum eLoggerType
{
	eDebug=0,
	eGUI,
	eSlots
};
void create_loggers(const std::string &strPluginLogName = std::string());
void destroyLoggers();
void printLog(eLoggerType loggerType, eLogLevel debugLevel, const std::string &strMsg);
#endif //_LOGGERS_INCLUDE_
