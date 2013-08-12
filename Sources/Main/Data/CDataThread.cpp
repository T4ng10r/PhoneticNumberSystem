#include <Data/CDataThread.h>
#include <QString>
#include <QApplication>
#include <Data/CAppSettings.h>

#include <list>
#include <tools/loggers.h>
#include <hunspell/hunspell.hxx>

boost::shared_ptr<CDataThread> CDataThread::pInstance_;
enum { MaxRecentFiles = 5 };

class CDataThreadPrivate
{
public:
	CDataThreadPrivate(CDataThread * ptrPublic);
	~CDataThreadPrivate();
	bool checkCurrentAppDictionary();
public:
	CDataThread *	publicPart;
	boost::scoped_ptr<Hunspell>		hunspellDictionary;
};

CDataThreadPrivate::CDataThreadPrivate(CDataThread * ptrPublic):publicPart(ptrPublic)
{
}
CDataThreadPrivate::~CDataThreadPrivate()
{
}
bool CDataThreadPrivate::checkCurrentAppDictionary()
{
	std::string currentAppDictPath = gAppSettings->getCurrentDictPath();
	return false;
}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////// 
CDataThread::CDataThread(void):
privPart(new CDataThreadPrivate(this))
{
}
boost::shared_ptr<CDataThread> CDataThread::getInstance()
{
	if(!pInstance_)
	{
		if(!pInstance_)
		{
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
void CDataThread::loadCurrentAppDictionary()
{
	if (false==privPart->checkCurrentAppDictionary())
	{
		return;
	}
	
	privPart->hunspellDictionary;

}
//////////////////////////////////////////////////////////////////////////
