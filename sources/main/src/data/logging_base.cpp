#include "logging_base.h"
#include <boost/format.hpp>

LoggingBase::LoggingBase()
    : logger(log4cplus::Logger::getInstance("DEBUG"))
    , slot_logger(log4cplus::Logger::getInstance("slots"))
{
}

void LoggingBase::logConnection(const QString& strClassName, const QString& strConnDesc, bool bResult)
{
    std::string strDebug =
        str(boost::format("%1%: Connection %2%") % strClassName.toStdString() % strConnDesc.toStdString());
    log4cplus::LogLevel level;

    if (bResult) {
        strDebug += " SUCCESS";
        level = log4cplus::DEBUG_LOG_LEVEL;
    } else {
        strDebug += " --FAIL--";
        level = log4cplus::WARN_LOG_LEVEL;
    }
    logger.log(level, strDebug);
    Q_ASSERT_X(bResult == true, strClassName.toLatin1(), strDebug.c_str());
}
