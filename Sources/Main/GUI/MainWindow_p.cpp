#include <GUI/MainWindow.h>
#include <GUI/MainWindow_p.h>
#include <GUI/Settings/SubstituteValuesConfigurationDlg.h>
#include <Data/CDataThread.h>
#include <QStatusBar>
#include <QMenuBar>
#include <QFileInfo>
#include <Tools/loggers.h>
#include <Tools/qtTools.h>
#include <Data/CAppSettings.h>

const QSize ciSize(700,450);

CMainWindowPrivate::CMainWindowPrivate(CMainWindow * ptrPublic):m_ptrPublic(ptrPublic)
{
	setupUI();
	setupActions();
	CDataThread::getInstance()->start();
	setConnections();
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
	CSubstituteValuesConfiguration  stSubstituteValuesConfiguration;
	m_ptrSubstituteConfiguration.reset(new CSubstituteValuesConfigurationDlg(stSubstituteValuesConfiguration));

	searchPhoneticRepresentations.reset(new CSearchPhoneticRepresentationsDlg);
	mainLayout->addWidget(searchPhoneticRepresentations.get());
/*	//m_ptToolBar = new QToolBar(this);
	//m_ptToolBar->setObjectName("m_ptToolBar");
	//m_ptToolBar->setMaximumHeight(25);

	//m_ptVLayout->addWidget(m_ptToolBar);
	//////////////////////////////////
	m_ptCompConfView = new FrozenTableView(this);
	m_ptCompConfView->setObjectName("CompConfView");

	m_ptVLayout->addWidget(m_ptCompConfView);
	//////////////////////////////////////////////////////////////////////////
	m_ptHLayout = new QHBoxLayout;

	m_ptrAddComp = new QPushButton(this);
	m_ptrAddComp->setIcon(QIcon(":/images/plus.png"));
	m_ptrAddComp->setText("Item");

	m_ptrAddShop = new QPushButton(this);
	m_ptrAddShop->setIcon(QIcon(":/images/plus.png"));
	m_ptrAddShop->setText("Shop");

	m_ptHLayout->addStretch(10);
	m_ptHLayout->addWidget(m_ptrAddComp);
	m_ptHLayout->addWidget(m_ptrAddShop);
	//////////////////////////////////////////////////////////////////////////
	m_ptVLayout->addLayout(m_ptHLayout);
*/

	//m_ptrPublic->show();
}
void CMainWindowPrivate::setConnections()
{
	bool bResult = false;
	bResult = QObject::connect(m_actionConfiguration, SIGNAL(triggered(bool)), 
		m_ptrPublic, SLOT(onActionTrigger(bool)));
	logConnection("CMainWindowPrivate::setConnections",
		"'CMainWindowPrivate::triggered' with 'CMainWindow::onActionTrigger'", 
		bResult);
/*	bool bResult = false;
	m_ptrStatusBarSignalMapper = new QSignalMapper;
	m_ptrStatusBarSignalMapper->setMapping(m_actionNewConf,QString("New configuration"));
	m_ptrStatusBarSignalMapper->setMapping(m_actionOpenConf,QString("Opening configuration from file"));
	m_ptrStatusBarSignalMapper->setMapping(m_actionSaveConf,QString("Saving configuration to file"));
	bResult = connect(m_actionNewConf, SIGNAL(triggered(bool)) , m_ptrStatusBarSignalMapper, SLOT(map()));
	bResult = connect(m_actionOpenConf, SIGNAL(triggered(bool)) , m_ptrStatusBarSignalMapper, SLOT(map()));
	bResult = connect(m_actionSaveConf, SIGNAL(triggered(bool)) , m_ptrStatusBarSignalMapper, SLOT(map()));
	bResult = connect(m_ptrStatusBarSignalMapper, SIGNAL(mapped(const QString &)), this, SLOT(onStatusBarMsgChange(const QString &)));


	bResult = connect(m_actionNewConf, SIGNAL(triggered(bool)), CDataThread::getInstance(), SLOT(onNewConf()));
	logConnection("MainWindow::setConnections","'m_actionNewConf::triggered' with 'CDataThread::onNewConf'", bResult);
	bResult = connect(CDataThread::getInstance(), SIGNAL(refreshingFinished()), this, SLOT(onRefreshingFinished()));
	logConnection("MainWindow::setConnections","'CDataThread::getInstance::refreshingFinished' with 'this::onRefreshingFinished'", bResult);
	/////
	bResult = connect(m_actionOpenConf, SIGNAL(triggered(bool)), CDataThread::getInstance(), SLOT(onOpenConf()));
	logConnection("MainWindow::setConnections","'m_actionOpenConf::toggled' with 'CDataThread::onOpenConf'", bResult);
	bResult = connect(m_actionSaveConf, SIGNAL(triggered(bool)), CDataThread::getInstance(), SLOT(onSaveConf()));
	logConnection("MainWindow::setConnections","'m_actionSaveConf::toggled' with 'CDataThread::onSaveConf'", bResult);
	bResult = connect(m_actionSaveAsConf, SIGNAL(triggered(bool)), CDataThread::getInstance(), SLOT(onSaveAsConf()));
	logConnection("MainWindow::setConnections","'m_actionSaveAsConf::toggled' with 'CDataThread::onSaveAsConf'", bResult);
	bResult = connect(m_actionProxySettings, SIGNAL(triggered(bool)) ,this, SLOT(onProxySettings(bool)));
	logConnection("MainWindow::setConnections","'m_actionProxySettings::toggled' with 'this::onProxySettings'", bResult);
	//////////////////////////////////////////////////////////////////////////
	bResult = connect(m_ptrAddComp, SIGNAL(clicked(bool)), CDataThread::getInstance(), SLOT(onAddComp()));
	logConnection("MainWindow::setConnections","'m_ptrAddComp::clicked' with 'CDataThread::onAddComp'", bResult);
	bResult = connect(m_ptrAddShop, SIGNAL(clicked(bool)), CDataThread::getInstance(), SLOT(onAddShop()));
	logConnection("MainWindow::setConnections","'m_ptrAddShop::clicked' with 'CDataThread::onAddShop'", bResult);
	//////////////////////////////////////////////////////////////////////////
	bResult = connect(m_ptCompConfView, SIGNAL(removeShop(int)), CDataThread::getInstance(), SLOT(onRemShop(int)));
	logConnection("MainWindow::setConnections","'m_ptCompConfView::removeShop' with 'CDataThread::onRemShop'", bResult);
	//////////////////////////////////////////////////////////////////////////
	bResult = connect(m_ptCompConfView, SIGNAL(removeComponent(int)), CDataThread::getInstance(), SLOT(onRemComp(int)));
	logConnection("MainWindow::setConnections","'m_ptCompConfView::removeComponent' with 'CDataThread::onRemComp'", bResult);
	////////////////////////////////////////////////////////////////////////////
	bResult = connect(m_ptCompConfView, SIGNAL(onOpenLink(QModelIndex)) ,
		CDataThread::getInstance(), SLOT(onOpenLinkInExternalBrowser(QModelIndex)));
	logConnection("MainWindow::setConnections","'m_ptCompConfView::onOpenLink' with 'CDataThread::onOpenLinkInExternalBrowser'", bResult);
	////////////////////////////////////////////////////////////////////////////
	bResult = connect(m_ptCompConfView, SIGNAL(onOpenSearchPage(QModelIndex)), CDataThread::getInstance(), SLOT(onOpenSearchInExternalBrowser(QModelIndex)));
	logConnection("MainWindow::setConnections","'m_ptCompConfView::onOpenSearchPage' with 'CDataThread::onOpenSerachInExternalBrowser'", bResult);
	////////////////////////////////////////////////////////////////////////////
	bResult = connect(m_ptCompConfView, SIGNAL(resetFieldCache(QModelIndex)) ,
		CDataThread::getInstance(), SLOT(onResetFieldCache(QModelIndex)));
	logConnection("MainWindow::setConnections","'m_ptCompConfView::onOpenSearchPage' with 'CDataThread::onOpenSerachInExternalBrowser'", bResult);
	////////////////////////////////////////////////////////////////////////////
	bResult = connect(CDataThread::getInstance(), SIGNAL(onStatusBarMsg(const QString & )) ,
		this , SLOT(onStatusBarMsgChange(const QString &)));
	logConnection("MainWindow::setConnections","'CDataThread::getInstance()::onStatusBarMsg' with 'this::onStatusBarMsgChange'", bResult);
	////////////////////////////////////////////////////////////////////////////
	bResult = connect(CDataThread::getInstance(), SIGNAL(updateRecentFilesAction(const QStringList &)), this , SLOT(onUpdateRecentFileActions(const QStringList &)));
	logConnection("MainWindow::setConnections","'CDataThread::getInstance()::updateRecentFilesAction' with 'this::onUpdateRecentFileActions'", bResult);
*/
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
