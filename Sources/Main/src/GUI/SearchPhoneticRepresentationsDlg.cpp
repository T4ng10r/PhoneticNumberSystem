#include <Data/CDataThread.h>
#include <GUI/SearchPhoneticRepresentationsDlg.h>
#include <GUI/ComposeSubstituteSentenceWidget.h>
#include <GUI/SearchNumberLineEdit.h>
#include <Tools/loggers.h>
#include <QtCore/QTextCodec>
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
	//composeSubstituteSentenceWidget->reset();

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

	bResult = QObject::connect(searchedNumber, SIGNAL(returnPressed()), 
		publicClass, SLOT(on_return_pressed()));
	logConnection("CSearchPhoneticRepresentationsDlgPrivate::setConnections",
		"'performSearch::clicked' with 'publicClass::onPerformSearch'", 
		bResult);
}
void CSearchPhoneticRepresentationsDlgPrivate::moveSearchResultIntoModel()
{
	searchResultsModel.clear();
	searchResultsModel.setColumnCount(1);
	const WordsList & result = CDataThread::getInstance()->getSearchResult(0);
	QTextCodec * codec = CDataThread::getInstance()->get_current_codepage();
	for(SuccessWord success_word : result)
	{
		if (false==success_word.bFullCoverage)
			continue;
		QString itemString;
		if (codec)
			itemString = codec->toUnicode(success_word.getWord().c_str());
		else
			itemString = success_word.getWord().c_str();
		searchResultsModel.appendRow(new QStandardItem(itemString));
	}
}
void CSearchPhoneticRepresentationsDlgPrivate::moveSearchResultIntoSubstituteComposer()
{
	composeSubstituteSentenceWidget->initialize_after_success_search();
	WordsList result = CDataThread::getInstance()->getSearchResult(0);
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
void CSearchPhoneticRepresentationsDlg::on_return_pressed()
{
	if (m_ptrPriv->performSearchButton->isEnabled())
		onPerformSearch();
}
