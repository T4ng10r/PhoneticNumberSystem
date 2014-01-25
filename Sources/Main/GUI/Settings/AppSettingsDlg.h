#ifndef _APPSETTINGSDLG_INCLUDE_
#define _APPSETTINGSDLG_INCLUDE_

#include <QWidget>
#include <boost/scoped_ptr.hpp>

class CAppSettingsDlgPrivate;

class CAppSettingsDlg : public QWidget
{
     Q_OBJECT
     friend class AppSettingsDlgPrivate;
public:
     CAppSettingsDlg(QWidget * parent = NULL);
     ~CAppSettingsDlg(void);
	 void performInitialUpdateAfterAllChildrenUpdate();
Q_SIGNALS:
	 void dictionarySelected();
protected:
     boost::scoped_ptr<CAppSettingsDlgPrivate> m_ptrPriv;
};
#endif //_APPSETTINGSDLG_INCLUDE_