#ifndef _CAF_DATA_THREAD_INCLUDE_
#define _CAF_DATA_THREAD_INCLUDE_
#include <QtCore/QObject>
#include <Data/CSubstituteSearchTypes.h>
#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>

class CDataThreadPrivate;

class CDataThread : public QObject
{
	Q_OBJECT
	friend class CDataThreadPrivate;
	CDataThread(void);
	static boost::shared_ptr<CDataThread> pInstance_;
public:
	~CDataThread(void);
	static boost::shared_ptr<CDataThread> getInstance();
	void loadCurrentAppDictionary();
	WordSearchResult getSearchResult();
Q_SIGNALS:
	void onDictionariesFilesRefreshed();
	void searchProgress(int current, int max);
	void searchFinished();
	void dictionaryLoaded();
public Q_SLOTS:
	void onScanDirectoryForDictionaries();
	void onSetDictionary();
	void onNumberSearchStarted(const std::string & number);
//protected Q_SLOTS:
//	void onSearchProgress(int current);
protected:
	boost::scoped_ptr<CDataThreadPrivate> privPart;
};

#define gDataThread CDataThread::getInstance() 


#endif //_CAF_DATA_THREAD_INCLUDE_
