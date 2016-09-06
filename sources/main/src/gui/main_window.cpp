#include <gui/main_window.h>
#include <gui/main_window_p.h>
#include <gui/settings/SettingsDlg.h>
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
