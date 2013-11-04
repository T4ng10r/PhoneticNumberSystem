#ifndef _SEARCHRESULTWORDWIDGET_INCLUDE_
#define _SEARCHRESULTWORDWIDGET_INCLUDE_

#include <QtGui/QLabel>
#include <boost/scoped_ptr.hpp>

class CSearchResultWordWidgetPrivate;

class CSearchResultWordWidget : public QLabel
{
     Q_OBJECT
     friend class SearchResultWordWidgetPrivate;
public:
     CSearchResultWordWidget(QWidget * parent = NULL);
     ~CSearchResultWordWidget(void);
	 void setFoundWord(const std::string & text);
	 void setHighlightedLetters(const std::string & highlight_letters);
protected:
     boost::scoped_ptr<CSearchResultWordWidgetPrivate> m_ptrPriv;
};
#endif //_SEARCHRESULTWORDWIDGET_INCLUDE_
