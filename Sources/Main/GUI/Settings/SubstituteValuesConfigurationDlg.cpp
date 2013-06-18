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
#include <boost/shared_ptr.hpp>

enum { DigitsCount = 10 };
const unsigned int cPushButtonWidth(50);
//each push button should have list of available consonants
//already selected consonants (in other buttons) shall be hidenn or inactive

struct EntryLine
{
	QLabel *				m_ptrConsonantLabel;		//label with digit name
	QPushButton *			m_ptrConsonantButton1;		//button for first consonant used in substitution
	QPushButton *			m_ptrConsonantButton2;		//button for second consonant used in substitution
	QMenu *					m_ptrConsonantsMenu1;		//menu on first substitution button
	QMenu *					m_ptrConsonantsMenu2;		//menu on second substitution button
	QActionGroup *			m_ptrActionGroup1;			//grouping consonant actions from first button
	QActionGroup *			m_ptrActionGroup2;			//grouping consonant actions from second button
	std::vector<QAction *>	m_ptrConsonantsActions1;	//actions-consonants under first button
	std::vector<QAction *>	m_ptrConsonantsActions2;	//actions-consonants under second button
};

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
	boost::property_tree::ptree				m_stProperties;
	CSubstituteValuesConfigurationDlg *		m_ptrPublic;
	std::vector<EntryLine>					m_ptrDigitsEntries;
	std::map<char, std::vector<QAction *> > m_mActionsList;
};

CSubstituteValuesConfigurationDlgPrivate::CSubstituteValuesConfigurationDlgPrivate(CSubstituteValuesConfigurationDlg * ptrPublic):
m_ptrPublic(ptrPublic)
{
}
CSubstituteValuesConfigurationDlgPrivate::~CSubstituteValuesConfigurationDlgPrivate(){}
void CSubstituteValuesConfigurationDlgPrivate::setupUI()
{
	QVBoxLayout * ptrMainLayout = new QVBoxLayout;
	delete m_ptrPublic->layout();
	m_ptrPublic->setLayout(ptrMainLayout);

	m_ptrDigitsEntries.resize(DigitsCount);

	for(int i=0;i<DigitsCount;i++)
	{
		ptrMainLayout->addLayout(setupUI_line(i));
	}
}
QHBoxLayout * CSubstituteValuesConfigurationDlgPrivate::setupUI_line(int iIndex)
{
	bool bResult = false;
	EntryLine & stEntryLine = m_ptrDigitsEntries[iIndex];

	QHBoxLayout * ptrHLayout = new QHBoxLayout;
	stEntryLine.m_ptrActionGroup1 = new QActionGroup(m_ptrPublic);
	stEntryLine.m_ptrActionGroup2 = new QActionGroup(m_ptrPublic);

	stEntryLine.m_ptrConsonantLabel = new QLabel;
	std::string strIndex = boost::lexical_cast<std::string>(iIndex);
	stEntryLine.m_ptrConsonantLabel->setText(strIndex.data());
	ptrHLayout->addWidget(stEntryLine.m_ptrConsonantLabel);
	stEntryLine.m_ptrConsonantButton1 = new QPushButton;
	stEntryLine.m_ptrConsonantButton1->setFixedWidth(cPushButtonWidth);

	stEntryLine.m_ptrConsonantsMenu1 = new QMenu;
	stEntryLine.m_ptrConsonantButton1->setMenu(stEntryLine.m_ptrConsonantsMenu1);
	stEntryLine.m_ptrConsonantButton1->setFlat(true);
	ptrHLayout->addWidget(stEntryLine.m_ptrConsonantButton1);

	stEntryLine.m_ptrConsonantButton2 = new QPushButton;
	stEntryLine.m_ptrConsonantButton2->setFixedWidth(cPushButtonWidth);
	stEntryLine.m_ptrConsonantButton2->setFlat(true);
	stEntryLine.m_ptrConsonantsMenu2 = new QMenu;
	stEntryLine.m_ptrConsonantButton2->setMenu(stEntryLine.m_ptrConsonantsMenu2);
	ptrHLayout->addWidget(stEntryLine.m_ptrConsonantButton2);

	bResult = QObject::connect(stEntryLine.m_ptrConsonantsMenu1, SIGNAL(triggered ( QAction * )), 
		m_ptrPublic, SLOT(onMenuTriggered(QAction *)));
	logConnection("CSubstituteValuesConfigurationDlgPrivate::setupUI_line",
		"'m_ptrConsonantsMenu1::triggered' with 'm_ptrPublic::onMenuTriggered'", 
		bResult);
	bResult = QObject::connect(stEntryLine.m_ptrConsonantsMenu2, SIGNAL(triggered ( QAction * )), 
		m_ptrPublic, SLOT(onMenuTriggered(QAction *)));
	logConnection("CSubstituteValuesConfigurationDlgPrivate::setupUI_line",
		"'m_ptrConsonantsMenu2::triggered' with 'm_ptrPublic::onMenuTriggered'", 
		bResult);

	return ptrHLayout;
}
void CSubstituteValuesConfigurationDlgPrivate::setupActions()
{
	using boost::property_tree::ptree;
	//int iConsonantsCount(stAppSettings.get<int>("settings.consonants.count"));
	m_stProperties.size();
	ptree::iterator iter1 = m_stProperties.begin();
	iter1->first.data();
	iter1->second.data();
	m_stProperties.get_child("count");
	int iConsonantsCount(m_stProperties.get_child("count").get_value<int>());
	bool bResult = false;

	BOOST_FOREACH(EntryLine & stEntry, m_ptrDigitsEntries)
	{
		QAction * ptrConsonantEmptyAction1 = new QAction(QString(" "), NULL);
		m_mActionsList[' '].push_back(ptrConsonantEmptyAction1);
		stEntry.m_ptrConsonantsMenu1->addAction(ptrConsonantEmptyAction1);
		ptrConsonantEmptyAction1->setCheckable(true);
		ptrConsonantEmptyAction1->setChecked(true);
		stEntry.m_ptrActionGroup1->addAction(ptrConsonantEmptyAction1);
		bResult = QObject::connect(ptrConsonantEmptyAction1, SIGNAL(toggled ( bool )), 
			m_ptrPublic, SLOT(onActionToggled(bool)));
		logConnection("CSubstituteValuesConfigurationDlgPrivate::setupActions",
			"'ptrConsonantEmptyAction1::triggered' with 'm_ptrPublic::onActionToggled'", 
			bResult);

		QAction * ptrConsonantEmptyAction2 = new QAction(QString(" "), NULL);
		m_mActionsList[' '].push_back(ptrConsonantEmptyAction2);
		stEntry.m_ptrConsonantsMenu2->addAction(ptrConsonantEmptyAction2);
		ptrConsonantEmptyAction2->setCheckable(true);
		ptrConsonantEmptyAction2->setChecked(true);
		stEntry.m_ptrActionGroup2->addAction(ptrConsonantEmptyAction2);
		bResult = QObject::connect(ptrConsonantEmptyAction2, SIGNAL(toggled ( bool )), 
			m_ptrPublic, SLOT(onActionToggled(bool)));
		logConnection("CSubstituteValuesConfigurationDlgPrivate::setupActions",
			"'ptrConsonantEmptyAction2::triggered' with 'm_ptrPublic::onActionToggled'", 
			bResult);
	}
	BOOST_FOREACH(const ptree::value_type &singleEntry, m_stProperties/*.get_child("")*/)
	{
		if (singleEntry.first!="consonant")
			continue;
		char cConsonant(singleEntry.second.data().at(0));
		BOOST_FOREACH(EntryLine & stEntry, m_ptrDigitsEntries)
		{
			QAction * ptrConsonantAction1 = new QAction(QString("&%1").arg(cConsonant), NULL);
			m_mActionsList[cConsonant].push_back(ptrConsonantAction1);
			stEntry.m_ptrConsonantsMenu1->addAction(ptrConsonantAction1);
			ptrConsonantAction1->setCheckable(true);
			stEntry.m_ptrActionGroup1->addAction(ptrConsonantAction1);
			bResult = QObject::connect(ptrConsonantAction1, SIGNAL(toggled ( bool )), 
				m_ptrPublic, SLOT(onActionToggled(bool)));
			logConnection("CSubstituteValuesConfigurationDlgPrivate::setupActions",
				"'ptrConsonantAction1::triggered' with 'm_ptrPublic::onActionToggled'", 
				bResult);

			QAction * ptrConsonantAction2 = new QAction(QString("&%1").arg(cConsonant), NULL);
			m_mActionsList[cConsonant].push_back(ptrConsonantAction2);
			stEntry.m_ptrConsonantsMenu2->addAction(ptrConsonantAction2);
			ptrConsonantAction2->setCheckable(true);
			stEntry.m_ptrActionGroup2->addAction(ptrConsonantAction2);
			bResult = QObject::connect(ptrConsonantAction2, SIGNAL(toggled ( bool )), 
				m_ptrPublic, SLOT(onActionToggled(bool)));
			logConnection("CSubstituteValuesConfigurationDlgPrivate::setupActions",
				"'ptrConsonantAction2::triggered' with 'm_ptrPublic::onActionToggled'", 
				bResult);
		}
	}
}
void CSubstituteValuesConfigurationDlgPrivate::setConnections()
{
}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
CSubstituteValuesConfigurationDlg::CSubstituteValuesConfigurationDlg(const boost::property_tree::ptree &stProperties)
:QWidget(NULL), m_ptrPriv(new CSubstituteValuesConfigurationDlgPrivate(this))
{
	m_ptrPriv->m_stProperties=stProperties;
	m_ptrPriv->setupUI();
	m_ptrPriv->setupActions();
	m_ptrPriv->setConnections();
}
CSubstituteValuesConfigurationDlg::~CSubstituteValuesConfigurationDlg(void){}
void CSubstituteValuesConfigurationDlg::onMenuTriggered(QAction * pAction )
{
	QObject * pObject = sender();
	QMenu * ptrMenu = static_cast<QMenu*>(sender());
	int iIndex;
	char cConsonant;
	iIndex = pAction->text().length()>1?1:0;
	cConsonant = pAction->text().at(iIndex).toAscii();

	BOOST_FOREACH(EntryLine & stEntry, m_ptrPriv->m_ptrDigitsEntries)
		if (stEntry.m_ptrConsonantsMenu1==ptrMenu)
		{
			stEntry.m_ptrConsonantButton1->setText(QString(cConsonant));
			break;
		}
		else if (stEntry.m_ptrConsonantsMenu2==ptrMenu)
		{
			stEntry.m_ptrConsonantButton2->setText(QString(cConsonant));
			break;
		}
}
void CSubstituteValuesConfigurationDlg::onActionToggled(bool bState)
{
	QAction * pAction = static_cast<QAction*>(sender());
	QMenu * ptrMenu = pAction->menu();
	int iIndex;
	char cConsonant;
	iIndex = pAction->text().length()>1?1:0;
	cConsonant = pAction->text().at(iIndex).toAscii();
	BOOST_FOREACH(QAction * pConsonantAction, m_ptrPriv->m_mActionsList[cConsonant])
		if (pConsonantAction!=pAction && cConsonant!=' ')
		{
			pConsonantAction->setDisabled(bState);
		}
}
