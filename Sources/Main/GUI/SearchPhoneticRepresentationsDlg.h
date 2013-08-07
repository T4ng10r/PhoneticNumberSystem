#ifndef _SEARCHPHONETICREPRESENTATIONSDLG_INCLUDE_
#define _SEARCHPHONETICREPRESENTATIONSDLG_INCLUDE_

#include <QtGui/QWidget>
#include <boost/scoped_ptr.hpp>

class CSearchPhoneticRepresentationsDlgPrivate;

class CSearchPhoneticRepresentationsDlg : public QWidget
{
     Q_OBJECT
     friend class SearchPhoneticRepresentationsDlgPrivate;
public:
     CSearchPhoneticRepresentationsDlg(QWidget * parent = NULL);
     ~CSearchPhoneticRepresentationsDlg(void);
protected slots:
	void onPerformSearch();
protected:
     boost::scoped_ptr<CSearchPhoneticRepresentationsDlgPrivate> m_ptrPriv;
};
#endif //_SEARCHPHONETICREPRESENTATIONSDLG_INCLUDE_
