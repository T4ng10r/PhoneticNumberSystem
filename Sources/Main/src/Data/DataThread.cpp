#include <Data/DataThread.h>
#include <QString>
#include <QApplication>
#include <Data/AppSettings.h>
#include <Data/CAppSettingsKeywords.h>
#include <Data/CDictionaryData.h>
#include <Data/CSubstituteSearch.h>
#include <QtCore/QDir>
#include <QtCore/QThread>

#include <list>
#include <Tools/loggers.h>
#include <Tools/qtTools.h>
#include <boost/foreach.hpp>
#include <boost/format.hpp>

boost::shared_ptr<DataThread> DataThread::pInstance_;
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
	boost::shared_ptr<CDictionaryData> dictionaryData;
	boost::shared_ptr<CSubstituteSearch> substituteSearch;
};

DataThreadPrivate::DataThreadPrivate(DataThread * ptrPublic):publicPart(ptrPublic), 
	dictionaryData(new CDictionaryData()), substituteSearch(new CSubstituteSearch())
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
		dirPath = gAppSettings->get<std::string>(DICTIONARIES_DIRECTORY).c_str();
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
	//std::string currentAppDictPath = gAppSettings->getCurrentDictPath();
	return false;
}
std::string  DataThreadPrivate::createCurrentDictionaryPath() 
{
	std::string dictionaryName = gAppSettings->get<std::string>(SELECTED_DICTIONARY,"");
	std::string dictionaryDir = gAppSettings->get<std::string>(DICTIONARIES_DIRECTORY,"");
	std::string dictionaryPath = dictionaryDir + QDir::separator().toLatin1() + dictionaryName+DICTIONARY_FILE_EXTENSION;
	return dictionaryPath;
}
std::string  DataThreadPrivate::createCurrentDictionaryAffPath() 
{
	std::string dictionaryName = gAppSettings->get<std::string>(SELECTED_DICTIONARY,"");
	std::string dictionaryDir = gAppSettings->get<std::string>(DICTIONARIES_DIRECTORY,"");
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
	std::string dictionaryDir = gAppSettings->get<std::string>(DICTIONARIES_DIRECTORY,"");
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
privPart(new DataThreadPrivate(this))
{
	QThread* thread = new QThread;
	this->moveToThread(thread);
	thread->start();
}
boost::shared_ptr<DataThread> DataThread::getInstance()
{
	if(!pInstance_)
	{
		if(!pInstance_)
		{
			pInstance_.reset(new DataThread());
		}
	}
	return pInstance_;
}
DataThread::~DataThread(void)
{
}
void DataThread::loadCurrentlySetDictionary()
{
	if (false==privPart->checkCurrentAppDictionary())
	{
		return;
	}
}
WordsList DataThread::getSearchResult(StartingIndex start_index)
{
	return privPart->substituteSearch->getSearchResult(start_index);
}

void DataThread::onScanDirectoryForDictionaries()
{
	QDir directory(privPart->get_dictionaries_directory().get());
	
	//QStringList fileFilters(QString("*.dic"));
	QFileInfoList dictionaryFileList = directory.entryInfoList(QStringList("*.dic"), QDir::Files | QDir::NoDotAndDotDot, QDir::Name);
	if (dictionaryFileList.empty())
		return;
	try
	{
		gAppSettings->get_child(DICTIONARY_FILES_LIST).erase("file");
	}
	catch (boost::property_tree::ptree_bad_path &/*e*/)
	{
		gAppSettings->put<std::string>(DICTIONARY_FILES_LIST,"");
	}
	Q_FOREACH ( const QFileInfo & fileItem, dictionaryFileList )
	{
		gAppSettings->add(DICTIONARY_FILE_ITEM, fileItem.fileName().toStdString());
		//dodaj listê dostêpnych s³owników do appsettings
	}
	//sprawdz wszystkie podkatalogi pierwszego poziomu 
	Q_EMIT onDictionariesFilesRefreshed();
}
void DataThread::onSetDictionary()
{
	std::string dicionaryPath = privPart->createCurrentDictionaryPath();
	Q_EMIT dictionaryLoaded(privPart->dictionaryData->loadDictionary(dicionaryPath));
}
void DataThread::onNumberSearchStarted(const std::string & number)
{
	if (gAppSettings->getDigitsConfiguraions().size()<=0)
	{
		Q_EMIT searchFinished(false);
		return;
	}
	privPart->substituteSearch->setSubstituteDigitsConfiguration(gAppSettings->getDigitsConfiguraions()[0]);
	privPart->substituteSearch->setDictionaryWords(privPart->dictionaryData);
	privPart->substituteSearch->startSearchForNumber(number);
}
QTextCodec * DataThread::get_current_codepage()
{
	std::string codepage = privPart->dictionaryData->get_file_codepage();
	QTextCodec * codec = QTextCodec::codecForName(codepage.c_str());
	return codec;
}
