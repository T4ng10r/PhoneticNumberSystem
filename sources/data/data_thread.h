#ifndef _CAF_DATA_THREAD_INCLUDE_
#define _CAF_DATA_THREAD_INCLUDE_
#include <QtCore/QObject>
#include <QtCore/QTextCodec>
#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <data/matching_words_search_types.h>
#include <QStringList>

class DataThreadPrivate;


class DataThread : public QObject
{
    Q_OBJECT
    friend class DataThreadPrivate;
    static const StartingIndex c_end_index = 1000000;

  public:
    typedef boost::shared_ptr<DataThread> ptr;

    ~DataThread(void);
    static ptr instance();
    // currently not used
    void              loadCurrentlySetDictionary();
    MatchingWordsList getSearchResult(StartingIndex start_index, StartingIndex end_index = c_end_index);
    QTextCodec* get_current_codepage();
    void save_substitute(QStringList substituteWords, std::string number);
    void load_user_substitution();
    std::map<std::string, QStringList> get_user_substitutions();

  public Q_SLOTS:
    void onScanDirectoryForDictionaries();

    // void getDictionariesPath();

    void onSetDictionary();
    // initiate searching for substitute words
    void onNumberSearchStarted(const std::string& number);
  Q_SIGNALS:
    void dictionaryLoaded(bool success);
    void onDictionariesFilesRefreshed();
    void searchProgress(int current, int max);
    void searchFinished(bool);

  protected:
    boost::scoped_ptr<DataThreadPrivate> _pimpl;

  private:
    DataThread(void);
    static ptr _instance;
};

#define gDataThread DataThread::instance()

#endif //_CAF_DATA_THREAD_INCLUDE_
