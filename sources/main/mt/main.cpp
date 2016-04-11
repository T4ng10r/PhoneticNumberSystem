#include <TestRunner.h>
#include <tools/loggers.h>
//#include <gtest/gtest.h>

int main(int argc, char *argv[]) 
{ 
  createLoggers();
	QApplication app(argc, argv);
  return RUN_ALL_QTESTS(argc, argv);

	if (RUN_ALL_QTESTS(argc, argv)>0)
	{
		return -1;
	}
	//::testing::InitGoogleTest(&argc, argv);
	//return RUN_ALL_TESTS();
}
