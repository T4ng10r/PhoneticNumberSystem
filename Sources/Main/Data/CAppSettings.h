#ifndef _CAPPSETTINGS_INCLUDE_
#define _CAPPSETTINGS_INCLUDE_

//#include <QtCore/QObject>
#include <boost/scoped_ptr.hpp>
#include <boost/property_tree/ptree.hpp>

class CAppSettingsPrivate;

class CAppSettings : public boost::property_tree::ptree //QObject
{
	//Q_OBJECT
	friend class CAppSettingsPrivate;
public:
	CAppSettings(void);
	~CAppSettings(void);

protected:
	boost::scoped_ptr<CAppSettingsPrivate> m_ptrPriv;
};
#endif //_CAPPSETTINGS_INCLUDE_

