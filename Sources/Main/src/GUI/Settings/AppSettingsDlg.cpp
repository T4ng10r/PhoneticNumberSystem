#include <GUI/Settings/AppSettingsDlg.h>
#include <GUI/Settings/SubstituteValuesConfigurationDlg.h>
#include <GUI/Settings/DictionariesConfigurationDlg.h>
#include <Data/CAppSettings.h>
#include <Data/CAppSettingsKeywords.h>
#include <Tools/loggers.h>
#include <QBoxLayout>
#include <QTabWidget>
#include <Tools/qtTools.h>

class CAppSettingsDlgPrivate
{
public:
     CAppSettingsDlgPrivate(CAppSettingsDlg * ptrPublic);
     ~CAppSettingsDlgPrivate();
	 void setupUI();
	 void setConnections();
public:
     CAppSettingsDlg *				m_ptrPublic;
	 QVBoxLayout *					mainVerticalLayout;
	 QTabWidget *					tabWidget;
	 boost::shared_ptr<CSubstituteValuesConfigurationDlg>	substituteConfiguration;
	 boost::shared_ptr<CDictionariesConfigurationDlg>		dictionariesConfiguration;
};

CAppSettingsDlgPrivate::CAppSettingsDlgPrivate(CAppSettingsDlg * ptrPublic):m_ptrPublic(ptrPublic)
{
	setupUI();
	setConnections();
}
CAppSettingsDlgPrivate::~CAppSettingsDlgPrivate(){}
void CAppSettingsDlgPrivate::setupUI()
{
	m_ptrPublic->setObjectName(QString::fromUtf8("AppSettingsDlg"));
	mainVerticalLayout = new QVBoxLayout(m_ptrPublic);
	mainVerticalLayout->setObjectName(QString::fromUtf8("mainVerticalLayout"));

	tabWidget = new QTabWidget;
	mainVerticalLayout->addWidget(tabWidget);

	//CSubstituteValuesConfiguration  stSubstituteValuesConfiguration;
  CSubstituteValuesConfigurationDlgInit init_data = 
    { CSubstituteValuesConfiguration(), 
    gAppSettings->getDigitsConfiguraions(), 
    gAppSettings->get<std::string>(SELECTED_CONSONANTS_SYSTEM,"").c_str()
    };
	substituteConfiguration.reset(new CSubstituteValuesConfigurationDlg(init_data));
	tabWidget->addTab(substituteConfiguration.get(), "Consonants");

	dictionariesConfiguration.reset(new CDictionariesConfigurationDlg);
	tabWidget->addTab(dictionariesConfiguration.get(), "Dictionaries");
}
void CAppSettingsDlgPrivate::setConnections()
{
	bool bResult = false;
	bResult = QObject::connect(dictionariesConfiguration.get(), SIGNAL(dictionarySelected()), 
		m_ptrPublic, SIGNAL(dictionarySelected()));
	logConnection("CAppSettingsDlgPrivate::setConnections",
		"'dictionariesConfiguration::dictionarySelected' with 'CAppSettingsDlg::dictionarySelected'", 
		bResult);
  //////////////////////////////////////////////////////////////////////////
  bResult = QObject::connect(substituteConfiguration.get(), SIGNAL(set_selected_consonant_system(const QString &)), 
    gAppSettings.get(), SLOT(on_set_selected_consonant_system(const QString &)));
  logConnection("CAppSettingsDlgPrivate::setConnections",
    "'substituteConfiguration::set_selected_consonant_system' with 'CAppSettings::on_set_selected_consonant_system'", 
    bResult);
}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
CAppSettingsDlg::CAppSettingsDlg(QWidget * parent):QWidget(parent),
	m_ptrPriv(new CAppSettingsDlgPrivate(this))
{}
CAppSettingsDlg::~CAppSettingsDlg(void){}
void CAppSettingsDlg::performInitialUpdateAfterAllChildrenUpdate()
{
	m_ptrPriv->dictionariesConfiguration->updateInitialData();
}
