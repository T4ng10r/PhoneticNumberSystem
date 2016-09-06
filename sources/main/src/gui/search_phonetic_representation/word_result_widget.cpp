#include "substitute_search_result_combobox.h"
#include "word_result_widget.h"
#include <gui/search_phonetic_representation/combobox_matching_words_result_delegate.h>
#include <QLabel>
#include <QBoxLayout>
#include <tools/loggers.h>
#include <tools/qtTools.h>
#include <data/data_thread.h>

class WordResultWidgetPrivate
{
  public:
    WordResultWidgetPrivate(WordResultWidget* public_)
        : ptr_public(public_)
    {
      setupUI();
      setConnections();
    }
    void setupUI();
    void setConnections();
    void fill(const MatchingWordsList& list);
    QString process_word(MatchingWord word, QTextCodec* codec);

  public:
    WordResultWidget*               ptr_public;
    SubstituteSearchResultComboBox* combo_box;
    QLabel*                         label;
    static ComboBoxMatchingWordsResultDelegate delegate_;
};

ComboBoxMatchingWordsResultDelegate WordResultWidgetPrivate::delegate_;

void WordResultWidgetPrivate::setupUI()
{
    delete ptr_public->layout();
    QVBoxLayout* main_layout = new QVBoxLayout;
    label                    = new QLabel();
    label->setAlignment(Qt::AlignHCenter);
    main_layout->addWidget(label);

    combo_box = new SubstituteSearchResultComboBox();
    main_layout->addWidget(combo_box);
    combo_box->setItemDelegate(&delegate_);
    combo_box->setSizeAdjustPolicy(QComboBox::AdjustToContents);

    ptr_public->setLayout(main_layout);
}
void WordResultWidgetPrivate::setConnections()
{
    bool bResult = false;
    bResult      = QObject::connect(combo_box, SIGNAL(activated(int)), ptr_public, SLOT(on_activated(int)));
    logConnection("WordResultWidgetPrivate::add_combo_box",
                  "'combo_box::activated' with 'public_part::on_activated'", bResult);
}
void WordResultWidgetPrivate::fill(const MatchingWordsList& list)
{
    combo_box->clear();
    combo_box->addItem("");

    QTextCodec* codec = gDataThread->get_current_codepage();
    for (const MatchingWord& word : list) {
        QString q_word = process_word(word, codec);
        combo_box->addItem(q_word, QVariant::fromValue(word));
    }

}
QString WordResultWidgetPrivate::process_word(MatchingWord word, QTextCodec* codec)
{
    QString style_word;
    if (codec)
        style_word = codec->toUnicode(word.getWord().c_str());
    else
        style_word = word.getWord().c_str();
    return style_word;
}
//--------------------------------------------------------------
WordResultWidget::WordResultWidget(QWidget* parent)
    : QWidget(parent)
    , _pimpl(new WordResultWidgetPrivate(this))
{
}
WordResultWidget::~WordResultWidget(){}
void WordResultWidget::fill_matching_words(const MatchingWordsList& list)
{
  _pimpl->fill(list);
}
void WordResultWidget::on_activated(int index)
{
    const MatchingWord& matching_word = _pimpl->combo_box->itemData(index).value<MatchingWord>();
    _pimpl->label->setText(matching_word.getWord().c_str());
    Q_EMIT word_selected(matching_word.coveredDigitsIndices.front().end_index+1);
}

