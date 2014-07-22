#include <TestRunner.h>
#include <gtest/gtest.h>

int main(int argc, char *argv[]) 
{ 
	QApplication app(argc, argv);
	if (RUN_ALL_QTESTS(argc, argv)>0)
	{
		return -1;
	}
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
