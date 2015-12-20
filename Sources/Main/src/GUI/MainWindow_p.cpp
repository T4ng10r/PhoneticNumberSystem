#include <GUI/MainWindow.h>
#include <GUI/MainWindow_p.h>
#include <GUI/Settings/SettingsDlg.h>
#include <Data/DataThread.h>
#include <QStatusBar>
#include <QMenuBar>
#include <QFileInfo>
#include <Tools/loggers.h>
#include <Tools/qtTools.h>
#include <Data/Settings.h>
#include <boost/scoped_ptr.hpp>

const QSize ciSize(700,450);

CMainWindowPrivate::CMainWindowPrivate(CMainWindow * ptrPublic):m_ptrPublic(ptrPublic)
{
	setupUI();
	setupActions();
	gDataThread;
	setConnections();
	appSettingsDlg->performInitialUpdateAfterAllChildrenUpdate();
	m_ptrPublic->statusBar()->showMessage("Ready");
}
CMainWindowPrivate::~CMainWindowPrivate()
{}
void CMainWindowPrivate::setupUI()
{
	m_ptCentralWidget = new QWidget;
	m_ptrPublic->setCentralWidget(m_ptCentralWidget);

	mainLayout = new QVBoxLayout;
	delete m_ptCentralWidget->layout();
	m_ptCentralWidget->setLayout(mainLayout);
	appSettingsDlg.reset(new CSettingsDlg);

	searchPhoneticRepresentations.reset(new CSearchPhoneticRepresentationsDlg);
	mainLayout->addWidget(searchPhoneticRepresentations.get());
}
void CMainWindowPrivate::setConnections()
{
	bool bResult = false;
	bResult = QObject::connect(m_actionConfiguration, SIGNAL(triggered(bool)), 
		m_ptrPublic, SLOT(onShowSettingsConfigureDialog(bool)));
	logConnection("CMainWindowPrivate::setConnections",
		"'CMainWindowPrivate::triggered' with 'CMainWindow::onActionTrigger'", 
		bResult);

	bResult = QObject::connect(searchPhoneticRepresentations.get(), SIGNAL(performSearch(const std::string & )),
		gDataThread.get(), SLOT(onNumberSearchStarted(const std::string & )) );
	logConnection("CMainWindowPrivate::setConnections",
		"'DataThread::searchProgess' with 'searchPhoneticRepresentations::onSearchProgess'", 
		bResult);

	bResult = QObject::connect(gDataThread.get(), SIGNAL(searchProgress(int, int)), 
		searchPhoneticRepresentations.get(), SLOT(onSearchProgress(int, int )));
	logConnection("CMainWindowPrivate::setConnections",
		"'DataThread::searchProgess' with 'searchPhoneticRepresentations::onSearchProgess'", 
		bResult);

	bResult = QObject::connect(gDataThread.get(), SIGNAL(searchFinished(bool)),
		searchPhoneticRepresentations.get(), SLOT(searchFinished(bool)) );
	logConnection("CMainWindowPrivate::setConnections",
		"'DataThread::searchFinished' with 'searchPhoneticRepresentations::searchFinished'", 
		bResult);

	bResult = QObject::connect(appSettingsDlg.get(), SIGNAL(dictionarySelected()),
		searchPhoneticRepresentations.get(), SLOT(disableSearchButton()) );
	logConnection("CMainWindowPrivate::setConnections",
		"'appSettingsDlg::dictionarySelected' with 'searchPhoneticRepresentations::disableSearchButton'", 
		bResult);

	bResult = QObject::connect(gDataThread.get(), SIGNAL(dictionaryLoaded(bool)),
		searchPhoneticRepresentations.get(), SLOT(enableSearchButton()) );
	logConnection("CMainWindowPrivate::setConnections",
		"'DataThread::dictionaryLoaded' with 'searchPhoneticRepresentations::enableSearchButton'", 
		bResult);
}
void CMainWindowPrivate::setupActions()
{
	m_actionNewConf = m_ptrPublic->menuBar()->addAction("New");
	m_actionNewConf->setShortcut(QKeySequence::New);
	m_actionNewConf->setToolTip("");
	m_ptrPublic->menuBar()->addSeparator();
	////////////////////////////////////////////////////////////////////////////
	m_recentFilesMenu = m_ptrPublic->menuBar()->addMenu("Open");
	m_actionOpenConf = m_recentFilesMenu->addAction("Open");
	m_actionOpenConf->setShortcut(QKeySequence::Open);
	m_actionOpenConf->setToolTip("");
	m_recentFilesMenu->addSeparator();
	////////////////////////////////////////////////////////////////////////////
	for (int i = 0; i < MaxRecentFiles; ++i) {	
		m_aRecentFileActs[i] = m_recentFilesMenu->addAction("");
		m_aRecentFileActs[i]->setVisible(false);
		//m_ptrPublic->connect(m_aRecentFileActs[i], SIGNAL(triggered()),
		//	this, SLOT(onOpenRecentFile()));
	}
	//////////////////////////////////////////////////////////////////////////
	m_actionSaveConf = m_ptrPublic->menuBar()->addAction("Save");
	m_actionSaveConf->setShortcut(QKeySequence::Save);
	m_actionSaveConf->setToolTip("");
	////////////////////////////////////////////////////////////////////////////
	m_actionSaveAsConf = m_ptrPublic->menuBar()->addAction("Save as");
	m_actionSaveAsConf->setShortcut(QKeySequence::SaveAs);
	m_actionSaveAsConf->setToolTip("");
	////////////////////////////////////////////////////////////////////////////
	m_ptrPublic->menuBar()->addSeparator();
	m_actionConfiguration= m_ptrPublic->menuBar()->addAction("Configuration");
	m_actionConfiguration->setToolTip("");
}
