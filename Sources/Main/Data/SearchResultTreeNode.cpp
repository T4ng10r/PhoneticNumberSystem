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
		result.push_back(shared_from_this());
		return result;
	}
	BOOST_FOREACH(const ChildrenMap::value_type & childItem,children)
	{
		TreeNodesList childResult = childItem.second.second->find_node(searchedNode);
 		result.merge(childResult);
	}
	return result;
}
WordSearchResult SearchResultTreeNode::parseDFS( unsigned int endIndex )
{
	WordSearchResult result;
	//pass through all edges
    BOOST_FOREACH(const EdgesList & edgesToSingleChild , children | boost::adaptors::map_values)
    //for(const EdgesList & edgesToSingleChild : children | boost::adaptors::map_values)
	{
		//currently -> list of words between here and child
		//get parseDFS words from child and add them to this list
		WordsList wordsOnEdge = edgesToSingleChild.cwords_list();
		WordSearchResult childWords = edgesToSingleChild.second->parseDFS(endIndex);
		if (*iCurrentIndex<(endIndex-1) && childWords.empty())
			continue;

		if (childWords.empty())
			std::copy(wordsOnEdge.begin(),wordsOnEdge.end(),back_inserter(result));
		else
			//this coping section takes too much time
            BOOST_FOREACH(const std::string & word , wordsOnEdge)
            //for(const std::string & word : wordsOnEdge)
			{
                BOOST_FOREACH(const SuccessWord & childWord, childWords)
                //for(SuccessWord & childWord : childWords)
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

    BOOST_FOREACH(SharedTreeNodes & startNode , startNodesList)
    //for(SharedTreeNodes & startNode : startNodesList)
	{
		EdgesList & edges = startNode->children[endInd+1];
		edges.words_list().push_back(word);
		if (!edges.target_node())
			edges.set_target_node(targetNode);
	}
}

