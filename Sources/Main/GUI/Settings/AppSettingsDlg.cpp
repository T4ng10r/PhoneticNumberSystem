#include <GUI/Settings/AppSettingsDlg.h>
#include <GUI/Settings/SubstituteValuesConfigurationDlg.h>
#include <GUI/Settings/DictionariesConfigurationDlg.h>
#include <Data/CAppSettings.h>
#include <tools/loggers.h>
#include <QtGui/QBoxLayout>
#include <QtGui/QTabWidget>
#include <tools/qtTools.h>

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

	CSubstituteValuesConfiguration  stSubstituteValuesConfiguration;
	substituteConfiguration.reset(new CSubstituteValuesConfigurationDlg(stSubstituteValuesConfiguration));
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