#include <Data/SearchResultTreeNode.h>
#include <boost/foreach.hpp>

void SearchResultTreeNode::clear()
{
	words.clear();
	children.clear();
	iCurrentIndex.reset();
}
void SearchResultTreeNode::find_node(unsigned int searchedNode, std::list<boost::shared_ptr<SearchResultTreeNode> > &foundNodes)
{
	if (searchedNode<*iCurrentIndex)
		return;
	if (*iCurrentIndex==searchedNode)
	{
		boost::shared_ptr<SearchResultTreeNode> node(shared_from_this());
		foundNodes.push_back(node);
		return;
	}
	BOOST_FOREACH(const ChildrenMap::value_type & childItem,children)
	{
		childItem.second->find_node(searchedNode, foundNodes);
	}
}
WordSearchResult SearchResultTreeNode::parseDFS()
{
	WordSearchResult result;

	BOOST_FOREACH(const ChildrenMap::value_type & childItem,children)
	{
		WordSearchResult childWords = childItem.second->parseDFS();
		std::copy(childWords.begin(),childWords.end(),back_inserter(result));
	}
	BOOST_FOREACH(const std::string & word,words)
	{
//		std::for_each(result.begin(),result.end(),front_inserter(result));
		//std::copy(childWords.begin(),childWords.end(),front_inserter(result));
		BOOST_FOREACH(SuccessWord & resultItem, result)
		{
			resultItem.words.push_front(word);
		}
	}
	return result;
}
