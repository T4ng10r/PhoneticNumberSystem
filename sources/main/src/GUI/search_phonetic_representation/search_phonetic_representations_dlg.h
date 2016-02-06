#ifndef _SEARCHPHONETICREPRESENTATIONSDLG_INCLUDE_
#define _SEARCHPHONETICREPRESENTATIONSDLG_INCLUDE_

#include <QWidget>
#include <boost/scoped_ptr.hpp>

class CSearchPhoneticRepresentationsDlgPrivate;

class CSearchPhoneticRepresentationsDlg : public QWidget
{
     Q_OBJECT
     friend class SearchPhoneticRepresentationsDlgPrivate;
public:
     CSearchPhoneticRepresentationsDlg(QWidget * parent = NULL);
     ~CSearchPhoneticRepresentationsDlg(void);
Q_SIGNALS:
	 void performSearch(const std::string & );
public Q_SLOTS:
	void onSearchProgress(int current, int max);
	void searchFinished(bool);
	void disableSearchButton();
	void enableSearchButton();
protected Q_SLOTS:
	void onPerformSearch();
	void on_return_pressed();
protected:
     boost::scoped_ptr<CSearchPhoneticRepresentationsDlgPrivate> _pimpl;
};
#endif //_SEARCHPHONETICREPRESENTATIONSDLG_INCLUDE_
