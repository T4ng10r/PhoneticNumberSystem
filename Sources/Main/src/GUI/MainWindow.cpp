#include <data/DataThread.h>
#include <GUI/MainWindow.h>
#include <GUI/MainWindow_p.h>
#include <GUI/Settings/SettingsDlg.h>
#include <QFileInfo>
#include <QStatusBar>
#include <QMenuBar>
#include <QtCore/QThread>
#include <tools/loggers.h>
#include <tools/qtTools.h>

const QSize ciSize(700,450);

CMainWindow::CMainWindow() : QMainWindow(),
_pimpl(new MainWindowPrivate(this))
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
void CMainWindow::onShowSettingsConfigureDialog(bool)
{
	_pimpl->settings_dialog->show();
}
