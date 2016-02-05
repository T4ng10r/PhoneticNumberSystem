#ifndef _CAF_WINDOW_INCLUDE_
#define _CAF_WINDOW_INCLUDE_

#include <QMutex>
#include <QMainWindow>
#include <boost/scoped_ptr.hpp>

class MainWindowPrivate;

class CMainWindow : public QMainWindow
{
	Q_OBJECT
public:
	CMainWindow();
	~CMainWindow();
public Q_SLOTS:
	void onShowSettingsConfigureDialog(bool);
protected:
	void closeEvent(QCloseEvent *);
	QString strippedName(const QString &fullFileName);
protected:
	boost::scoped_ptr<MainWindowPrivate> _pimpl;
};
#endif //_CAF_WINDOW_INCLUDE_ 
