#include <qtTools.h>
#include <loggers.h>
#include <boost/format.hpp>

void logConnection(const QString & strClassName,const QString &strConnDesc,bool bResult)
{
	std::string strDebug = str(boost::format("%1%: Connection %2%") % strClassName.toStdString() %
		strConnDesc.toStdString());
	if (bResult)
	{
		strDebug+=" SUCCESS";	
		printLog(eSlots, eDebugLogLevel, strDebug);
	}
	else
	{
		strDebug+=" --FAIL--";
		printLog(eSlots, eWarningLogLevel, strDebug);
	}
	Q_ASSERT_X(bResult==true,strClassName.toLatin1(),strDebug.c_str());
}
