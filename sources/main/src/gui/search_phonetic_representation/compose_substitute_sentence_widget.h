#ifndef _COMPOSESUBSTITUTESENTENCEWIDGET_INCLUDE_
#define _COMPOSESUBSTITUTESENTENCEWIDGET_INCLUDE_

#include <QWidget>
#include <boost/scoped_ptr.hpp>

class ComposeSubstituteSentenceWidgetPrivate;

class ComposeSubstituteSentenceWidget : public QWidget
{
	Q_OBJECT
	friend class ComposeSubstituteSentenceWidgetPrivate;
public:
	ComposeSubstituteSentenceWidget(QWidget * parent = NULL);
	~ComposeSubstituteSentenceWidget(void);
    void initialize_after_success_search();
    void setSearchNumber(std::string searchNumber);
protected Q_SLOTS:
  void on_word_selected(int end_index);
  void on_save_clicked(bool);
protected:
	boost::scoped_ptr<ComposeSubstituteSentenceWidgetPrivate> _pimpl;
};
#endif //_COMPOSESUBSTITUTESENTENCEWIDGET_INCLUDE_
