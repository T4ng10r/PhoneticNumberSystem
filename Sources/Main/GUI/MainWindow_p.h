#ifndef _CAF_WINDOW_PRIVATE_INCLUDE_
#define _CAF_WINDOW_PRIVATE_INCLUDE_

#include <QtGui/QBoxLayout>
#include <QtGui/QAction>
#include <QtGui/QToolBar>
#include <QtGui/QMenuBar>
#include <QtGui/QLabel>
#include <QtGui/QTableView>
#include <QtGui/QPushButton>
#include <QtGui/QTabWidget>
#include <QtGui/QTreeView>
#include <QtCore/QSignalMapper>
#include <boost/scoped_ptr.hpp>
#include <wallaroo/catalog.h>

enum { MaxRecentFiles = 5 };

class CMainWindow;
class CSubstituteValuesConfigurationDlg;

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
	wallaroo::Catalog			m_catalog;
	QWidget *					m_ptCentralWidget;
	QVBoxLayout *				m_ptVLayout;
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
	boost::shared_ptr<CSubstituteValuesConfigurationDlg>	m_ptrSubstituteConfiguration;
};
#endif //_CAF_WINDOW_PRIVATE_INCLUDE_ 
