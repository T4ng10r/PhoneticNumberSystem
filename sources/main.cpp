#include <gui/main_window.h>
#include <QApplication>
#include <memory>
#include <loggers.h>

int main(int argc, char* argv[])
{

  QApplication app(argc, argv);
  app.setApplicationName("PhoneticNumberSystem");
  app.setOrganizationName("T4ng10r");
  create_loggers();

  std::unique_ptr<MainWindow> main_window(new MainWindow);
  main_window->show();

  //////////////////////////////////////////////////////////////////////////
  int iReturn = app.exec();
  // destroyLoggers();
  return iReturn;
}
