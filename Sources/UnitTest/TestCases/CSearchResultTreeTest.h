#include <QtTest/QtTest>
#include <boost/shared_ptr.hpp>
#include <Data/SearchResultTreeNode.h>

class CSearchResultTreeNodeTest : public QObject
{
	Q_OBJECT
public:
	SharedTreeNodes root;
	void initialTree();
private Q_SLOTS:
	void init();
	void cleanup();
	void test_FindFirstLevel();
	void test_FindSecondLevel();
	void test_FindOnDiffLevel_01();
	void test_FindOnDiffLevel_02();
	void test_BuildTree_01();
	void test_BuildTree_02();
	void test_BuildTree_03();
};
