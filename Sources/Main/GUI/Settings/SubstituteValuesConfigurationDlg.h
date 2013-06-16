#ifndef _SUBSTITUTEVALUESCONFIGURATIONDLG_INCLUDE_
#define _SUBSTITUTEVALUESCONFIGURATIONDLG_INCLUDE_
#include <QtGui/QWidget>
#include <boost/scoped_ptr.hpp>
#include <boost/property_tree/ptree.hpp>

class CSubstituteValuesConfigurationDlgPrivate;

class CSubstituteValuesConfigurationDlg : public QWidget
{
	Q_OBJECT
	friend class SubstituteValuesConfigurationDlgPrivate;
public:
	CSubstituteValuesConfigurationDlg(const boost::property_tree::ptree &stProperties);
	~CSubstituteValuesConfigurationDlg(void);
public slots:
	void onMenuTriggered(QAction * action );
	void onActionToggled(bool );
protected:
	boost::scoped_ptr<CSubstituteValuesConfigurationDlgPrivate> m_ptrPriv;
};
#endif //_SUBSTITUTEVALUESCONFIGURATIONDLG_INCLUDE_

