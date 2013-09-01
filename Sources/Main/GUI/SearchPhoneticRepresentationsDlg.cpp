#include <Data/CDataThread.h>
#include <GUI/SearchPhoneticRepresentationsDlg.h>
#include <tools/loggers.h>
#include <QtGui/QLineEdit>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QBoxLayout>
#include <QtGui/QListView>
#include <QtGui/QProgressBar>
#include <QtGui/QStandardItemModel>
#include <Tools/qtTools.h>
#include <QtCore/QMetaType>

//Q_DECLARE_METATYPE (std::string)

class CSearchPhoneticRepresentationsDlgPrivate
{
public:
     CSearchPhoneticRepresentationsDlgPrivate(CSearchPhoneticRepresentationsDlg * ptrPublic);
     ~CSearchPhoneticRepresentationsDlgPrivate();
	 void setupUI();
	 void retranstaleUI();
	 void setConnections();
	 void moveSearchResultIntoModel();
public:
	QLabel *									searchedNumberLabel;
	QLineEdit *									searchedNumber;
	QPushButton *								performSearchButton;
	QProgressBar *								searchProgressBar;
	QListView *									searchResultsView;
	QStandardItemModel  						searchResultsModel;

	CSearchPhoneticRepresentationsDlg *         publicClass;
};

CSearchPhoneticRepresentationsDlgPrivate::CSearchPhoneticRepresentationsDlgPrivate(CSearchPhoneticRepresentationsDlg * ptrPublic):publicClass(ptrPublic)
{
	setupUI();
	retranstaleUI();
	setConnections();
	searchResultsView->setModel(&searchResultsModel);
}
CSearchPhoneticRepresentationsDlgPrivate::~CSearchPhoneticRepresentationsDlgPrivate(){}
void CSearchPhoneticRepresentationsDlgPrivate::setupUI()
{
	QVBoxLayout * mainLayout = new QVBoxLayout;
	delete publicClass->layout();
	publicClass->setLayout(mainLayout);

	QHBoxLayout *	editLayout = new QHBoxLayout;
	searchedNumberLabel = new QLabel;
	searchedNumber = new QLineEdit;
	searchedNumber->setInputMask("00000000000000000000");
	searchedNumber->setMaxLength(20);
	performSearchButton = new QPushButton ;

	editLayout->addWidget(searchedNumberLabel);
	editLayout->addWidget(searchedNumber);
	editLayout->addWidget(performSearchButton);

	mainLayout->addLayout(editLayout);

	searchResultsView = new QListView;
	mainLayout->addWidget(searchResultsView);
	searchProgressBar = new QProgressBar;
	searchProgressBar->hide();
	mainLayout->addWidget(searchProgressBar);
}
void CSearchPhoneticRepresentationsDlgPrivate::retranstaleUI()
{
	searchedNumberLabel->setText(publicClass->tr("Enter number to remember"));
	performSearchButton->setText(publicClass->tr("Search"));
}
void CSearchPhoneticRepresentationsDlgPrivate::setConnections()
{
	bool bResult = false;
		
	bResult = QObject::connect(performSearchButton, SIGNAL(clicked ( bool )), 
		publicClass, SLOT(onPerformSearch()));
	logConnection("CSearchPhoneticRepresentationsDlgPrivate::setConnections",
		"'performSearch::clicked' with 'publicClass::onPerformSearch'", 
		bResult);
}
void CSearchPhoneticRepresentationsDlgPrivate::moveSearchResultIntoModel()
{
	searchResultsModel.clear();
	searchResultsModel.setColumnCount(1);
	std::string searchNumber = searchedNumber->text().toStdString();
	const WordSearchResult & result = CDataThread::getInstance()->getSearchResult();
	for(WordSearchResult::const_iterator iter = result.begin();iter!=result.end();iter++)
	{
		if (iter->bFullCoverage==false)
			continue;
		QStandardItem *item = new QStandardItem(QString("%0").arg(iter->word.c_str()));
		searchResultsModel.appendRow(item);
	}
}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
CSearchPhoneticRepresentationsDlg::CSearchPhoneticRepresentationsDlg(QWidget * parent):QWidget(parent),m_ptrPriv(new CSearchPhoneticRepresentationsDlgPrivate(this))
{}
CSearchPhoneticRepresentationsDlg::~CSearchPhoneticRepresentationsDlg(void){}
void CSearchPhoneticRepresentationsDlg::onPerformSearch()
{
	m_ptrPriv->searchProgressBar->show();
	std::string searchNumber = m_ptrPriv->searchedNumber->text().toStdString();
	qRegisterMetaType<std::string>("std::string");
	disableSearchButton();
	Q_EMIT performSearch(searchNumber);
}
void CSearchPhoneticRepresentationsDlg::onSearchProgress(int current, int max)
{
	if (m_ptrPriv->searchProgressBar->maximum()!=max)
		m_ptrPriv->searchProgressBar->setMaximum(max);
	m_ptrPriv->searchProgressBar->setValue(current);
}
void CSearchPhoneticRepresentationsDlg::disableSearchButton()
{
	m_ptrPriv->performSearchButton->setDisabled(true);
}
void CSearchPhoneticRepresentationsDlg::enableSearchButton()
{
	m_ptrPriv->performSearchButton->setEnabled(true);
}
void CSearchPhoneticRepresentationsDlg::searchFinished()
{
	m_ptrPriv->searchProgressBar->hide();
	enableSearchButton();
	m_ptrPriv->moveSearchResultIntoModel();
}
