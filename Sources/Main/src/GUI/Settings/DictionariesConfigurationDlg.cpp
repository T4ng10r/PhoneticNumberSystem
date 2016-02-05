#include <GUI/Settings/DictionariesConfigurationDlg.h>
#include <data/Settings.h>
#include <data/CSettingsKeywords.h>
#include <data/DataThread.h>
#include <Tools/loggers.h>
#include <Tools/qtTools.h>
#include <boost/foreach.hpp>
#include <QVariant>
#include <QAction>
#include <QApplication>
#include <QButtonGroup>
#include <QComboBox>
#include <QGridLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QSpacerItem>
#include <QVBoxLayout>
#include <QWidget>
#include <QFileDialog>
#include <QMenu>
#include <QAction>
#include <boost/format.hpp>

#define REFRESH_ACTION_NAME "Refresh"

class CDictionariesConfigurationDlgPrivate
{
public:
     CDictionariesConfigurationDlgPrivate(CDictionariesConfigurationDlg * ptrPublic);
     ~CDictionariesConfigurationDlgPrivate();
	 void setupUI();
	 void retranslateUI();
	 void setupUI_CreateSelectedDictionaryRow();
	 void setupUI_CreateFindDictionariesButtons();
	 void setupUI_CreateDictionaryDirectoryInfoRow();
	 void setupUI_CreateSelectDictionariesFromWeb();
	 void setupUI_RefreshDictionariesFilesMenu();
	 void setupUI_AddRefreshAction();
	 void setConnections();
	 void setDictionaryFilesMenuActionConnection(QAction * action);
	 void setCurrentlySetDictionaryInMenu();
public:
     CDictionariesConfigurationDlg *                              m_ptrPublic;

	 QVBoxLayout *		mainVerticalLayout;
	 QLabel *			selectedDictionaryLabel;
	 QPushButton *		changeDictionaryButton;
	 QPushButton *		changeDictionaryDirButton;
	 QPushButton *		downloadDictButton;
	 QLabel *			dictionaryDirectoryLabel;
	 QLineEdit *		dictionaryDirectoryEdit;
	 QGroupBox *		dictionariesToDownloadGroup;

	 QGridLayout *		gridLayout;
	 QComboBox *		comboBox;
	 QMenu *			dicionariesFilesMenu;
};

CDictionariesConfigurationDlgPrivate::CDictionariesConfigurationDlgPrivate(CDictionariesConfigurationDlg * ptrPublic):m_ptrPublic(ptrPublic)
{
	setupUI();
	retranslateUI();
	setConnections();
}
CDictionariesConfigurationDlgPrivate::~CDictionariesConfigurationDlgPrivate(){}
void CDictionariesConfigurationDlgPrivate::setupUI()
{
	m_ptrPublic->setObjectName(QString::fromUtf8("SelectDictionary"));
	mainVerticalLayout = new QVBoxLayout;
	mainVerticalLayout->setObjectName("mainVerticalLayout");
	m_ptrPublic->setLayout(mainVerticalLayout);
	mainVerticalLayout->setObjectName(QString::fromUtf8("mainVerticalLayout"));
	dicionariesFilesMenu = new QMenu;

	setupUI_RefreshDictionariesFilesMenu();
	setupUI_CreateSelectedDictionaryRow();
	setupUI_CreateFindDictionariesButtons();
	setupUI_CreateDictionaryDirectoryInfoRow();
	setupUI_CreateSelectDictionariesFromWeb();
}
void CDictionariesConfigurationDlgPrivate::setupUI_CreateSelectedDictionaryRow()
{
	QHBoxLayout *	horizontalLayout = new QHBoxLayout;
	horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout_DictionaryRow"));
	selectedDictionaryLabel = new QLabel(m_ptrPublic);
	selectedDictionaryLabel->setObjectName(QString::fromUtf8("label"));
	QSizePolicy sizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred);
	sizePolicy.setHorizontalStretch(0);
	sizePolicy.setVerticalStretch(0);
	sizePolicy.setHeightForWidth(selectedDictionaryLabel->sizePolicy().hasHeightForWidth());
	selectedDictionaryLabel->setSizePolicy(sizePolicy);

	horizontalLayout->addWidget(selectedDictionaryLabel);

	changeDictionaryButton = new QPushButton(m_ptrPublic);
	changeDictionaryButton->setObjectName(QString::fromUtf8("changeDictionaryButton"));
	changeDictionaryButton->setFlat(true);
	changeDictionaryButton->setMenu(dicionariesFilesMenu);

	horizontalLayout->addWidget(changeDictionaryButton);


	mainVerticalLayout->addLayout(horizontalLayout);
}
void CDictionariesConfigurationDlgPrivate::setupUI_CreateFindDictionariesButtons() 
{
	QHBoxLayout *	horizontalLayout = new QHBoxLayout();
	horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
	QSpacerItem *horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
	horizontalLayout->addItem(horizontalSpacer_4);

	changeDictionaryDirButton = new QPushButton(m_ptrPublic);
	changeDictionaryDirButton->setObjectName(QString::fromUtf8("changeDictionaryDirButton"));

	horizontalLayout->addWidget(changeDictionaryDirButton);

	QSpacerItem *horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
	horizontalLayout->addItem(horizontalSpacer);

	downloadDictButton = new QPushButton(m_ptrPublic);
	downloadDictButton->setObjectName(QString::fromUtf8("downloadDictButton"));

	horizontalLayout->addWidget(downloadDictButton);

	QSpacerItem *horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
	horizontalLayout->addItem(horizontalSpacer_3);


	mainVerticalLayout->addLayout(horizontalLayout);
}
void CDictionariesConfigurationDlgPrivate::setupUI_CreateDictionaryDirectoryInfoRow() 
{
	QHBoxLayout *horizontalLayout = new QHBoxLayout();

	horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout_3"));
	dictionaryDirectoryLabel = new QLabel(m_ptrPublic);
	dictionaryDirectoryLabel->setObjectName(QString::fromUtf8("dictionaryDirectoryLabel"));

	horizontalLayout->addWidget(dictionaryDirectoryLabel);

	dictionaryDirectoryEdit = new QLineEdit(m_ptrPublic);
	dictionaryDirectoryEdit->setObjectName(QString::fromUtf8("dictionaryDirectoryEdit"));
	dictionaryDirectoryEdit->setReadOnly(true);

	horizontalLayout->addWidget(dictionaryDirectoryEdit);


	mainVerticalLayout->addLayout(horizontalLayout);
}
void CDictionariesConfigurationDlgPrivate::setupUI_CreateSelectDictionariesFromWeb() 
{
	dictionariesToDownloadGroup = new QGroupBox(m_ptrPublic);
	dictionariesToDownloadGroup->setObjectName(QString::fromUtf8("dictionariesToDownloadGroup"));
	gridLayout = new QGridLayout(dictionariesToDownloadGroup);
	gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
	comboBox = new QComboBox(dictionariesToDownloadGroup);
	comboBox->setObjectName(QString::fromUtf8("comboBox"));

	gridLayout->addWidget(comboBox, 0, 0, 1, 1);


	mainVerticalLayout->addWidget(dictionariesToDownloadGroup);
}
void CDictionariesConfigurationDlgPrivate::setupUI_RefreshDictionariesFilesMenu() 
{
	try
	{
		const boost::property_tree::ptree & dictFilesTree = gSettings->get_child(DICTIONARY_FILES_LIST);
		BOOST_FOREACH(const boost::property_tree::ptree::value_type & dictionaryFileItem, dictFilesTree)
		{
			QString actionText = dictionaryFileItem.second.data().c_str();
			actionText = actionText.left(actionText.lastIndexOf('.'));
			QAction * action = new QAction(actionText,NULL);
			dicionariesFilesMenu->addAction(action);
			setDictionaryFilesMenuActionConnection(action);
		}
	}
	catch (boost::property_tree::ptree_bad_path &e)
	{
		printLog(eDebug, eWarningLogLevel, str(boost::format("Error during gathering dictionaryFilesList '%1%'") % e.what()));	
	}
	setupUI_AddRefreshAction();
}
void CDictionariesConfigurationDlgPrivate::setupUI_AddRefreshAction()
{
	dicionariesFilesMenu->addSeparator();
	QAction * action = new QAction(QString(REFRESH_ACTION_NAME),NULL);
	dicionariesFilesMenu->addAction(action);
	setDictionaryFilesMenuActionConnection(action);
}
void CDictionariesConfigurationDlgPrivate::retranslateUI()
{
	m_ptrPublic->setWindowTitle(QApplication::translate("selectdictionary", "Selected Dictionary", 0, 0));
	selectedDictionaryLabel->setText(QApplication::translate("selectdictionary", "Selected dictionary", 0, 0));
	changeDictionaryButton->setText(QApplication::translate("selectdictionary", "Change", 0, 0));
	changeDictionaryDirButton->setText(QApplication::translate("selectdictionary", "Browse", 0, 0));
	downloadDictButton->setText(QApplication::translate("selectdictionary", "Download", 0, 0));
	dictionaryDirectoryLabel->setText(QApplication::translate("selectdictionary", "Directory", 0, 0));
	dictionariesToDownloadGroup->setTitle(QApplication::translate("selectdictionary", "GroupBox", 0, 0));
}
void CDictionariesConfigurationDlgPrivate::setConnections()
{
	bool bResult = false;
	bResult = QObject::connect(changeDictionaryDirButton, SIGNAL(clicked(bool)), 
		m_ptrPublic, SLOT(onSelectDictionaryDirectory()));
	logConnection("CDictionariesConfigurationDlgPrivate::setConnections",
		"'changeDictionaryDirButton::clicked' with 'CDictionariesConfigurationDlg::onSelectDictionaryDirectory'", 
		bResult);

	bResult = QObject::connect(m_ptrPublic, SIGNAL(onDictionaryDirectoryChanged()), 
		gDataThread.get(), SLOT(onScanDirectoryForDictionaries()));
	logConnection("CDictionariesConfigurationDlgPrivate::setConnections",
		"'dictionariesConfiguration::onDictionaryDirectoryChanged' with 'gDataThread::onScanDirectoryForDictionaries'", 
		bResult);

	bResult = QObject::connect(gDataThread.get(), SIGNAL(onDictionariesFilesRefreshed()), 
		m_ptrPublic, SLOT(onDictionariesFilesRefreshed()));
	logConnection("CDictionariesConfigurationDlgPrivate::setConnections",
		"'gDataThread::onDictionariesFilesRefreshed' with 'm_ptrPublic::onDictionariesFilesRefreshed'", 
		bResult);
	
	bResult = QObject::connect(m_ptrPublic, SIGNAL(dictionarySelected()),
		gDataThread.get(), SLOT(onSetDictionary()));
	logConnection("CDictionariesConfigurationDlgPrivate::setConnections",
		"'m_ptrPublic::onDictionarySelected' with 'gDataThread::onSetDictionary'", 
		bResult);
}
void CDictionariesConfigurationDlgPrivate::setDictionaryFilesMenuActionConnection(QAction * action) 
{
	bool bResult = false;
	bResult = QObject::connect(action, SIGNAL(triggered(bool)), 
		m_ptrPublic, SLOT(onDictionaryFilesMenuActionToggled()));
	logConnection("CDictionariesConfigurationDlgPrivate::setConnections",
		"'changeDictionaryDirButton::clicked' with 'CDictionariesConfigurationDlg::onSelectDictionaryDirectory'", 
		bResult);
}
void CDictionariesConfigurationDlgPrivate::setCurrentlySetDictionaryInMenu()
{
	QString dictDir = gSettings->get<std::string>(SELECTED_DICTIONARY,"").c_str();
	if (dictDir.length()==0)
		return;
	Q_FOREACH(QAction * const action, dicionariesFilesMenu->actions())
	{
		if (action->isSeparator()==false && action->text()==dictDir)
		{
			action->trigger();
			return;
		}
	}
}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
CDictionariesConfigurationDlg::CDictionariesConfigurationDlg(QWidget * parent):QWidget(parent),
_pimpl(new CDictionariesConfigurationDlgPrivate(this))
{
}
CDictionariesConfigurationDlg::~CDictionariesConfigurationDlg(void){}
void CDictionariesConfigurationDlg::updateInitialData()
{
	_pimpl->dictionaryDirectoryEdit->setText(gSettings->get<std::string>(DICTIONARIES_DIRECTORY,"").c_str());
	Q_EMIT onDictionaryDirectoryChanged();
	_pimpl->setCurrentlySetDictionaryInMenu();
}
void CDictionariesConfigurationDlg::onSelectDictionaryDirectory()
{
	QString dictDir = gSettings->get<std::string>(DICTIONARIES_DIRECTORY).c_str();
	QString dir = QFileDialog::getExistingDirectory(this, tr("Select Dictionaries Directory"));
	if (dir==dictDir)
		return;
	gSettings->put<std::string>(DICTIONARIES_DIRECTORY,dir.toStdString());
	_pimpl->dictionaryDirectoryEdit->setText(dir);
	Q_EMIT onDictionaryDirectoryChanged();
}
void CDictionariesConfigurationDlg::onDictionariesFilesRefreshed()
{
	_pimpl->dicionariesFilesMenu->clear();
	QString dictDir = gSettings->get<std::string>(DICTIONARIES_DIRECTORY).c_str();
	_pimpl->dictionaryDirectoryEdit->setText(dictDir);
	_pimpl->setupUI_RefreshDictionariesFilesMenu();
}
void CDictionariesConfigurationDlg::onDictionaryFilesMenuActionToggled()
{
	QAction * actionToggled = static_cast<QAction*>(sender());
	QString dictionaryName = actionToggled->text();
	if (dictionaryName==REFRESH_ACTION_NAME)
	{
		_pimpl->changeDictionaryButton->setText(QApplication::translate("selectdictionary", "Change", 0, 0));
		Q_EMIT onDictionaryDirectoryChanged();
		return;
	}
	dictionaryName.remove(DICTIONARY_FILE_EXTENSION);
	_pimpl->changeDictionaryButton->setText(dictionaryName);
	gSettings->put<std::string>(SELECTED_DICTIONARY,dictionaryName.toStdString());
	gSettings->saveSettings();
	Q_EMIT dictionarySelected();
}
