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

class CSearchPhoneticRepresentationsDlgPrivate
{
public:
     CSearchPhoneticRepresentationsDlgPrivate(CSearchPhoneticRepresentationsDlg * ptrPublic);
     ~CSearchPhoneticRepresentationsDlgPrivate();
	 void setupUI();
	 void retranstaleUI();
	 void setConnections();
public:
	QLabel *									searchedNumberLabel;
	QLineEdit *									searchedNumber;
	QPushButton *								performSearch;
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
	performSearch = new QPushButton ;

	editLayout->addWidget(searchedNumberLabel);
	editLayout->addWidget(searchedNumber);
	editLayout->addWidget(performSearch);

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
	performSearch->setText(publicClass->tr("Search"));
}
void CSearchPhoneticRepresentationsDlgPrivate::setConnections()
{
	bool bResult = false;
		
	bResult = QObject::connect(performSearch, SIGNAL(clicked ( bool )), 
		publicClass, SLOT(onPerformSearch()));
	logConnection("CSearchPhoneticRepresentationsDlgPrivate::setConnections",
		"'performSearch::clicked' with 'publicClass::onPerformSearch'", 
		bResult);
}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
CSearchPhoneticRepresentationsDlg::CSearchPhoneticRepresentationsDlg(QWidget * parent):QWidget(parent),m_ptrPriv(new CSearchPhoneticRepresentationsDlgPrivate(this))
{}
CSearchPhoneticRepresentationsDlg::~CSearchPhoneticRepresentationsDlg(void){}
void CSearchPhoneticRepresentationsDlg::onPerformSearch()
{
	m_ptrPriv->searchProgressBar->show();
}