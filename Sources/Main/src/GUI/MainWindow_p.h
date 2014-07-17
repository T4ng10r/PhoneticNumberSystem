#ifndef _CAF_WINDOW_PRIVATE_INCLUDE_
#define _CAF_WINDOW_PRIVATE_INCLUDE_

#include <QBoxLayout>
#include <QAction>
#include <QToolBar>
#include <QMenuBar>
#include <QLabel>
#include <QTableView>
#include <QPushButton>
#include <QTabWidget>
#include <QTreeView>
#include <QSignalMapper>
#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <GUI/SearchPhoneticRepresentationsDlg.h>

enum { MaxRecentFiles = 5 };

class CMainWindow;
class CAppSettingsDlg;

class CMainWindowPrivate
{
public:
	CMainWindowPrivate(CMainWindow * ptrPublic);
	~CMainWindowPrivate();
public:
	void createConnectionsCatalog();
	void setupUI();
	void setupActions();
	void setConnections();
public:
	//wallaroo::Catalog			m_catalog;
	QWidget *					m_ptCentralWidget;
	QVBoxLayout *				mainLayout;
	QToolBar *					m_ptToolBar;

	//////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////
	QAction *					m_actionNewConf;
	QAction *					m_actionOpenConf;
	QAction *					m_actionSaveConf;
	QAction *					m_actionSaveAsConf;
	QAction *					m_actionConfiguration;
	//////////////////////////////////////////////////////////////////////////
	QMenu *						m_recentFilesMenu;
	QAction *					m_separatorAct;
	QAction *					m_aRecentFileActs[MaxRecentFiles];
	//////////////////////////////////////////////////////////////////////////
	QMutex						m_DataThreadTerminatedMutex;
	void *						ptrDataThread;
	CMainWindow *				m_ptrPublic;
	boost::shared_ptr<CAppSettingsDlg>	appSettingsDlg;
	boost::shared_ptr<CSearchPhoneticRepresentationsDlg>	searchPhoneticRepresentations;
};
#endif //_CAF_WINDOW_PRIVATE_INCLUDE_ 
