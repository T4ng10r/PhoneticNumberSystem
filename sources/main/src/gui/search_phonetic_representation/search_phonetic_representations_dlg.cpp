#include <gui/search_phonetic_representation/compose_substitute_sentence_widget.h>
#include <gui/search_phonetic_representation/search_number_lineedit.h>
#include <gui/search_phonetic_representation/search_phonetic_representations_dlg.h>
#include <QBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QListView>
#include <QMetaType>
#include <QProgressBar>
#include <QPushButton>
#include <QStandardItemModel>
#include <QtCore/QTextCodec>
#include <boost/foreach.hpp>
#include <data/data_thread.h>
#include <tools/loggers.h>
#include <tools/qtTools.h>

class CSearchPhoneticRepresentationsDlgPrivate
{
  public:
    CSearchPhoneticRepresentationsDlgPrivate(SearchPhoneticRepresentationsDlg* ptrPublic);
    ~CSearchPhoneticRepresentationsDlgPrivate();
    void setupUI();
    void retranstaleUI();
    void setConnections();
    void moveSearchResultIntoModel();
    void moveSearchResultIntoSubstituteComposer();

  public:
    QLabel*               searchedNumberLabel;
    SearchNumberLineEdit* searchedNumber;
    QPushButton*          performSearchButton;
    QProgressBar*         searchProgressBar;
    QListView*            searchResultsView;
    QStandardItemModel    searchResultsModel;

    SearchPhoneticRepresentationsDlg* publicClass;
    ComposeSubstituteSentenceWidget*   composeSubstituteSentenceWidget;
};

CSearchPhoneticRepresentationsDlgPrivate::CSearchPhoneticRepresentationsDlgPrivate(
    SearchPhoneticRepresentationsDlg* ptrPublic)
    : publicClass(ptrPublic)
{
    setupUI();
    retranstaleUI();
    setConnections();
    searchResultsView->setModel(&searchResultsModel);
}
CSearchPhoneticRepresentationsDlgPrivate::~CSearchPhoneticRepresentationsDlgPrivate() {}
void CSearchPhoneticRepresentationsDlgPrivate::setupUI()
{
    QVBoxLayout* mainLayout = new QVBoxLayout;
    delete publicClass->layout();
    publicClass->setLayout(mainLayout);

    QHBoxLayout* editLayout = new QHBoxLayout;
    searchedNumberLabel     = new QLabel;
    searchedNumber          = new SearchNumberLineEdit;
    performSearchButton     = new QPushButton;

    editLayout->addWidget(searchedNumberLabel);
    editLayout->addWidget(searchedNumber);
    editLayout->addWidget(performSearchButton);

    mainLayout->addLayout(editLayout);

    composeSubstituteSentenceWidget = new ComposeSubstituteSentenceWidget;
    mainLayout->addWidget(composeSubstituteSentenceWidget);
    // composeSubstituteSentenceWidget->reset();

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

    bResult = QObject::connect(performSearchButton, SIGNAL(clicked(bool)), publicClass, SLOT(onPerformSearch()));
    logConnection("CSearchPhoneticRepresentationsDlgPrivate::setConnections",
                  "'performSearch::clicked' with 'publicClass::onPerformSearch'", bResult);

    bResult = QObject::connect(searchedNumber, SIGNAL(returnPressed()), publicClass, SLOT(on_return_pressed()));
    logConnection("CSearchPhoneticRepresentationsDlgPrivate::setConnections",
                  "'performSearch::clicked' with 'publicClass::onPerformSearch'", bResult);
}
void CSearchPhoneticRepresentationsDlgPrivate::moveSearchResultIntoModel()
{
    searchResultsModel.clear();
    searchResultsModel.setColumnCount(1);
    const MatchingWordsList& result = gDataThread->getSearchResult(0);
    QTextCodec*              codec  = gDataThread->get_current_codepage();
    for (MatchingWord success_word : result) {
        if (false == success_word.bFullCoverage)
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
    MatchingWordsList result = gDataThread->getSearchResult(0);
}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
SearchPhoneticRepresentationsDlg::SearchPhoneticRepresentationsDlg(QWidget* parent)
    : QWidget(parent)
    , _pimpl(new CSearchPhoneticRepresentationsDlgPrivate(this))
{
}
SearchPhoneticRepresentationsDlg::~SearchPhoneticRepresentationsDlg(void) {}
void SearchPhoneticRepresentationsDlg::onPerformSearch()
{
    _pimpl->searchProgressBar->show();
    std::string searchNumber = _pimpl->searchedNumber->text().toStdString();
    qRegisterMetaType<std::string>("std::string");
    disableSearchButton();
    _pimpl->composeSubstituteSentenceWidget->setSearchNumber(searchNumber);
    Q_EMIT performSearch(searchNumber);
}
void SearchPhoneticRepresentationsDlg::onSearchProgress(int current, int max)
{
    if (_pimpl->searchProgressBar->maximum() != max)
        _pimpl->searchProgressBar->setMaximum(max);
    _pimpl->searchProgressBar->setValue(current);
}
void SearchPhoneticRepresentationsDlg::disableSearchButton() { _pimpl->performSearchButton->setDisabled(true); }
void SearchPhoneticRepresentationsDlg::enableSearchButton() { _pimpl->performSearchButton->setEnabled(true); }
void SearchPhoneticRepresentationsDlg::searchFinished(bool)
{
    _pimpl->searchProgressBar->hide();
    enableSearchButton();
    _pimpl->moveSearchResultIntoModel();
    _pimpl->moveSearchResultIntoSubstituteComposer();
}
void SearchPhoneticRepresentationsDlg::on_return_pressed()
{
    if (_pimpl->performSearchButton->isEnabled())
        onPerformSearch();
}
