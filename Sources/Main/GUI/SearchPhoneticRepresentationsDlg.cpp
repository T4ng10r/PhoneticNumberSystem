#include <Data/CDataThread.h>
#include <GUI/SearchPhoneticRepresentationsDlg.h>
#include <GUI/ComposeSubstituteSentenceWidget.h>
#include <GUI/SearchNumberLineEdit.h>
#include <tools/loggers.h>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QBoxLayout>
#include <QListView>
#include <QProgressBar>
#include <QStandardItemModel>
#include <Tools/qtTools.h>
#include <QMetaType>
#include <boost/foreach.hpp>

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
	 void moveSearchResultIntoSubstituteComposer();
public:
	QLabel *									searchedNumberLabel;
	SearchNumberLineEdit *						searchedNumber;
	QPushButton *								performSearchButton;
	QProgressBar *								searchProgressBar;
	QListView *									searchResultsView;
	QStandardItemModel  						searchResultsModel;

	CSearchPhoneticRepresentationsDlg *         publicClass;
	ComposeSubstituteSentenceWidget * composeSubstituteSentenceWidget;
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
	searchedNumber = new SearchNumberLineEdit;
	performSearchButton = new QPushButton ;

	editLayout->addWidget(searchedNumberLabel);
	editLayout->addWidget(searchedNumber);
	editLayout->addWidget(performSearchButton);

	mainLayout->addLayout(editLayout);

	composeSubstituteSentenceWidget = new ComposeSubstituteSentenceWidget;
	mainLayout->addWidget(composeSubstituteSentenceWidget);
	composeSubstituteSentenceWidget->reset();

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
		std::string itemString;
        BOOST_FOREACH(const std::string & word, iter->words)
        //for(const std::string & word : iter->words)
		{
			if (!itemString.empty())
				itemString.push_back(' ');
			itemString+=word;
		}
		//QStandardItem *item = new QStandardItem(QString("%0").arg(iter->words.front().c_str()));
		QStandardItem *item = new QStandardItem(QString("%0").arg(itemString.c_str()));
		searchResultsModel.appendRow(item);
	}
}
void CSearchPhoneticRepresentationsDlgPrivate::moveSearchResultIntoSubstituteComposer()
{
	WordSearchResult result = CDataThread::getInstance()->getSearchResult();
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
void CSearchPhoneticRepresentationsDlg::searchFinished(bool)
{
	m_ptrPriv->searchProgressBar->hide();
	enableSearchButton();
	m_ptrPriv->moveSearchResultIntoModel();
	m_ptrPriv->moveSearchResultIntoSubstituteComposer();
}
