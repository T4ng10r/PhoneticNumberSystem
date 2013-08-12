#ifndef _CAF_DATA_THREAD_INCLUDE_
#define _CAF_DATA_THREAD_INCLUDE_
#include <QThread>
#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>

class CDataThreadPrivate;

class CDataThread : public QThread
{
	Q_OBJECT
	friend class CDataThreadPrivate;
	CDataThread(void);
	static boost::shared_ptr<CDataThread> pInstance_;
public:
	~CDataThread(void);
	static boost::shared_ptr<CDataThread> getInstance();
	void run();
	void loadCurrentAppDictionary();
public Q_SLOTS:
protected:
	boost::scoped_ptr<CDataThreadPrivate> privPart;
};
#endif //_CAF_DATA_THREAD_INCLUDE_
