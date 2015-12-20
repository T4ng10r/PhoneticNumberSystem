#include <Data/DataThread.h>
#include <GUI/MainWindow.h>
#include <GUI/MainWindow_p.h>
#include <GUI/Settings/AppSettingsDlg.h>
#include <QFileInfo>
#include <QStatusBar>
#include <QMenuBar>
#include <QtCore/QThread>
#include <Tools/loggers.h>
#include <Tools/qtTools.h>

const QSize ciSize(700,450);

CMainWindow::CMainWindow() : QMainWindow(),
m_ptrPriv(new CMainWindowPrivate(this))
{
	resize(ciSize);
}
CMainWindow::~CMainWindow()
{

}
void CMainWindow::closeEvent(QCloseEvent *)
{
	gDataThread->thread()->quit();
}
void CMainWindow::onShowAppSettingsConfigureDialog(bool)
{
	m_ptrPriv->appSettingsDlg->show();
}
