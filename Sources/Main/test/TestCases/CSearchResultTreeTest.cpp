#include <TestCases/CSearchResultTreeTest.h>
#include <boost/foreach.hpp>

#define CREATE_CONNECTION(base_node, new_node, index)\
	SharedTreeNodes new_node(new SearchResultTreeNode(index));\
	EdgesList & edges##new_node = base_node->children[new_node->iCurrentIndex.get()];\
	edges##new_node.second=new_node;


void CSearchResultTreeNodeTest ::init()
{
	root.reset(new SearchResultTreeNode(0));
}

SharedTreeNodes createConnection(SharedTreeNodes base_node, StartingIndex index, std::string word)
{
  SharedTreeNodes new_node(new SearchResultTreeNode(index));
  EdgesList & node_edges = base_node->children[new_node->iCurrentIndex.get()];
  node_edges.words_list().push_back(word);
  node_edges.set_target_node(new_node);
  return new_node;
}
void CSearchResultTreeNodeTest::initialTreeForFindTests()
{
  //connection between root and node (3)
  SharedTreeNodes node_1_3 = createConnection(root, 3, "root_child_3");
  //connection between node (3) and node (3)(4)
  SharedTreeNodes node_3_4 = createConnection(node_1_3, 4, "root_3_child_4");
  //connection between node (3) and node (3)(5)
  SharedTreeNodes node_3_5 = createConnection(node_1_3, 5, "root_3_child_5");

  SharedTreeNodes node_1_4 = createConnection(root, 4, "root_child_4");
  SharedTreeNodes node_2_3 = createConnection(node_1_4, 5, "root_4_child_5");
  SharedTreeNodes node_2_4 = createConnection(node_1_4, 6, "root_4_child_6");

  SharedTreeNodes node_1_5 = createConnection(root, 5, "root_child_5");
  SharedTreeNodes node_5_7 = createConnection(node_1_5, 7, "root_5_child_7");
  SharedTreeNodes node_5_8 = createConnection(node_1_5, 8, "root_5_child_8");
};
void CSearchResultTreeNodeTest ::initialTreeForParseDFS()
{
	std::string firstWord("First");
	std::string secondWord("Second");
	std::string thirdWord("Third");
	std::string fourthWord("Fourth");
	root->addNode(0,0,firstWord);
	root->addNode(0,0,secondWord);
	root->addNode(0,0,thirdWord);
	root->addNode(0,0,fourthWord);
}
void CSearchResultTreeNodeTest ::cleanup()
{
	root.reset();
}
//add word covering first and only first digit
void CSearchResultTreeNodeTest::test_BuildTree_01()
{
	std::string firstWord("First");
	root->addNode(0,0,firstWord);

	QCOMPARE(root->getChildrenCount(),(std::size_t)1);
	//test amount of added word on edge (0,0)
	QCOMPARE(root->children[1].first.size(),(std::size_t)1);
	TreeNodesList foundNodes = root->find_node(0);
	QCOMPARE(foundNodes.size(),(std::size_t)1);
	QCOMPARE(foundNodes.front().get(),root.get());
	foundNodes = root->find_node(1);
	QCOMPARE(foundNodes.size(),(std::size_t)1);
	QCOMPARE(root->children[1].second.get(),foundNodes.front().get());
}
void CSearchResultTreeNodeTest::test_BuildTree_02()
{
	std::string firstWord("First");
	std::string secondWord("Second");
	root->addNode(0,0,firstWord);
	root->addNode(1,1,secondWord);
	QCOMPARE(root->children.size(),(std::size_t)1);
	//second node with Startingindex=1 - starting point for second digit
	TreeNodesList foundNodes = root->find_node(1);
	QCOMPARE(foundNodes.size(),(std::size_t)1);
	EdgesList & edges_1 = foundNodes.front()->children[2];
	QCOMPARE(edges_1.first.size(),(std::size_t)1);
	QCOMPARE(edges_1.first.front().getWord(),secondWord);
	foundNodes = root->find_node(2);
	QCOMPARE(edges_1.second.get(),foundNodes.front().get());
}
void CSearchResultTreeNodeTest::test_BuildTree_03()
{
	std::string firstWord("First");
	std::string firstWord2("First First");
	std::string firstWord3("First2");

	root->addNode(0,0,firstWord);
	root->addNode(0,0,firstWord3);
	root->addNode(0,2,firstWord2);
	QCOMPARE(root->children.size(),(std::size_t)2);
	EdgesList & edges_0_0 = root->children[1];
	QCOMPARE(edges_0_0.first.size(),(std::size_t)2);
	QCOMPARE(edges_0_0.first.front().getWord(),firstWord);
	QCOMPARE(edges_0_0.first.back().getWord(),firstWord3);
	EdgesList & edges_0_2 = root->children[3];
	QCOMPARE(edges_0_2.first.size(),(std::size_t)1);
	QCOMPARE(edges_0_2.first.front().getWord(),firstWord2);
}
void CSearchResultTreeNodeTest ::test_FindFirstLevel()
{
	initialTreeForFindTests();
	TreeNodesList foundNodes = root->find_node(0);
	QCOMPARE(foundNodes.size(),(std::size_t)1);
	QCOMPARE(foundNodes.front().get(),root.get());
}
void CSearchResultTreeNodeTest ::test_FindSecondLevel()
{
	initialTreeForFindTests();
	TreeNodesList foundNodes = root->find_node(3);
	QCOMPARE(foundNodes.size(),(std::size_t)1);
	QCOMPARE(foundNodes.front()->iCurrentIndex.get(),(unsigned int)3);
}
void CSearchResultTreeNodeTest ::test_FindOnDiffLevel_01()
{
	initialTreeForFindTests();
	TreeNodesList foundNodes = root->find_node(4);
	QCOMPARE(foundNodes.size(),(std::size_t)2);
	QCOMPARE(foundNodes.front()->iCurrentIndex.get(),(unsigned int)4);
	QCOMPARE(foundNodes.back()->iCurrentIndex.get(),(unsigned int)4);
}
void CSearchResultTreeNodeTest ::test_FindOnDiffLevel_02()
{
	initialTreeForFindTests();
	TreeNodesList foundNodes = root->find_node(5);
	QCOMPARE(foundNodes.size(),(std::size_t)3);
	QCOMPARE(foundNodes.front()->iCurrentIndex.get(),(unsigned int)5);
	QCOMPARE(foundNodes.back()->iCurrentIndex.get(),(unsigned int)5);
}
