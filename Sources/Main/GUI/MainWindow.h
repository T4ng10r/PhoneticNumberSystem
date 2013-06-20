#ifndef _CAF_WINDOW_INCLUDE_
#define _CAF_WINDOW_INCLUDE_

#include <QMutex>
#include <QMainWindow>
#include <boost/scoped_ptr.hpp>

class CMainWindowPrivate;

class CMainWindow : public QMainWindow
{
	Q_OBJECT
public:
	CMainWindow();
	~CMainWindow();
public slots:
	void onActionTrigger(bool);
protected:
	void closeEvent(QCloseEvent *);
	QString strippedName(const QString &fullFileName);
protected:
	boost::scoped_ptr<CMainWindowPrivate> m_ptrPriv;
};
#endif //_CAF_WINDOW_INCLUDE_ 
