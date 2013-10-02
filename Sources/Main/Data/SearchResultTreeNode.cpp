#include <Data/SearchResultTreeNode.h>
#include <boost/foreach.hpp>

void SearchResultTreeNode::clear()
{
	//words.clear();
	children.clear();
	iCurrentIndex.reset();
}
TreeNodesList SearchResultTreeNode::find_node(unsigned int searchedNode)
{
	TreeNodesList result;
	if (searchedNode<*iCurrentIndex)
		return result;
	if (*iCurrentIndex==searchedNode)
	{
		SharedTreeNodes node(shared_from_this());
		result.push_back(node);
		return result;
	}
	BOOST_FOREACH(const ChildrenMap::value_type & childItem,children)
	{
		TreeNodesList childResult = childItem.second.second->find_node(searchedNode);
 		result.merge(childResult);
	}
	return result;
}
WordSearchResult SearchResultTreeNode::parseDFS()
{
	WordSearchResult result;
	//pass through all edges
	BOOST_FOREACH(const ChildrenMap::value_type & childItem,children)
	{
		//childItem.first
		//WordSearchResult childWords = childItem.second->parseDFS();
		//std::copy(childWords.begin(),childWords.end(),back_inserter(result));
	}
	//BOOST_FOREACH(const std::string & word,words)
	//{
	//	//std::for_each(result.begin(),result.end(),front_inserter(result));
	//	//std::copy(childWords.begin(),childWords.end(),front_inserter(result));
	//	BOOST_FOREACH(SuccessWord & resultItem, result)
	//	{
	//		resultItem.words.push_front(word);
	//	}
	//}
	return result;
}
void SearchResultTreeNode::addNode( StartingIndex startInd, StartingIndex endInd, std::string word )
{
	TreeNodesList startNodesList = find_node(startInd);
	TreeNodesList targetNodesList = find_node(endInd+1);
	SharedTreeNodes targetNode;
	if (targetNodesList.empty())
		targetNode.reset(new SearchResultTreeNode(endInd+1));
	else
		targetNode = targetNodesList.front();

	for(SharedTreeNodes & startNode : startNodesList)
	{
		EdgesList & edges = startNode->children[endInd+1];
		edges.first.push_back(word);
		if (!edges.second)
			edges.second=targetNode;
	}
}

