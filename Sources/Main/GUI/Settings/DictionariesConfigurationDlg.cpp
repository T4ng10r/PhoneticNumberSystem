#include <GUI/Settings/DictionariesConfigurationDlg.h>
#include <Data/CAppSettings.h>
#include <Data/CAppSettingsKeywords.h>
#include <Data/CDataThread.h>
#include <tools/loggers.h>
#include <tools/qtTools.h>
#include <boost/foreach.hpp>
#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include <QtGui/QFileDialog>
#include <QtGui/QMenu>
#include <QtGui/QAction>

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
	BOOST_FOREACH(const boost::property_tree::ptree::value_type & dictionaryFileItem, gAppSettings->get_child(DICTIONARY_FILES_LIST))
	{
		QString actionText = dictionaryFileItem.second.data().c_str();
		actionText = actionText.left(actionText.lastIndexOf('.'));
		QAction * action = new QAction(actionText,NULL);
		dicionariesFilesMenu->addAction(action);
		setDictionaryFilesMenuActionConnection(action);
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
	m_ptrPublic->setWindowTitle(QApplication::translate("selectdictionary", "Selectd Dctionary", 0, QApplication::UnicodeUTF8));
	selectedDictionaryLabel->setText(QApplication::translate("selectdictionary", "Selected dictionary", 0, QApplication::UnicodeUTF8));
	changeDictionaryButton->setText(QApplication::translate("selectdictionary", "Change", 0, QApplication::UnicodeUTF8));
	changeDictionaryDirButton->setText(QApplication::translate("selectdictionary", "Browse", 0, QApplication::UnicodeUTF8));
	downloadDictButton->setText(QApplication::translate("selectdictionary", "Download", 0, QApplication::UnicodeUTF8));
	dictionaryDirectoryLabel->setText(QApplication::translate("selectdictionary", "Directory", 0, QApplication::UnicodeUTF8));
	dictionariesToDownloadGroup->setTitle(QApplication::translate("selectdictionary", "GroupBox", 0, QApplication::UnicodeUTF8));
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
	QString dictDir = gAppSettings->get<std::string>(SELECTED_DICTIONARY).c_str();
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
m_ptrPriv(new CDictionariesConfigurationDlgPrivate(this))
{
}
CDictionariesConfigurationDlg::~CDictionariesConfigurationDlg(void){}
void CDictionariesConfigurationDlg::updateInitialData()
{
	m_ptrPriv->dictionaryDirectoryEdit->setText(gAppSettings->get<std::string>(DICTIONARIES_DIRECTORY,"").c_str());
	Q_EMIT onDictionaryDirectoryChanged();
	m_ptrPriv->setCurrentlySetDictionaryInMenu();
}
void CDictionariesConfigurationDlg::onSelectDictionaryDirectory()
{
	QString dictDir = gAppSettings->get<std::string>(DICTIONARIES_DIRECTORY).c_str();
	QString dir = QFileDialog::getExistingDirectory(this, tr("Select Dictionaries Directory"));
	if (dir==dictDir)
		return;
	gAppSettings->put<std::string>(DICTIONARIES_DIRECTORY,dir.toStdString());
	m_ptrPriv->dictionaryDirectoryEdit->setText(dir);
	Q_EMIT onDictionaryDirectoryChanged();
}
void CDictionariesConfigurationDlg::onDictionariesFilesRefreshed()
{
	m_ptrPriv->dicionariesFilesMenu->clear();
	QString dictDir = gAppSettings->get<std::string>(DICTIONARIES_DIRECTORY).c_str();
	m_ptrPriv->dictionaryDirectoryEdit->setText(dictDir);
	m_ptrPriv->setupUI_RefreshDictionariesFilesMenu();
}
void CDictionariesConfigurationDlg::onDictionaryFilesMenuActionToggled()
{
	QAction * actionToggled = static_cast<QAction*>(sender());
	QString dictionaryName = actionToggled->text();
	if (dictionaryName==REFRESH_ACTION_NAME)
	{
		m_ptrPriv->changeDictionaryButton->setText(QApplication::translate("selectdictionary", "Change", 0, QApplication::UnicodeUTF8));
		Q_EMIT onDictionaryDirectoryChanged();
		return;
	}
	dictionaryName.remove(DICTIONARY_FILE_EXTENSION);
	m_ptrPriv->changeDictionaryButton->setText(dictionaryName);
	gAppSettings->put<std::string>(SELECTED_DICTIONARY,dictionaryName.toStdString());
	gAppSettings->saveSettings();
	Q_EMIT dictionarySelected();
}
