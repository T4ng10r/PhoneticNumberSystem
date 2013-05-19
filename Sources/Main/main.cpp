#include <QtGui/QApplication>
#include <Tools/loggers.h>
#include <GUI/MainWindow.h>
#include <boost/scoped_ptr.hpp>
#include <Data/CAppSettings.h>

CAppSettings	stAppSettings;

int main(int argc, char *argv[])
{
      //Q_INIT_RESOURCE(application);
      QApplication app(argc, argv);
	  app.setApplicationName("PhoneticNumberSystem");
	  app.setOrganizationName("T4ng10r");

	  createLoggers();
	  boost::scoped_ptr<CMainWindow> ptMainWindow(new CMainWindow);
      ptMainWindow->show();

	  //////////////////////////////////////////////////////////////////////////
	  int iReturn = app.exec();
	  destroyLoggers();
	  return iReturn;
}
