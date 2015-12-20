#ifndef _CAF_DATA_THREAD_INCLUDE_
#define _CAF_DATA_THREAD_INCLUDE_
#include <QtCore/QObject>
#include <Data/CSubstituteSearchTypes.h>
#include <QtCore/QTextCodec>
#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>

class DataThreadPrivate;

class DataThread : public QObject
{
	Q_OBJECT
	friend class DataThreadPrivate;
	DataThread(void);
	static boost::shared_ptr<DataThread> pInstance_;
public:
	~DataThread(void);
	static boost::shared_ptr<DataThread> getInstance();
  //currently not used
	void loadCurrentlySetDictionary();
	WordsList getSearchResult(StartingIndex start_index);
  QTextCodec * get_current_codepage();
  
public Q_SLOTS:
  void onScanDirectoryForDictionaries();

  //void getDictionariesPath();

  void onSetDictionary();
  //initiate searching for substitute words
  void onNumberSearchStarted(const std::string & number);
Q_SIGNALS:
	void dictionaryLoaded(bool success);
	void onDictionariesFilesRefreshed();
	void searchProgress(int current, int max);
	void searchFinished(bool);
protected:
	boost::scoped_ptr<DataThreadPrivate> privPart;
};

#define gDataThread DataThread::getInstance() 


#endif //_CAF_DATA_THREAD_INCLUDE_
