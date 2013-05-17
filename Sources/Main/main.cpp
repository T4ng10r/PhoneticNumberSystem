#include <QtGui/QApplication>
#include <GUI/MainWindow.h>

#include <QtGui/QTreeView>
#include <Tools/loggers.h>
#include <boost/scoped_ptr.hpp>

int main(int argc, char *argv[])
{
      //Q_INIT_RESOURCE(application);
      QApplication app(argc, argv);
	  app.setApplicationName("CyfrowyAlfabetFonetyczny");
	  app.setOrganizationName("T4ng10r");

	  createLoggers();
	  boost::scoped_ptr<CMainWindow> ptMainWindow(new CMainWindow);
      ptMainWindow->show();

	  //////////////////////////////////////////////////////////////////////////
	  int iReturn = app.exec();
	  destroyLoggers();
	  return iReturn;
}
