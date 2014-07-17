#ifndef _CAF_DATA_THREAD_INCLUDE_
#define _CAF_DATA_THREAD_INCLUDE_
#include <QtCore/QObject>
#include <Data/CSubstituteSearchTypes.h>
#include <QtCore/QTextCodec>
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
  //currently not used
	void loadCurrentlySetDictionary();
	WordsList getSearchResult(StartingIndex start_index);
  QTextCodec * get_current_codepage();
public Q_SLOTS:
  void onScanDirectoryForDictionaries();
  void onSetDictionary();
  //initiate searching for substitute words
  void onNumberSearchStarted(const std::string & number);
Q_SIGNALS:
	void dictionaryLoaded(bool success);
	void onDictionariesFilesRefreshed();
	void searchProgress(int current, int max);
	void searchFinished(bool);
protected:
	boost::scoped_ptr<CDataThreadPrivate> privPart;
};

#define gDataThread CDataThread::getInstance() 


#endif //_CAF_DATA_THREAD_INCLUDE_