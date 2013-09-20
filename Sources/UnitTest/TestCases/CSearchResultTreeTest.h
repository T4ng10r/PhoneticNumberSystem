#include <QtTest/QtTest>
#include <boost/shared_ptr.hpp>
#include <Data/SearchResultTreeNode.h>

class CSearchResultTreeNodeTest : public QObject
{
	Q_OBJECT
public:
	boost::shared_ptr<SearchResultTreeNode> root;
private Q_SLOTS:
	void init();
	void cleanup();
	void test_FindFirstLevel();
	void test_FindSecondLevel();
	void test_FindOnDiffLevel_01();
	void test_FindOnDiffLevel_02();
};
