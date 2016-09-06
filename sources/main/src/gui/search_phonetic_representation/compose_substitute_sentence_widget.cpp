#include <gui/search_phonetic_representation/compose_substitute_sentence_widget.h>
#include <gui/search_phonetic_representation/substitute_search_result_combobox.h>
#include <gui/search_phonetic_representation/word_result_widget.h>
#include <boost/format.hpp>
#include <data/data_thread.h>
#include <data/logging_base.h>
#include <tools/loggers.h>
#include <tools/qtTools.h>

#include <QBoxLayout>
#include <QSettings>
#include <QComboBox>
#include <algorithm>
#include <QCoreApplication>
#include <QPushButton>
// Q_DECLARE_METATYPE(MatchingWord);

bool cmp_success_words(const MatchingWord& first, const MatchingWord& second)
{
    if (first.matchingLetters.size() > second.matchingLetters.size())
        return true;
    if (first.matchingLetters.size() == second.matchingLetters.size() && first.words.front() < second.words.front())
        return true;
    return false;
}

class ComposeSubstituteSentenceWidgetPrivate : public LoggingBase
{
  public:
    ComposeSubstituteSentenceWidgetPrivate(ComposeSubstituteSentenceWidget* ptrPublic);
    ~ComposeSubstituteSentenceWidgetPrivate();
    void setup_ui();
    void add_combo_box();
    void reset(std::size_t starting_index = 1);
    void    set_connections();
    QString process_word(MatchingWord word, QTextCodec* codec);
    std::size_t which_matching_word_selected();
    void set_subtitute();

  public:
    ComposeSubstituteSentenceWidget* public_part;
    std::vector<WordResultWidget*>   word_results_container;
    QHBoxLayout*                     compose_layout;
    std::string                      searchNumber;
    QPushButton*                     save_subtitution;
};

ComposeSubstituteSentenceWidgetPrivate::ComposeSubstituteSentenceWidgetPrivate(
    ComposeSubstituteSentenceWidget* ptrPublic)
    : public_part(ptrPublic)
{
    setup_ui();
    set_connections();
}
ComposeSubstituteSentenceWidgetPrivate::~ComposeSubstituteSentenceWidgetPrivate() { reset(); }
void ComposeSubstituteSentenceWidgetPrivate::add_combo_box()
{

    WordResultWidget* result_widget = new WordResultWidget();
    word_results_container.push_back(result_widget);
    compose_layout->addWidget(result_widget);

    bool bResult = false;
    bResult = QObject::connect(result_widget, SIGNAL(word_selected(int)), public_part, SLOT(on_word_selected(int)));
    logConnection("ComposeSubstituteSentenceWidgetPrivate::add_combo_box",
                  "'result_widget::word_selected' with 'public_part::on_word_selected'", bResult);
}

void ComposeSubstituteSentenceWidgetPrivate::setup_ui()
{
    QHBoxLayout* main_layout = new QHBoxLayout;
    delete public_part->layout();
    public_part->setLayout(main_layout);

    compose_layout = new QHBoxLayout;
    main_layout->addLayout(compose_layout);
    main_layout->addStretch(5);
    save_subtitution = new QPushButton();
    main_layout->addWidget(save_subtitution);
    save_subtitution->setText("Save");
    save_subtitution->setEnabled(false);
    add_combo_box();
}
void ComposeSubstituteSentenceWidgetPrivate::reset(std::size_t starting_index)
{
    if (starting_index > word_results_container.size())
        starting_index = word_results_container.size() - 1;
    for (std::size_t i = starting_index; i < word_results_container.size(); i++) {
        WordResultWidget* result_widget = word_results_container.back();
        compose_layout->removeWidget(result_widget);
        delete result_widget;
        word_results_container.pop_back();
    }
}
void ComposeSubstituteSentenceWidgetPrivate::set_connections()
{
    bool bResult = false;
    bResult = QObject::connect(save_subtitution, SIGNAL(clicked(bool)),
        public_part, SLOT(on_save_clicked(bool)));
//    slot_logger.log(log4cplus::INFO_LOG_LEVEL,
//     logConnection("CMainWindowPrivate::setConnections",
//        "'CMainWindowPrivate::triggered' with 'CMainWindow::onActionTrigger'",
//        bResult);
}
std::size_t ComposeSubstituteSentenceWidgetPrivate::which_matching_word_selected()
{
    WordResultWidget* sender_                  = dynamic_cast<WordResultWidget*>(public_part->sender());
    std::size_t       combo_box_starting_index = 1;
    for (WordResultWidget* result_widget : word_results_container) {
        if (result_widget != sender_)
            combo_box_starting_index++;
        else
            break;
    }
    return combo_box_starting_index;
}
void ComposeSubstituteSentenceWidgetPrivate::set_subtitute()
{
    QSettings userData(QSettings::UserScope, QCoreApplication::applicationName(), QCoreApplication::organizationName());
    userData.childKeys();
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
ComposeSubstituteSentenceWidget::ComposeSubstituteSentenceWidget(QWidget* parent)
    : QWidget(parent)
    , _pimpl(new ComposeSubstituteSentenceWidgetPrivate(this))
{
}
ComposeSubstituteSentenceWidget::~ComposeSubstituteSentenceWidget(void) {}
void ComposeSubstituteSentenceWidget::initialize_after_success_search()
{
    _pimpl->reset();
    _pimpl->save_subtitution->setEnabled(false);
    MatchingWordsList result = gDataThread->getSearchResult(0, _pimpl->searchNumber.size());
    result.sort(cmp_success_words);
    _pimpl->word_results_container[0]->fill_matching_words(result);
}
void ComposeSubstituteSentenceWidget::on_word_selected(int end_index)
{
    size_t combo_box_starting_index = _pimpl->which_matching_word_selected();
    _pimpl->logger.log(log4cplus::DEBUG_LOG_LEVEL,
                       str(boost::format("'%1%' results word has been changed") % combo_box_starting_index));
    _pimpl->reset(combo_box_starting_index);
    _pimpl->logger.log(log4cplus::DEBUG_LOG_LEVEL,
                       str(boost::format("Next word should start from '%1%' digit of searched number ( %2%)") % end_index % _pimpl->searchNumber.substr(end_index-1)));
    MatchingWordsList result = gDataThread->getSearchResult(end_index, _pimpl->searchNumber.size());
    if (result.size()) {
        result.sort(cmp_success_words);
        _pimpl->add_combo_box();
        MatchingWordsList::iterator it = std::unique(result.begin(), result.end());
        _pimpl->word_results_container[combo_box_starting_index]->fill_matching_words(
            MatchingWordsList(result.begin(), it));
        // _pimpl->fill_combo_box(MatchingWordsList(result.begin(), it), combo_box_starting_index);
        _pimpl->save_subtitution->setEnabled(false);
    }
    else {
        _pimpl->save_subtitution->setEnabled(true);
        _pimpl->set_subtitute();
    }
}

void ComposeSubstituteSentenceWidget::setSearchNumber(std::string searchNumber)
{
    _pimpl->searchNumber = searchNumber;
}
void ComposeSubstituteSentenceWidget::on_save_clicked(bool)
{

}
