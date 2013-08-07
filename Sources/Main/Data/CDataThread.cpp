#include <Data/CDataThread.h>
#include <QString>
#include <QApplication>

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
}
void CDataThread::run()
{
	bool bResult = false;
	exec();
}
//////////////////////////////////////////////////////////////////////////
