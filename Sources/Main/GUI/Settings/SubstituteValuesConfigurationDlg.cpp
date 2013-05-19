#include <GUI/Settings/SubstituteValuesConfigurationDlg.h>
#include <QtCore/QString>
#include <QtGui/QMenu>
#include <QtGui/QAction>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QBoxLayout>
#include <tools/loggers.h>
#include <Tools/qtTools.h>
#include <Data/CAppSettings.h>
#include <boost/lexical_cast.hpp>
#include <boost/foreach.hpp>

extern CAppSettings	stAppSettings;

//Qt::ContextMenuPolicy
//Qt::CustomContextMenu
//QWidget::actions()

//char aConsonats[] = {' ', 'B', 'C', 'D', 'F', 'G', 'H', 'J', 'K', 'L', 'M', 'N', 
//'P', 'Q', 'R', 'S', 'T', 'V', 'X', 'Z', 'W'};
//enum { ConsonantsMax = sizeof(aConsonats) };
enum { DigitsCount = 10 };

class CSubstituteValuesConfigurationDlgPrivate
{
public:
	CSubstituteValuesConfigurationDlgPrivate(CSubstituteValuesConfigurationDlg * ptrPublic);
	~CSubstituteValuesConfigurationDlgPrivate();
	void setupUI();
	QHBoxLayout * setupUI_line(int iIndex);
	void setupActions();
	void setConnections();
public:
	CSubstituteValuesConfigurationDlg *		m_ptrPublic;
	QMenu *									m_ptrConsonantsMenu1;
	QMenu *									m_ptrConsonantsMenu2;
	std::vector<QLabel *>					m_ptrConsonantLabels;
	std::vector<QPushButton *>				m_ptrConsonantButton1;
	std::vector<QPushButton *>				m_ptrConsonantButton2;
	std::vector<QAction *>					m_ptrConsonantsActions1;
	std::vector<QAction *>					m_ptrConsonantsActions2;
};

CSubstituteValuesConfigurationDlgPrivate::CSubstituteValuesConfigurationDlgPrivate(CSubstituteValuesConfigurationDlg * ptrPublic):m_ptrPublic(ptrPublic)
{
	setupUI();
	setupActions();
	setConnections();
}
CSubstituteValuesConfigurationDlgPrivate::~CSubstituteValuesConfigurationDlgPrivate(){}
void CSubstituteValuesConfigurationDlgPrivate::setupUI()
{
	int iConsonantsCount(stAppSettings.get<int>("settings.consonants.count"));
	QVBoxLayout * ptrMainLayout = new QVBoxLayout;
	delete m_ptrPublic->layout();
	m_ptrPublic->setLayout(ptrMainLayout);

	m_ptrConsonantLabels.resize(DigitsCount);
	m_ptrConsonantButton1.resize(DigitsCount);
	m_ptrConsonantButton2.resize(DigitsCount);
	m_ptrConsonantsActions1.resize(iConsonantsCount+1);
	m_ptrConsonantsActions2.resize(iConsonantsCount+1);

	m_ptrConsonantsMenu1 = new QMenu;
	m_ptrConsonantsMenu2 = new QMenu;

	for(int i=0;i<DigitsCount;i++)
	{
		ptrMainLayout->addLayout(setupUI_line(i));
	}

	bool bResult = false;
	
	bResult = QObject::connect(m_ptrConsonantsMenu1, SIGNAL(triggered ( QAction *)), 
		m_ptrPublic, SLOT(onPushButton1ActionTriggered(QAction *)));
	logConnection("CSubstituteValuesConfigurationDlgPrivate::setConnections",
		"'m_ptrConsonantsMenu1::triggered' with 'CSubstituteValuesConfigurationDlg::onPushButton1ActionTriggered'", 
		bResult);
	bResult = QObject::connect(m_ptrConsonantsMenu2, SIGNAL(triggered ( QAction *)), 
		m_ptrPublic, SLOT(onPushButton2ActionTriggered(QAction *)));
	logConnection("CSubstituteValuesConfigurationDlgPrivate::setConnections",
		"'m_ptrConsonantsMenu2::triggered' with 'CSubstituteValuesConfigurationDlg::onPushButton2ActionTriggered'", 
		bResult);
}
QHBoxLayout * CSubstituteValuesConfigurationDlgPrivate::setupUI_line(int iIndex)
{
	QHBoxLayout * ptrHLayout = new QHBoxLayout;
	m_ptrConsonantLabels[iIndex] = new QLabel;
	std::string strIndex = boost::lexical_cast<std::string>(iIndex);
	m_ptrConsonantLabels[iIndex]->setText(strIndex.data());
	ptrHLayout->addWidget(m_ptrConsonantLabels[iIndex]);
	m_ptrConsonantButton1[iIndex] = new QPushButton;
	m_ptrConsonantButton1[iIndex]->setMenu(m_ptrConsonantsMenu1);
	m_ptrConsonantButton1[iIndex]->setFlat(true);
	ptrHLayout->addWidget(m_ptrConsonantButton1[iIndex]);
	m_ptrConsonantButton2[iIndex] = new QPushButton;
	m_ptrConsonantButton2[iIndex]->setFlat(true);
	m_ptrConsonantButton2[iIndex]->setMenu(m_ptrConsonantsMenu2);
	ptrHLayout->addWidget(m_ptrConsonantButton2[iIndex]);

	return ptrHLayout;
}
void CSubstituteValuesConfigurationDlgPrivate::setupActions()
{
	int iConsonantsCount(stAppSettings.get<int>("settings.consonants.count"));
	//goes through all consonants
	int iIndex(0);
	using boost::property_tree::ptree;

	m_ptrConsonantsActions1[iIndex] = m_ptrConsonantsMenu1->addAction(QString(" "));
	m_ptrConsonantsActions2[iIndex] = m_ptrConsonantsMenu2->addAction(QString(" "));
	iIndex++;
	BOOST_FOREACH(const ptree::value_type &v, stAppSettings.get_child("settings.consonants"))
	{
		if (v.first!="consonant")
			continue;
		m_ptrConsonantsActions1[iIndex] = m_ptrConsonantsMenu1->addAction(QString("&%1").arg(std::string(v.second.data()).data()));
		m_ptrConsonantsActions2[iIndex] = m_ptrConsonantsMenu2->addAction(QString("&%1").arg(std::string(v.second.data()).data()));
		iIndex++;
	}
}
void CSubstituteValuesConfigurationDlgPrivate::setConnections()
{
}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
CSubstituteValuesConfigurationDlg::CSubstituteValuesConfigurationDlg(QWidget * parent)
:QWidget(parent), m_ptrPriv(new CSubstituteValuesConfigurationDlgPrivate(this))
{}
CSubstituteValuesConfigurationDlg::~CSubstituteValuesConfigurationDlg(void){}
void CSubstituteValuesConfigurationDlg::onPushButton1ActionTriggered(QAction *)
{
	printLog(eDebugLogLevel, eDebug, "onPushButton1ActionTriggered:Action in Menu for column 1");
}
void CSubstituteValuesConfigurationDlg::onPushButton2ActionTriggered(QAction *)
{
	printLog(eDebugLogLevel, eDebug, "onPushButton2ActionTriggered:Action in Menu for column 2");
}
