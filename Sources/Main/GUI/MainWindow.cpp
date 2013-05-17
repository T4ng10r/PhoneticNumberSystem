#include <Data/CDataThread.h>
#include <Data/ComputerConfModel.h>
#include <GUI/MainWindow.h>
#include <GUI/MainWindow_p.h>
#include <QtCore/QFileInfo>
#include <QtGui/QStatusBar>
#include <QtGui/QMenuBar>
#include <Tools/loggers.h>
#include <Tools/qtTools.h>

const QSize ciSize(700,450);

CMainWindow::CMainWindow() : QMainWindow(),
m_ptrPriv(new CMainWindowPrivate(this))
{
}
CMainWindow::~CMainWindow(){}
void CMainWindow::closeEvent(QCloseEvent *)
{
	//m_DataThreadTerminatedMutex.lock();
	//bool bResult = connect(CDataThread::getInstance(), SIGNAL(terminated()) ,
	//	this, SLOT(onWaitingForDataThreadTermination()));
	//bResult = connect(CDataThread::getInstance(), SIGNAL(finished()) ,
	//	this, SLOT(onWaitingForDataThreadTermination()));
	//ptrDataThread = CDataThread::getInstance();
	CDataThread::getInstance()->quit();
}
