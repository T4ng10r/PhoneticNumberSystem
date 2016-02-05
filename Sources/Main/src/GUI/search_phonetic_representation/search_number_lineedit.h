#ifndef _SEARCHNUMBERLINEEDIT_INCLUDE_
#define _SEARCHNUMBERLINEEDIT_INCLUDE_

#include <QLineEdit>
#include <boost/scoped_ptr.hpp>

class SearchNumberLineEditPrivate;

class SearchNumberLineEdit : public QLineEdit
{
     Q_OBJECT
     friend class SearchNumberLineEditPrivate;
public:
      SearchNumberLineEdit(QWidget* parent=0);
     ~SearchNumberLineEdit(void);
protected:
     boost::scoped_ptr<SearchNumberLineEditPrivate> _pimpl;
};
#endif //_SEARCHNUMBERLINEEDIT_INCLUDE_
