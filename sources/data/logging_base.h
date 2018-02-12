#pragma once
#include <log4cplus/logger.h>
#include <QtCore/QString>

class LoggingBase
{
  public:
    LoggingBase();
    void logConnection(const QString & strClassName, const QString &strConnDesc, bool bResult);

  public:
    log4cplus::Logger logger;
    log4cplus::Logger slot_logger;
};

