#include <Data/CDataThread.h>
#include <QString>
#include <QApplication>
#include <Data/CAppSettings.h>
#include <Data/CAppSettingsKeywords.h>
#include <Data/CDictionaryData.h>
#include <Data/CSubstituteSearch.h>
#include <QtCore/QDir>
#include <QtCore/QThread>

#include <list>
#include <Tools/loggers.h>
#include <Tools/qtTools.h>
#include <boost/foreach.hpp>

boost::shared_ptr<CDataThread> CDataThread::pInstance_;
enum { MaxRecentFiles = 5 };

class CDataThreadPrivate
{
public:
	CDataThreadPrivate(CDataThread * ptrPublic);
	~CDataThreadPrivate();
	bool checkCurrentAppDictionary();
	std::string createCurrentDictionaryPath();
	std::string createCurrentDictionaryAffPath();
	void setConnections();
	void prepareDirectories();
public:
	CDataThread * publicPart;
	boost::shared_ptr<CDictionaryData> dictionaryData;
	boost::shared_ptr<CSubstituteSearch> substituteSearch;
};

CDataThreadPrivate::CDataThreadPrivate(CDataThread * ptrPublic):publicPart(ptrPublic), 
	dictionaryData(new CDictionaryData()), substituteSearch(new CSubstituteSearch())
{
	setConnections();
	prepareDirectories();
}
CDataThreadPrivate::~CDataThreadPrivate()
{
}
bool CDataThreadPrivate::checkCurrentAppDictionary()
{
	std::string currentAppDictPath = gAppSettings->getCurrentDictPath();
	return false;
}
std::string  CDataThreadPrivate::createCurrentDictionaryPath() 
{
	std::string dictionaryName = gAppSettings->get<std::string>(SELECTED_DICTIONARY,"");
	std::string dictionaryDir = gAppSettings->get<std::string>(DICTIONARIES_DIRECTORY,"");
	std::string dictionaryPath = dictionaryDir + QDir::separator().toLatin1() + dictionaryName+DICTIONARY_FILE_EXTENSION;
	return dictionaryPath;
}
std::string  CDataThreadPrivate::createCurrentDictionaryAffPath() 
{
	std::string dictionaryName = gAppSettings->get<std::string>(SELECTED_DICTIONARY,"");
	std::string dictionaryDir = gAppSettings->get<std::string>(DICTIONARIES_DIRECTORY,"");
	std::string dictionaryPath = dictionaryDir + QDir::separator().toLatin1() + dictionaryName+DICTIONARY_AFF_FILE_EXTENSION;
	return dictionaryPath;
}
void CDataThreadPrivate::setConnections()
{
	bool bResult = false;
	bResult = QObject::connect(substituteSearch.get(), SIGNAL(searchProgress(int,int)), 
		publicPart, SIGNAL(searchProgress(int,int)));
	logConnection("CDataThreadPrivate::setConnections",
		"'substituteSearch::searchProgress' with 'CDataThread::onSearchProgress'", 
		bResult);

	bResult = QObject::connect(substituteSearch.get(), SIGNAL(searchFinished(bool)), 
		publicPart, SIGNAL(searchFinished(bool)));
	logConnection("CDataThreadPrivate::setConnections",
		"'substituteSearch::searchFinished' with 'CDataThread::searchFinished'", 
		bResult);

}
void CDataThreadPrivate::prepareDirectories()
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
CDataThread::CDataThread(void):
privPart(new CDataThreadPrivate(this))
{
	QThread* thread = new QThread;
	this->moveToThread(thread);
	thread->start();
}
boost::shared_ptr<CDataThread> CDataThread::getInstance()
{
	if(!pInstance_)
	{
		if(!pInstance_)
		{
			pInstance_.reset(new CDataThread());
		}
	}
	return pInstance_;
}
CDataThread::~CDataThread(void)
{
}
void CDataThread::loadCurrentlySetDictionary()
{
	if (false==privPart->checkCurrentAppDictionary())
	{
		return;
	}
}
WordsList CDataThread::getSearchResult(StartingIndex start_index)
{
	return privPart->substituteSearch->getSearchResult(start_index);
}

void CDataThread::onScanDirectoryForDictionaries()
{
	QString dirPath;
	try
	{
		dirPath = gAppSettings->get<std::string>(DICTIONARIES_DIRECTORY).c_str();
	}
	catch (boost::property_tree::ptree_bad_path &e)
	{
		printLog(eWarningLogLevel,eDebug,QString("Error during gathering dictionaryFilesList '%1'").arg(e.what()));	
	}

	QDir directory(dirPath);
	
	QStringList fileFilters(QString("*.dic"));
	QFileInfoList dictionaryFileList = directory.entryInfoList(fileFilters, QDir::Files | QDir::NoDotAndDotDot, QDir::Name);
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
		//dodaj list� dost�pnych s�ownik�w do appsettings
	}
	//sprawdz wszystkie podkatalogi pierwszego poziomu 
	Q_EMIT onDictionariesFilesRefreshed();
}
void CDataThread::onSetDictionary()
{
	std::string dicionaryPath = privPart->createCurrentDictionaryPath();
	Q_EMIT dictionaryLoaded(privPart->dictionaryData->loadDictionary(dicionaryPath));
}
void CDataThread::onNumberSearchStarted(const std::string & number)
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
QTextCodec * CDataThread::get_current_codepage()
{
	std::string codepage = privPart->dictionaryData->get_file_codepage();
	QTextCodec * codec = QTextCodec::codecForName(codepage.c_str());
	return codec;
}