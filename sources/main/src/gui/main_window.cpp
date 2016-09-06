#include <GUI/MainWindow.h>
#include <GUI/MainWindow_p.h>
#include <GUI/Settings/SettingsDlg.h>
#include <QFileInfo>
#include <QMenuBar>
#include <QStatusBar>
#include <QtCore/QThread>
#include <data/data_thread.h>
#include <tools/loggers.h>
#include <tools/qtTools.h>

const QSize ciSize(700, 450);

MainWindow::MainWindow()
    : QMainWindow()
    , _pimpl(new MainWindowPrivate(this))
{
  resize(ciSize);
}
MainWindow::~MainWindow() {}
void MainWindow::closeEvent(QCloseEvent*) { gDataThread->thread()->quit(); }
void MainWindow::onShowSettingsConfigureDialog(bool) { _pimpl->settings_dialog->show(); }
