#include <GUI/MainWindow.h>
#include <GUI/MainWindow_p.h>
#include <GUI/Settings/SettingsDlg.h>
#include <GUI/search_phonetic_representation/search_phonetic_representations_dlg.h>
#include <data/DataThread.h>
#include <QStatusBar>
#include <QMenuBar>
#include <QFileInfo>
#include <tools/loggers.h>
#include <tools/qtTools.h>
#include <data/settings.h>
#include <boost/scoped_ptr.hpp>

const QSize ciSize(700,450);

MainWindowPrivate::MainWindowPrivate(CMainWindow * ptrPublic):public_ptr(ptrPublic)
{
	setupUI();
	setupActions();
	gDataThread;
	setConnections();
	settings_dialog->performInitialUpdateAfterAllChildrenUpdate();
	public_ptr->statusBar()->showMessage("Ready");
}
MainWindowPrivate::~MainWindowPrivate()
{}
void MainWindowPrivate::setupUI()
{
	central_widget_ = new QWidget;
	public_ptr->setCentralWidget(central_widget_);

  QVBoxLayout* main_layout = new QVBoxLayout;
	delete central_widget_->layout();
	central_widget_->setLayout(main_layout);
	settings_dialog.reset(new CSettingsDlg);

	search_phonetic_representations_dialog.reset(new CSearchPhoneticRepresentationsDlg);
	main_layout->addWidget(search_phonetic_representations_dialog.get());
}
void MainWindowPrivate::setConnections()
{
	bool bResult = false;
	bResult = QObject::connect(action_open_settings, SIGNAL(triggered(bool)), 
		public_ptr, SLOT(onShowSettingsConfigureDialog(bool)));
	logConnection("MainWindowPrivate::setConnections",
		"'MainWindowPrivate::triggered' with 'CMainWindow::onActionTrigger'", 
		bResult);

	bResult = QObject::connect(search_phonetic_representations_dialog.get(), SIGNAL(performSearch(const std::string & )),
		gDataThread.get(), SLOT(onNumberSearchStarted(const std::string & )) );
	logConnection("MainWindowPrivate::setConnections",
		"'DataThread::searchProgess' with 'search_phonetic_representations_dialog::onSearchProgess'", 
		bResult);

	bResult = QObject::connect(gDataThread.get(), SIGNAL(searchProgress(int, int)), 
		search_phonetic_representations_dialog.get(), SLOT(onSearchProgress(int, int )));
	logConnection("MainWindowPrivate::setConnections",
		"'DataThread::searchProgess' with 'search_phonetic_representations_dialog::onSearchProgess'", 
		bResult);

	bResult = QObject::connect(gDataThread.get(), SIGNAL(searchFinished(bool)),
		search_phonetic_representations_dialog.get(), SLOT(searchFinished(bool)) );
	logConnection("MainWindowPrivate::setConnections",
		"'DataThread::searchFinished' with 'search_phonetic_representations_dialog::searchFinished'", 
		bResult);

	bResult = QObject::connect(settings_dialog.get(), SIGNAL(dictionarySelected()),
		search_phonetic_representations_dialog.get(), SLOT(disableSearchButton()) );
	logConnection("MainWindowPrivate::setConnections",
		"'settings_dialog::dictionarySelected' with 'search_phonetic_representations_dialog::disableSearchButton'", 
		bResult);

	bResult = QObject::connect(gDataThread.get(), SIGNAL(dictionaryLoaded(bool)),
		search_phonetic_representations_dialog.get(), SLOT(enableSearchButton()) );
	logConnection("MainWindowPrivate::setConnections",
		"'DataThread::dictionaryLoaded' with 'search_phonetic_representations_dialog::enableSearchButton'", 
		bResult);
}
void MainWindowPrivate::setupActions()
{
	action_new_configuration = public_ptr->menuBar()->addAction("New");
	action_new_configuration->setShortcut(QKeySequence::New);
	action_new_configuration->setToolTip("");
	public_ptr->menuBar()->addSeparator();
	////////////////////////////////////////////////////////////////////////////
	recent_files_menu = public_ptr->menuBar()->addMenu("Open");
	action_open_configuration = recent_files_menu->addAction("Open");
	action_open_configuration->setShortcut(QKeySequence::Open);
	action_open_configuration->setToolTip("");
	recent_files_menu->addSeparator();
	////////////////////////////////////////////////////////////////////////////
	for (int i = 0; i < MaxRecentFiles; ++i) {	
		actions_recent_file[i] = recent_files_menu->addAction("");
		actions_recent_file[i]->setVisible(false);
		//public_ptr->connect(actions_recent_file[i], SIGNAL(triggered()),
		//	this, SLOT(onOpenRecentFile()));
	}
	//////////////////////////////////////////////////////////////////////////
	action_save_configuration = public_ptr->menuBar()->addAction("Save");
	action_save_configuration->setShortcut(QKeySequence::Save);
	action_save_configuration->setToolTip("");
	////////////////////////////////////////////////////////////////////////////
	action_save_configuration_as = public_ptr->menuBar()->addAction("Save as");
	action_save_configuration_as->setShortcut(QKeySequence::SaveAs);
	action_save_configuration_as->setToolTip("");
	////////////////////////////////////////////////////////////////////////////
	public_ptr->menuBar()->addSeparator();
	action_open_settings= public_ptr->menuBar()->addAction("Configuration");
	action_open_settings->setToolTip("");
}
