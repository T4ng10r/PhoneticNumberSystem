#ifndef _COMPOSESUBSTITUTESENTENCEWIDGET_INCLUDE_
#define _COMPOSESUBSTITUTESENTENCEWIDGET_INCLUDE_

#include <QtGui/QWidget>
#include <boost/scoped_ptr.hpp>

class ComposeSubstituteSentenceWidgetPrivate;

class ComposeSubstituteSentenceWidget : public QWidget
{
     Q_OBJECT
     friend class ComposeSubstituteSentenceWidgetPrivate;
public:
      ComposeSubstituteSentenceWidget(QWidget * parent = NULL);
     ~ComposeSubstituteSentenceWidget(void);
	 void reset();
protected:
     boost::scoped_ptr<ComposeSubstituteSentenceWidgetPrivate> m_ptrPriv;
};
#endif //_COMPOSESUBSTITUTESENTENCEWIDGET_INCLUDE_