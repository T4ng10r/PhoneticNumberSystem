#include <Data/CDataThread.h>
#include <QtCore/QString>
#include <QtGui/QApplication>

#include <list>
#include <tools/loggers.h>

boost::shared_ptr<CDataThread> CDataThread::pInstance_;
enum { MaxRecentFiles = 5 };

class CDataThreadPrivate
{
public:
	CDataThreadPrivate(CDataThread * ptrPublic);
	~CDataThreadPrivate();
public:
	CDataThread *	m_ptrPublic;
};

CDataThreadPrivate::CDataThreadPrivate(CDataThread * ptrPublic):m_ptrPublic(ptrPublic)
{
	//QSettings stSettings("T4ng10r","ComputerConfigurationPriceChecker");
}
CDataThreadPrivate::~CDataThreadPrivate()
{
	printLog(eDebugLogLevel,eDebug,QString("Enter %1").arg(__FUNCTION__));
}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////// 
CDataThread::CDataThread(void):
m_ptrPriv(new CDataThreadPrivate(this))
{
}
boost::shared_ptr<CDataThread> CDataThread::getInstance()
{
	if(!pInstance_)
	{
		if(!pInstance_)
		{
			//CDataThread * p = ;
			pInstance_.reset(new CDataThread());
		}
	}
	return pInstance_;
}
CDataThread::~CDataThread(void)
{
	printLog(eDebugLogLevel,eDebug,QString("Enter %1").arg(__FUNCTION__));
	//pInstance_.reset();
	printLog(eDebugLogLevel,eDebug,QString("Leave %1").arg(__FUNCTION__));
}
void CDataThread::run()
{
	bool bResult = false;
	exec();
}
//////////////////////////////////////////////////////////////////////////
