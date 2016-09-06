#ifndef _WORD_RESULT_WIDGET_INCLUDE_
#define _WORD_RESULT_WIDGET_INCLUDE_

#include <QWidget>
#include <boost/scoped_ptr.hpp>
#include <data/matching_words_search_types.h>

class WordResultWidgetPrivate;

class WordResultWidget : public QWidget
{
    Q_OBJECT
    friend class WordResultWidgetPrivate;

  public:
    WordResultWidget(QWidget* parent = NULL);
    ~WordResultWidget();
    void fill_matching_words(const MatchingWordsList& list);

  protected Q_SLOTS:
    void on_activated(int);
  Q_SIGNALS:
    void word_selected(int end_index);

  protected:
    boost::scoped_ptr<WordResultWidgetPrivate> _pimpl;
};

#endif
