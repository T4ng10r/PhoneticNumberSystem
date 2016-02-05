#include <GUI/Settings/SettingsDlg.h>
#include <GUI/Settings/SubstituteValuesConfigurationDlg.h>
#include <GUI/Settings/DictionariesConfigurationDlg.h>
#include <data/Settings.h>
#include <data/CSettingsKeywords.h>
#include <Tools/loggers.h>
#include <QBoxLayout>
#include <QTabWidget>
#include <Tools/qtTools.h>

class CSettingsDlgPrivate
{
public:
     CSettingsDlgPrivate(CSettingsDlg * ptrPublic);
     ~CSettingsDlgPrivate();
	 void setupUI();
	 void setConnections();
public:
     CSettingsDlg *				m_ptrPublic;
	 QVBoxLayout *					mainVerticalLayout;
	 QTabWidget *					tabWidget;
	 boost::shared_ptr<CSubstituteValuesConfigurationDlg>	substituteConfiguration;
	 boost::shared_ptr<CDictionariesConfigurationDlg>		dictionariesConfiguration;
};

CSettingsDlgPrivate::CSettingsDlgPrivate(CSettingsDlg * ptrPublic):m_ptrPublic(ptrPublic)
{
	setupUI();
	setConnections();
}
CSettingsDlgPrivate::~CSettingsDlgPrivate(){}
void CSettingsDlgPrivate::setupUI()
{
	m_ptrPublic->setObjectName(QString::fromUtf8("SettingsDlg"));
	mainVerticalLayout = new QVBoxLayout(m_ptrPublic);
	mainVerticalLayout->setObjectName(QString::fromUtf8("mainVerticalLayout"));

	tabWidget = new QTabWidget;
	mainVerticalLayout->addWidget(tabWidget);

	//CSubstituteValuesConfiguration  stSubstituteValuesConfiguration;
  CSubstituteValuesConfigurationDlgInit init_data = 
    { CSubstituteValuesConfiguration(), 
    gSettings->getDigitsConfiguraions(), 
    gSettings->get<std::string>(SELECTED_CONSONANTS_SYSTEM,"").c_str()
    };
	substituteConfiguration.reset(new CSubstituteValuesConfigurationDlg(init_data));
	tabWidget->addTab(substituteConfiguration.get(), "Consonants");

	dictionariesConfiguration.reset(new CDictionariesConfigurationDlg);
	tabWidget->addTab(dictionariesConfiguration.get(), "Dictionaries");
}
void CSettingsDlgPrivate::setConnections()
{
	bool bResult = false;
	bResult = QObject::connect(dictionariesConfiguration.get(), SIGNAL(dictionarySelected()), 
		m_ptrPublic, SIGNAL(dictionarySelected()));
	logConnection("CSettingsDlgPrivate::setConnections",
		"'dictionariesConfiguration::dictionarySelected' with 'CSettingsDlg::dictionarySelected'", 
		bResult);
  //////////////////////////////////////////////////////////////////////////
  bResult = QObject::connect(substituteConfiguration.get(), SIGNAL(set_selected_consonant_system(const QString &)), 
    gSettings.get(), SLOT(on_set_selected_consonant_system(const QString &)));
  logConnection("CSettingsDlgPrivate::setConnections",
    "'substituteConfiguration::set_selected_consonant_system' with 'Settings::on_set_selected_consonant_system'", 
    bResult);
}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
CSettingsDlg::CSettingsDlg(QWidget * parent):QWidget(parent),
	_pimpl(new CSettingsDlgPrivate(this))
{}
CSettingsDlg::~CSettingsDlg(void){}
void CSettingsDlg::performInitialUpdateAfterAllChildrenUpdate()
{
	_pimpl->dictionariesConfiguration->updateInitialData();
}
