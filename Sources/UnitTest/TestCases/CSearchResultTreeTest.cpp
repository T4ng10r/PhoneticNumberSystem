#include <TestCases/CSearchResultTreeTest.h>
#include <boost/foreach.hpp>

void CSearchResultTreeNodeTest ::init()
{
	root.reset(new SearchResultTreeNode(0));
	//////////////////////////////////////////////////////////////////////////
	//create second Level nodes
	boost::shared_ptr<SearchResultTreeNode> node1_1(new SearchResultTreeNode(3));
	root->children[node1_1->iCurrentIndex.get()]=node1_1;
	{
		boost::shared_ptr<SearchResultTreeNode> node2_1(new SearchResultTreeNode(4));
		node1_1->children[node2_1->iCurrentIndex.get()]=node2_1;

		boost::shared_ptr<SearchResultTreeNode> node3_1(new SearchResultTreeNode(5));
		node1_1->children[node3_1->iCurrentIndex.get()]=node3_1;
	}
	boost::shared_ptr<SearchResultTreeNode> node1_2(new SearchResultTreeNode(4));
	root->children[node1_2->iCurrentIndex.get()]=node1_2;
	{
		boost::shared_ptr<SearchResultTreeNode> node2_2(new SearchResultTreeNode(5));
		node1_2->children[node2_2->iCurrentIndex.get()]=node2_2;

		boost::shared_ptr<SearchResultTreeNode> node3_2(new SearchResultTreeNode(6));
		node1_2->children[node3_2->iCurrentIndex.get()]=node3_2;
	}

	boost::shared_ptr<SearchResultTreeNode> node1_3(new SearchResultTreeNode(5));
	root->children[node1_3->iCurrentIndex.get()]=node1_3;
	{
		boost::shared_ptr<SearchResultTreeNode> node2_3(new SearchResultTreeNode(7));
		node1_3->children[node2_3->iCurrentIndex.get()]=node2_3;

		boost::shared_ptr<SearchResultTreeNode> node3_3(new SearchResultTreeNode(8));
		node1_3->children[node3_3->iCurrentIndex.get()]=node3_3;
	}
	//////////////////////////////////////////////////////////////////////////
}
void CSearchResultTreeNodeTest ::cleanup()
{
	root.reset();
}
void CSearchResultTreeNodeTest ::test_FindFirstLevel()
{
	std::list<boost::shared_ptr<SearchResultTreeNode> > foundNodes;
	root->find_node(0,foundNodes);
	QCOMPARE(foundNodes.size(),(std::size_t)1);
	QCOMPARE(foundNodes.front().get(),root.get());
}
void CSearchResultTreeNodeTest ::test_FindSecondLevel()
{
	std::list<boost::shared_ptr<SearchResultTreeNode> > foundNodes;
	root->find_node(3,foundNodes);
	QCOMPARE(foundNodes.size(),(std::size_t)1);
	QCOMPARE(foundNodes.front()->iCurrentIndex.get(),(unsigned int)3);
}
void CSearchResultTreeNodeTest ::test_FindOnDiffLevel_01()
{
	std::list<boost::shared_ptr<SearchResultTreeNode> > foundNodes;
	root->find_node(4,foundNodes);
	QCOMPARE(foundNodes.size(),(std::size_t)2);
	QCOMPARE(foundNodes.front()->iCurrentIndex.get(),(unsigned int)4);
	QCOMPARE(foundNodes.back()->iCurrentIndex.get(),(unsigned int)4);
}
void CSearchResultTreeNodeTest ::test_FindOnDiffLevel_02()
{
	std::list<boost::shared_ptr<SearchResultTreeNode> > foundNodes;
	root->find_node(5,foundNodes);
	QCOMPARE(foundNodes.size(),(std::size_t)3);
	QCOMPARE(foundNodes.front()->iCurrentIndex.get(),(unsigned int)5);
	QCOMPARE(foundNodes.back()->iCurrentIndex.get(),(unsigned int)5);
}
