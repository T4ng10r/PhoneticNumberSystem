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
public slots:
protected:
	boost::scoped_ptr<CDataThreadPrivate> m_ptrPriv;
};
#endif //_CAF_DATA_THREAD_INCLUDE_
