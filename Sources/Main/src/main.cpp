#include <QApplication>
#include <tools/loggers.h>
#include <GUI/MainWindow.h>
#include <memory>

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	app.setApplicationName("PhoneticNumberSystem");
	app.setOrganizationName("T4ng10r");
	createLoggers();

	std::unique_ptr<CMainWindow> ptMainWindow(new CMainWindow);
	ptMainWindow->show();

	//////////////////////////////////////////////////////////////////////////
	int iReturn = app.exec();
	//destroyLoggers();
	return iReturn;
}
