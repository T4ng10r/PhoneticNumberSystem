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
#include <tools/loggers.h>
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
public:
	CDataThread *	publicPart;
	boost::shared_ptr<CDictionaryData>	dictionaryData;
	boost::shared_ptr<CSubstituteSearch>	substituteSearch;
};

CDataThreadPrivate::CDataThreadPrivate(CDataThread * ptrPublic):publicPart(ptrPublic), 
	dictionaryData(new CDictionaryData()), substituteSearch(new CSubstituteSearch())
{
	setConnections();
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
	std::string dictionaryPath = dictionaryDir + QDir::separator().toAscii() + dictionaryName+DICTIONARY_FILE_EXTENSION;
	return dictionaryPath;
}
std::string  CDataThreadPrivate::createCurrentDictionaryAffPath() 
{
	std::string dictionaryName = gAppSettings->get<std::string>(SELECTED_DICTIONARY,"");
	std::string dictionaryDir = gAppSettings->get<std::string>(DICTIONARIES_DIRECTORY,"");
	std::string dictionaryPath = dictionaryDir + QDir::separator().toAscii() + dictionaryName+DICTIONARY_AFF_FILE_EXTENSION;
	return dictionaryPath;
}
void CDataThreadPrivate::setConnections()
{
	bool bResult = false;
	bResult = QObject::connect(substituteSearch.get(), SIGNAL(searchProgress(int)), 
		publicPart, SLOT(onSearchProgress(int)));
	logConnection("CDataThreadPrivate::setConnections",
		"'substituteSearch::searchProgress' with 'CDataThread::onSearchProgress'", 
		bResult);

	bResult = QObject::connect(substituteSearch.get(), SIGNAL(searchFinished()), 
		publicPart, SIGNAL(searchFinished()));
	logConnection("CDataThreadPrivate::setConnections",
		"'substituteSearch::searchFinished' with 'CDataThread::searchFinished'", 
		bResult);

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
	int a=1;
}
void CDataThread::loadCurrentAppDictionary()
{
	if (false==privPart->checkCurrentAppDictionary())
	{
		return;
	}
}
const WordSearchResult & CDataThread::getSearchResult()
{
	return privPart->substituteSearch->getSearchResult();
}
//////////////////////////////////////////////////////////////////////////
void CDataThread::onSearchProgress(int current)
{
	Q_EMIT searchProgress(current, privPart->dictionaryData->getWordsCount());
}
void CDataThread::onScanDirectoryForDictionaries()
{
	QString dirPath = gAppSettings->get<std::string>(DICTIONARIES_DIRECTORY).c_str();

	QDir directory(dirPath);
	
	QStringList fileFilters(QString("*.dic"));
	QFileInfoList dictionaryFileList = directory.entryInfoList(fileFilters, QDir::Files | QDir::NoDotAndDotDot, QDir::Name);
	if (dictionaryFileList.empty())
		return;
	//delete current dictionaries file
	gAppSettings->get_child(DICTIONARY_FILES_LIST).erase("file");
	Q_FOREACH ( const QFileInfo & fileItem, dictionaryFileList )
	{
		gAppSettings->add(DICTIONARY_FILE_ITEM, fileItem.fileName().toStdString());
		//dodaj listê dostêpnych s³owników do appsettings
	}
	//sprawdz wszystkie podkatalogi pierwszego poziomu 
	Q_EMIT onDictionariesFilesRefreshed();
}
void CDataThread::onSetDictionary()
{
	std::string dicionaryPath = privPart->createCurrentDictionaryPath();
	std::string dicionaryAff = privPart->createCurrentDictionaryAffPath();
	privPart->dictionaryData->loadDictionary(dicionaryPath);
	Q_EMIT dictionaryLoaded();
}
void CDataThread::onNumberSearchStarted(const std::string & number)
{
	privPart->substituteSearch->setDictionaryWords(privPart->dictionaryData);
	privPart->substituteSearch->startSearchForNumber(number);
}
