#include <GUI/Settings/SubstituteValuesConfigurationDlg.h>
#include <QtCore/QString>
#include <QtGui/QMenu>
#include <QtGui/QAction>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QBoxLayout>
#include <tools/loggers.h>

//Qt::ContextMenuPolicy
//Qt::CustomContextMenu
//QWidget::actions()
// 
char aConsonats[] = {'B', 'C', 'D', 'F', 'G', 'H', 'J', 'K', 'L', 'M', 'N', 
'P', 'Q', 'R', 'S', 'T', 'V', 'X', 'Z', 'W'};
enum { ConsonantsMax = sizeof(aConsonats) };
enum { DigitsCount = 10 };

class CSubstituteValuesConfigurationDlgPrivate
{
public:
	CSubstituteValuesConfigurationDlgPrivate(CSubstituteValuesConfigurationDlg * ptrPublic);
	~CSubstituteValuesConfigurationDlgPrivate();
	void setupUI();
	QHBoxLayout * setupUI_line(int iIndex);
	void setupActions();
public:
	CSubstituteValuesConfigurationDlg *		m_ptrPublic;
	QAction *								m_ptrConsonantsActions[ConsonantsMax];
	QMenu *									m_ptrConsonantsMenu;
	QLabel *								m_ptrConsonantLabel[DigitsCount];
	QPushButton *							m_ptrConsonantButton1[DigitsCount];
	QPushButton *							m_ptrConsonantButton2[DigitsCount];
};

CSubstituteValuesConfigurationDlgPrivate::CSubstituteValuesConfigurationDlgPrivate(CSubstituteValuesConfigurationDlg * ptrPublic):m_ptrPublic(ptrPublic)
{
	setupUI();
	setupActions();
}
CSubstituteValuesConfigurationDlgPrivate::~CSubstituteValuesConfigurationDlgPrivate(){}
void CSubstituteValuesConfigurationDlgPrivate::setupUI()
{
	QVBoxLayout * ptrMainLayout = new QVBoxLayout;
	delete m_ptrPublic->layout();
	m_ptrPublic->setLayout(ptrMainLayout);

	for(int i=0;i<DigitsCount;i++)
	{
		ptrMainLayout->addLayout(setupUI_line(i));
	}
}
QHBoxLayout * CSubstituteValuesConfigurationDlgPrivate::setupUI_line(int iIndex)
{
	QHBoxLayout * ptrHLayout = new QHBoxLayout;
	m_ptrConsonantLabel[iIndex] = new QLabel;
	ptrHLayout->addWidget(m_ptrConsonantLabel[iIndex]);
	m_ptrConsonantButton1[DigitsCount] = new QPushButton;
	ptrHLayout->addWidget(m_ptrConsonantButton1[iIndex]);
	m_ptrConsonantButton2[DigitsCount] = new QPushButton;
	ptrHLayout->addWidget(m_ptrConsonantButton1[iIndex]);

	return ptrHLayout;
}
void CSubstituteValuesConfigurationDlgPrivate::setupActions()
{
	m_ptrConsonantsMenu = new QMenu;
	for(int a=0;a<ConsonantsMax;a++)
	{
		m_ptrConsonantsActions[a] = m_ptrConsonantsMenu->addAction(QString("&%1").arg(aConsonats));
		m_ptrConsonantsActions[a]->setVisible(true);
	}
}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
CSubstituteValuesConfigurationDlg::CSubstituteValuesConfigurationDlg(QWidget * parent)
:QWidget(parent), m_ptrPriv(new CSubstituteValuesConfigurationDlgPrivate(this))
{}
CSubstituteValuesConfigurationDlg::~CSubstituteValuesConfigurationDlg(void){}
