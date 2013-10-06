#include <Data/SearchResultTreeNode.h>
#include <boost/foreach.hpp>
#include <boost/bind.hpp>
#include <boost/range/adaptor/map.hpp>

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
	for(const EdgesList & edgesToSingleChild : children | boost::adaptors::map_values)
	{
		//currently -> list of words between here and child
		//get parseDFS words from child and add them to this list
		WordsList wordsOnEdges = edgesToSingleChild.first;
		WordSearchResult childWords = edgesToSingleChild.second->parseDFS();

		if (childWords.empty())
			std::copy(wordsOnEdges.begin(),wordsOnEdges.end(),back_inserter(result));
		else
			for(const std::string & word : wordsOnEdges)
			{
				for(SuccessWord & childWord : childWords)
				{
					SuccessWord newWord(word);
					std::copy(childWord.words.begin(),childWord.words.end(),back_inserter(newWord.words));
					result.push_back(newWord);
				}
			}
	}
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

