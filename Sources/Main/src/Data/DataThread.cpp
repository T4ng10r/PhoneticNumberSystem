#include <Data/DataThread.h>
#include <QString>
#include <QApplication>
#include <Data/Settings.h>
#include <Data/CSettingsKeywords.h>
#include <Data/DictionaryData.h>
#include <Data/MatchingWordsSearch.h>
#include <QtCore/QDir>
#include <QtCore/QThread>

#include <list>
#include <Tools/loggers.h>
#include <Tools/qtTools.h>
#include <boost/foreach.hpp>
#include <boost/format.hpp>

DataThread::ptr DataThread::_instance;
enum { MaxRecentFiles = 5 };

class DataThreadPrivate
{
public:
	DataThreadPrivate(DataThread * ptrPublic);
	~DataThreadPrivate();
	bool checkCurrentAppDictionary();
	std::string createCurrentDictionaryPath();
	std::string createCurrentDictionaryAffPath();
	void setConnections();
	void prepareDirectories();
	boost::optional<QDir> get_dictionaries_directory();
public:
	DataThread * publicPart;
	boost::shared_ptr<DictionaryData> dictionaryData;
	boost::shared_ptr<MatchingWordsSearch> substituteSearch;
};

DataThreadPrivate::DataThreadPrivate(DataThread * ptrPublic):publicPart(ptrPublic), 
	dictionaryData(new DictionaryData()), substituteSearch(new MatchingWordsSearch())
{
	setConnections();
	prepareDirectories();
}
DataThreadPrivate::~DataThreadPrivate()
{
}


boost::optional<QDir> DataThreadPrivate::get_dictionaries_directory()
{
	boost::optional<QDir> result;
	QString dirPath;
	try
	{
		dirPath = gSettings->get<std::string>(DICTIONARIES_DIRECTORY).c_str();
	}
	catch (boost::property_tree::ptree_bad_path &e)
	{
		printLog(eDebug, eWarningLogLevel, str(boost::format("Error during gathering dictionaryFilesList '%1%'")
			% e.what()));
		return result;
	}
	return QDir(dirPath);
}

bool DataThreadPrivate::checkCurrentAppDictionary()
{
	//std::string currentAppDictPath = gSettings->getCurrentDictPath();
	return false;
}
std::string  DataThreadPrivate::createCurrentDictionaryPath() 
{
	std::string dictionaryName = gSettings->get<std::string>(SELECTED_DICTIONARY,"");
	std::string dictionaryDir = gSettings->get<std::string>(DICTIONARIES_DIRECTORY,"");
	std::string dictionaryPath = dictionaryDir + QDir::separator().toLatin1() + dictionaryName+DICTIONARY_FILE_EXTENSION;
	return dictionaryPath;
}
std::string  DataThreadPrivate::createCurrentDictionaryAffPath() 
{
	std::string dictionaryName = gSettings->get<std::string>(SELECTED_DICTIONARY,"");
	std::string dictionaryDir = gSettings->get<std::string>(DICTIONARIES_DIRECTORY,"");
	std::string dictionaryPath = dictionaryDir + QDir::separator().toLatin1() + dictionaryName+DICTIONARY_AFF_FILE_EXTENSION;
	return dictionaryPath;
}
void DataThreadPrivate::setConnections()
{
	bool bResult = false;
	bResult = QObject::connect(substituteSearch.get(), SIGNAL(searchProgress(int,int)), 
		publicPart, SIGNAL(searchProgress(int,int)));
	logConnection("DataThreadPrivate::setConnections",
		"'substituteSearch::searchProgress' with 'DataThread::onSearchProgress'", 
		bResult);

	bResult = QObject::connect(substituteSearch.get(), SIGNAL(searchFinished(bool)), 
		publicPart, SIGNAL(searchFinished(bool)));
	logConnection("DataThreadPrivate::setConnections",
		"'substituteSearch::searchFinished' with 'DataThread::searchFinished'", 
		bResult);

}
void DataThreadPrivate::prepareDirectories()
{
	std::string dictionaryDir = gSettings->get<std::string>(DICTIONARIES_DIRECTORY,"");
	//check if DICTIONARIES_DIRECTORY exist in settings
	QDir dir;
	if (false==dir.exists(dictionaryDir.c_str()))
	{
		dir.mkpath(dictionaryDir.c_str());
	}
}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////// 
DataThread::DataThread(void):
_pimpl(new DataThreadPrivate(this))
{
	QThread* thread = new QThread;
	this->moveToThread(thread);
	thread->start();
}
DataThread::ptr DataThread::instance()
{
	if(!_instance)
	{
		if(!_instance)
		{
			_instance.reset(new DataThread());
		}
	}
	return _instance;
}
DataThread::~DataThread(void)
{
}
void DataThread::loadCurrentlySetDictionary()
{
	if (false==_pimpl->checkCurrentAppDictionary())
	{
		return;
	}
}
MatchingWordsList DataThread::getSearchResult(StartingIndex start_index)
{
	return _pimpl->substituteSearch->getSearchResult(start_index);
}

void DataThread::onScanDirectoryForDictionaries()
{
	QDir directory(_pimpl->get_dictionaries_directory().get());
	
	//QStringList fileFilters(QString("*.dic"));
	QFileInfoList dictionaryFileList = directory.entryInfoList(QStringList("*.dic"), QDir::Files | QDir::NoDotAndDotDot, QDir::Name);
	if (dictionaryFileList.empty())
		return;
	try
	{
		gSettings->get_child(DICTIONARY_FILES_LIST).erase("file");
	}
	catch (boost::property_tree::ptree_bad_path &/*e*/)
	{
		gSettings->put<std::string>(DICTIONARY_FILES_LIST,"");
	}
	Q_FOREACH ( const QFileInfo & fileItem, dictionaryFileList )
	{
		gSettings->add(DICTIONARY_FILE_ITEM, fileItem.fileName().toStdString());
		//dodaj listê dostêpnych s³owników do appsettings
	}
	//sprawdz wszystkie podkatalogi pierwszego poziomu 
	Q_EMIT onDictionariesFilesRefreshed();
}
void DataThread::onSetDictionary()
{
	std::string dicionaryPath = _pimpl->createCurrentDictionaryPath();
	Q_EMIT dictionaryLoaded(_pimpl->dictionaryData->loadDictionary(dicionaryPath));
}
void DataThread::onNumberSearchStarted(const std::string & number)
{
	if (gSettings->getDigitsConfiguraions().size()<=0)
	{
		Q_EMIT searchFinished(false);
		return;
	}
	_pimpl->substituteSearch->setSubstituteDigitsConfiguration(gSettings->getDigitsConfiguraions()[0]);
	_pimpl->substituteSearch->setDictionaryWords(_pimpl->dictionaryData);
	_pimpl->substituteSearch->startSearchForNumber(number);
}
QTextCodec * DataThread::get_current_codepage()
{
	std::string codepage = _pimpl->dictionaryData->get_file_codepage();
	QTextCodec * codec = QTextCodec::codecForName(codepage.c_str());
  if (!codec)
  	printLog(eDebug, eWarningLogLevel, str(boost::format("Couldn't get Text Codec for' %1%' codepage") % codepage));
	return codec;
}
