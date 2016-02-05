#ifndef _APPSETTINGSDLG_INCLUDE_
#define _APPSETTINGSDLG_INCLUDE_

#include <QWidget>
#include <boost/scoped_ptr.hpp>

class CSettingsDlgPrivate;

class CSettingsDlg : public QWidget
{
     Q_OBJECT
     friend class SettingsDlgPrivate;
public:
     CSettingsDlg(QWidget * parent = NULL);
     ~CSettingsDlg(void);
	 void performInitialUpdateAfterAllChildrenUpdate();
Q_SIGNALS:
	 void dictionarySelected();
protected:
     boost::scoped_ptr<CSettingsDlgPrivate> _pimpl;
};
#endif //_APPSETTINGSDLG_INCLUDE_
