#include <GUI/Settings/SubstituteValuesConfigurationDlg.h>
#include <Data/CAppSettings.h>
#include <Data/CAppSettingsKeywords.h>
#include <Data/CAppSettings_XMLKeywords.h>
#include <QString>
#include <QMenu>
#include <QAction>
#include <QLabel>
#include <QPushButton>
#include <QBoxLayout>
#include <QComboBox>
#include <Tools/qtTools.h>
#include <Tools/loggers.h>
#include <boost/lexical_cast.hpp>
#include <boost/foreach.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/format.hpp>

enum { DigitsCount = 10 };
const unsigned int cPushButtonWidth(50);
#define ACTION_QUICK_SELECTOR '&'
const QString empty_system_name("");
//each push button should have list of available consonants
//already selected consonants (in other buttons) shall be hidden or inactive

struct EntryLine
{
	QLabel *				m_ptrConsonantLabel;		          //label with digit name
	QPushButton *			m_ptrConsonantButton1;		      //button for first consonant used in substitution
	QPushButton *			m_ptrConsonantButton2;		      //button for second consonant used in substitution
	QMenu *					m_ptrConsonantsMenu1;		          //menu on first substitution button
	QMenu *					m_ptrConsonantsMenu2;		          //menu on second substitution button
	QActionGroup *			m_ptrActionGroup1;			      //grouping consonant actions from first button
	QActionGroup *			m_ptrActionGroup2;			      //grouping consonant actions from second button
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
	void setConfigurations();

	QAction *  createConsonantAction(char consonant, bool bChecked);
	void createAndAddConsonantActionOnBothSides( EntryLine &stEntry, char consonant, bool initialiyChecked );
	void setConnectionForConsonantAction( QAction * emptyAction );
	void setConnections();
	void addCreatedConsonantAction(EntryLine &stEntry, QAction * emptyAction, bool bAddToFirstGroup);
	void fillGUIWithDigitsSystem(const CSingleSubstituteDigitsConfiguration & digitsSystem);
	void selectConsonantActionByGivenConsonant( std::vector<QAction *>& actionsList, const char consonant);
	void selectDigitsSystemByName(QString system_name);

public:
	boost::property_tree::ptree				consonants_values_set;
  //boost::property_tree::ptree				digits_substistute_configuration;
  std::vector<CSingleSubstituteDigitsConfiguration>   digits_substistute_configuration;
	CSubstituteValuesConfigurationDlg *		ptrPublic;
	std::vector<EntryLine>					m_ptrDigitsEntries;
	std::map<char, std::vector<QAction *> > m_mActionsList;
	QComboBox *		m_ptrSystemsCombo;
	QLabel *		m_ptrSystemsLabel;
};

CSubstituteValuesConfigurationDlgPrivate::CSubstituteValuesConfigurationDlgPrivate(CSubstituteValuesConfigurationDlg * ptrPublic):
ptrPublic(ptrPublic)
{
}
CSubstituteValuesConfigurationDlgPrivate::~CSubstituteValuesConfigurationDlgPrivate(){}
void CSubstituteValuesConfigurationDlgPrivate::setupUI()
{
	QVBoxLayout * ptrMainLayout = new QVBoxLayout;
	delete ptrPublic->layout();
	ptrPublic->setLayout(ptrMainLayout);

	QHBoxLayout *	m_ptrComboLayout = new QHBoxLayout;
	m_ptrSystemsCombo = new QComboBox;
	m_ptrSystemsLabel = new QLabel;
	m_ptrSystemsLabel->setText("System configuration");
	m_ptrComboLayout->addWidget(m_ptrSystemsLabel);
	m_ptrComboLayout->addWidget(m_ptrSystemsCombo);
	ptrMainLayout->addLayout(m_ptrComboLayout);

	m_ptrDigitsEntries.resize(DigitsCount);

	for(int i=0;i<DigitsCount;i++)
	{
		ptrMainLayout->addLayout(setupUI_line(i));
	}
	setupActions();
	setConnections();
	setConfigurations();

}
void CSubstituteValuesConfigurationDlgPrivate::setConfigurations()
{
	m_ptrSystemsCombo->disconnect(ptrPublic); 
	m_ptrSystemsCombo->addItem(empty_system_name);
	const std::vector<CSingleSubstituteDigitsConfiguration> & vDigitsConf = gAppSettings->getDigitsConfiguraions();
	BOOST_FOREACH(const CSingleSubstituteDigitsConfiguration & digitsConf, vDigitsConf)
		m_ptrSystemsCombo->addItem(digitsConf.strName.c_str());
  setConnections();
}
QHBoxLayout * CSubstituteValuesConfigurationDlgPrivate::setupUI_line(int iIndex)
{
	bool bResult = false;
	EntryLine & stEntryLine = m_ptrDigitsEntries[iIndex];

	QHBoxLayout * ptrHLayout = new QHBoxLayout;
	stEntryLine.m_ptrActionGroup1 = new QActionGroup(ptrPublic);
	stEntryLine.m_ptrActionGroup2 = new QActionGroup(ptrPublic);

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
	stEntryLine.m_ptrConsonantButton2->setEnabled(false);
	ptrHLayout->addWidget(stEntryLine.m_ptrConsonantButton2);

	bResult = QObject::connect(stEntryLine.m_ptrConsonantsMenu1, SIGNAL(triggered ( QAction * )), 
		ptrPublic, SLOT(onMenuTriggered_SetButtonTextWithSelectedConsonant(QAction *)));
	logConnection("CSubstituteValuesConfigurationDlgPrivate::setupUI_line",
		"'m_ptrConsonantsMenu1::triggered' with 'm_ptrPublic::onMenuTriggered'", 
		bResult);
	bResult = QObject::connect(stEntryLine.m_ptrConsonantsMenu2, SIGNAL(triggered ( QAction * )), 
		ptrPublic, SLOT(onMenuTriggered_SetButtonTextWithSelectedConsonant(QAction *)));
	logConnection("CSubstituteValuesConfigurationDlgPrivate::setupUI_line",
		"'m_ptrConsonantsMenu2::triggered' with 'm_ptrPublic::onMenuTriggered'", 
		bResult);

	return ptrHLayout;
}
void CSubstituteValuesConfigurationDlgPrivate::setupActions()
{
	using boost::property_tree::ptree;
	int iConsonantsCount(consonants_values_set.get_child("count").get_value<int>());
	bool bResult = false;

	BOOST_FOREACH(EntryLine & stEntry, m_ptrDigitsEntries)
		createAndAddConsonantActionOnBothSides(stEntry, ' ', true);
	BOOST_FOREACH(const ptree::value_type &singleDigitsEntry, consonants_values_set)
	{
		if (singleDigitsEntry.first!="consonant")
			continue;
		char consonant(singleDigitsEntry.second.data().at(0));
		BOOST_FOREACH(EntryLine & stEntry, m_ptrDigitsEntries)
			createAndAddConsonantActionOnBothSides(stEntry, consonant, false);
	}
}
void CSubstituteValuesConfigurationDlgPrivate::setConnections()
{
	bool bResult = QObject::connect(m_ptrSystemsCombo, SIGNAL(currentIndexChanged   ( const QString )), 
		ptrPublic, SLOT(onSystemsActvivated_changeCurrentDigitsSystem(const QString&)));
	logConnection("CSubstituteValuesConfigurationDlgPrivate::setupActions",
		"'emptyAction::triggered' with 'm_ptrPublic::onActionToggled'", 
		bResult);
	;
}
QAction *  CSubstituteValuesConfigurationDlgPrivate::createConsonantAction(char consonant, bool bChecked) 
{
	QString actionName(consonant);
	if (' '!=consonant)
		actionName.push_front(ACTION_QUICK_SELECTOR);
	QAction * action = new QAction(actionName, NULL);
	m_mActionsList[consonant].push_back(action);

	action->setCheckable(true);
	action->setChecked(bChecked);
	setConnectionForConsonantAction(action);
	return action;
}
void CSubstituteValuesConfigurationDlgPrivate::createAndAddConsonantActionOnBothSides( EntryLine &stEntry, char consonant, bool initialiyChecked )
{
	QAction * emptyAction = createConsonantAction(consonant, initialiyChecked);
	addCreatedConsonantAction(stEntry, emptyAction, true);
	emptyAction = createConsonantAction(consonant, initialiyChecked);
	addCreatedConsonantAction(stEntry, emptyAction, false);
}
void CSubstituteValuesConfigurationDlgPrivate::addCreatedConsonantAction( EntryLine &stEntry, QAction * emptyAction, bool bAddToFirstGroup )
{
	if (bAddToFirstGroup)
	{
		stEntry.m_ptrConsonantsMenu1->addAction(emptyAction);
		stEntry.m_ptrActionGroup1->addAction(emptyAction);
		stEntry.m_ptrConsonantsActions1.push_back(emptyAction);
	}
	else
	{
		stEntry.m_ptrConsonantsMenu2->addAction(emptyAction);
		stEntry.m_ptrActionGroup2->addAction(emptyAction);
		stEntry.m_ptrConsonantsActions2.push_back(emptyAction);
	}
}
void CSubstituteValuesConfigurationDlgPrivate::setConnectionForConsonantAction( QAction * emptyAction ) 
{
	bool bResult = QObject::connect(emptyAction, SIGNAL(toggled ( bool )), 
		ptrPublic, SLOT(onActionToggled_DeactivateThisConsonantInOtherMenus(bool)));
	logConnection("CSubstituteValuesConfigurationDlgPrivate::setupActions",
		"'emptyAction::triggered' with 'm_ptrPublic::onActionToggled'", 
		bResult);
}
void CSubstituteValuesConfigurationDlgPrivate::fillGUIWithDigitsSystem(const CSingleSubstituteDigitsConfiguration & digitsSystem)
{
	for(SystemMap::const_iterator iter = digitsSystem.mSystem.begin();iter!=digitsSystem.mSystem.end();iter++)
	{
		EntryLine & entry = m_ptrDigitsEntries[iter->first];
		selectConsonantActionByGivenConsonant(entry.m_ptrConsonantsActions1, iter->second.first);
		selectConsonantActionByGivenConsonant(entry.m_ptrConsonantsActions2, iter->second.second);
	}
}
void CSubstituteValuesConfigurationDlgPrivate::selectConsonantActionByGivenConsonant( std::vector<QAction *>& actionsList, const char consonant) 
{
	for(size_t i=0;i<actionsList.size();i++)
	{
		QString actionText = actionsList[i]->text();
		actionText = actionText.remove(ACTION_QUICK_SELECTOR);
		if (consonant == actionText.at(0).toLatin1())
		{
			actionsList[i]->trigger();
			break;
		}
	}
}
void CSubstituteValuesConfigurationDlgPrivate::selectDigitsSystemByName(QString system_name)
{
	int index = m_ptrSystemsCombo->findText(system_name);
	if (index<0)
		return;
	m_ptrSystemsCombo->setCurrentIndex(index);
}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
CSubstituteValuesConfigurationDlg::CSubstituteValuesConfigurationDlg(CSubstituteValuesConfigurationDlgInit init_data)
:QWidget(NULL), priv_part(new CSubstituteValuesConfigurationDlgPrivate(this))
{
  priv_part->consonants_values_set=init_data.consonants_values_set;
	priv_part->digits_substistute_configuration=init_data.digits_substistute_configuration;
	priv_part->setupUI();
  priv_part->selectDigitsSystemByName(init_data.selected_system_name);
}
CSubstituteValuesConfigurationDlg::~CSubstituteValuesConfigurationDlg(void){}
void CSubstituteValuesConfigurationDlg::onMenuTriggered_SetButtonTextWithSelectedConsonant(QAction * pAction )
{
	QObject * pObject = sender();
	QMenu * ptrMenu = static_cast<QMenu*>(sender());
	int iIndex(pAction->text().length()>1?1:0);
	char cConsonant(pAction->text().at(iIndex).toLatin1());

	//check to which Digit Entry belongs this action
	BOOST_FOREACH(EntryLine & stEntry, priv_part->m_ptrDigitsEntries)
		if (stEntry.m_ptrConsonantsMenu1==ptrMenu)
		{
			stEntry.m_ptrConsonantButton1->setText(QString(cConsonant));
			stEntry.m_ptrConsonantButton2->setEnabled(cConsonant!=' ');
			return;
		}
		else if (stEntry.m_ptrConsonantsMenu2==ptrMenu)
		{
			stEntry.m_ptrConsonantButton2->setText(QString(cConsonant));
			return;
		}
}
void CSubstituteValuesConfigurationDlg::onActionToggled_DeactivateThisConsonantInOtherMenus(bool bState)
{
	QAction * pAction = static_cast<QAction*>(sender());
	QMenu * ptrMenu = pAction->menu();
	int iIndex;
	char cConsonant;
	iIndex = pAction->text().length()>1?1:0;
	cConsonant = pAction->text().at(iIndex).toLatin1();
	BOOST_FOREACH(QAction * pConsonantAction, priv_part->m_mActionsList[cConsonant])
		if (pConsonantAction!=pAction && cConsonant!=' ')
		{
			pConsonantAction->setDisabled(bState);
		}
}
void CSubstituteValuesConfigurationDlg::onSystemsActvivated_changeCurrentDigitsSystem(const QString& selectedSystemName)
{
	if (selectedSystemName==empty_system_name)
	{
		CSingleSubstituteDigitsConfiguration digitsConf;
		digitsConf.create_empty_system();
		priv_part->fillGUIWithDigitsSystem(digitsConf);
		Q_EMIT set_selected_consonant_system(selectedSystemName);
		return;
	}
	BOOST_FOREACH(const CSingleSubstituteDigitsConfiguration & digitsConf, priv_part->digits_substistute_configuration)
		if (digitsConf.strName.c_str()==selectedSystemName)
		{
			priv_part->fillGUIWithDigitsSystem(digitsConf);
			Q_EMIT set_selected_consonant_system(selectedSystemName);
			return;
		}
	Q_EMIT set_selected_consonant_system(empty_system_name);
	printLog(eDebug, eErrorLogLevel, str(boost::format("Couldn't find '%1%' consonants configuration in SelectionList") % 
	    selectedSystemName.toStdString()));
}
