#ifndef _DICTIONARIESCONFIGURATIONDLG_INCLUDE_
#define _DICTIONARIESCONFIGURATIONDLG_INCLUDE_

#include <QWidget>
#include <boost/scoped_ptr.hpp>

class CDictionariesConfigurationDlgPrivate;

class CDictionariesConfigurationDlg : public QWidget
{
     Q_OBJECT
     friend class DictionariesConfigurationDlgPrivate;
public:
     CDictionariesConfigurationDlg(QWidget * parent = NULL);
	 ~CDictionariesConfigurationDlg(void);
     void updateInitialData();
public Q_SLOTS:
	void onDictionariesFilesRefreshed();
	void onDictionaryFilesMenuActionToggled();
Q_SIGNALS:
	void dictionarySelected();
	void onDictionaryDirectoryChanged();
protected Q_SLOTS:
	void onSelectDictionaryDirectory();
protected:
     boost::scoped_ptr<CDictionariesConfigurationDlgPrivate> _pimpl;
};
#endif //_DICTIONARIESCONFIGURATIONDLG_INCLUDE_
