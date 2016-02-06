#ifndef _SUBSTITUTEVALUESCONFIGURATIONDLG_INCLUDE_
#define _SUBSTITUTEVALUESCONFIGURATIONDLG_INCLUDE_
#include <QWidget>
#include <boost/scoped_ptr.hpp>
#include <boost/property_tree/ptree.hpp>
#include <vector>
#include <data/CSystemDigitsConfiguration.h>

class CSubstituteValuesConfigurationDlgPrivate;

struct CSubstituteValuesConfigurationDlgInit
{
  boost::property_tree::ptree consonants_values_set;
  std::vector<CSingleSubstituteDigitsConfiguration> digits_substistute_configuration;
  QString selected_system_name;
};

class CSubstituteValuesConfigurationDlg : public QWidget
{
	Q_OBJECT
	friend class SubstituteValuesConfigurationDlgPrivate;
public:
	CSubstituteValuesConfigurationDlg(CSubstituteValuesConfigurationDlgInit init_data);
	~CSubstituteValuesConfigurationDlg(void);
Q_SIGNALS:
  void set_selected_consonant_system(const QString &);
public Q_SLOTS:
	void onMenuTriggered_SetButtonTextWithSelectedConsonant(QAction * action );
	void onActionToggled_DeactivateThisConsonantInOtherMenus(bool );
	void onSystemsActvivated_changeCurrentDigitsSystem(const QString&);
protected:
	boost::scoped_ptr<CSubstituteValuesConfigurationDlgPrivate> _pimpl;
};
#endif //_SUBSTITUTEVALUESCONFIGURATIONDLG_INCLUDE_

