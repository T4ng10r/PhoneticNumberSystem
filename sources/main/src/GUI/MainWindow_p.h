#ifndef _CAF_WINDOW_PRIVATE_INCLUDE_
#define _CAF_WINDOW_PRIVATE_INCLUDE_

#include <GUI/search_phonetic_representation/search_phonetic_representations_dlg.h>
#include <QAction>
#include <QBoxLayout>
#include <QLabel>
#include <QMenuBar>
#include <QPushButton>
#include <QSignalMapper>
#include <QTabWidget>
#include <QTableView>
#include <QToolBar>
#include <QTreeView>
#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <data/logging_base.h>

enum { MaxRecentFiles = 5 };

class MainWindow;
class CSettingsDlg;

class MainWindowPrivate : public LoggingBase
{
  public:
    MainWindowPrivate(MainWindow* ptrPublic);
    ~MainWindowPrivate();

  public:
    void createConnectionsCatalog();
    void setupUI();
    void setupActions();
    void setConnections();

  public:
    QWidget*     central_widget_;

    //////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////
    QAction* action_new_configuration;
    QAction* action_open_configuration;
    QAction* action_save_configuration;
    QAction* action_save_configuration_as;
    QAction* action_open_settings;
    //////////////////////////////////////////////////////////////////////////
    QMenu*   recent_files_menu;
    //QAction* m_separatorAct;
    QAction* actions_recent_file[MaxRecentFiles];
    //////////////////////////////////////////////////////////////////////////
    MainWindow*                                         public_ptr;
    boost::shared_ptr<CSettingsDlg>                      settings_dialog;
    boost::shared_ptr<CSearchPhoneticRepresentationsDlg> search_phonetic_representations_dialog;
};
#endif //_CAF_WINDOW_PRIVATE_INCLUDE_
