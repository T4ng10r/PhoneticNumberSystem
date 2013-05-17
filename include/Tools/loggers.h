#ifndef _LOGGERS_INCLUDE_
#define _LOGGERS_INCLUDE_
#include <QtCore/QString>

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
	eSlots,
	eNetwork
};
void createLoggers(const QString &strPluginLogName = QString());
void destroyLoggers();
void printLog(eLogLevel, eLoggerType, const QString &strMsg);
#endif //_LOGGERS_INCLUDE_