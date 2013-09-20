#include <map>
#include <list>
#include <string>
#include <boost/shared_ptr.hpp>
#include <boost/optional.hpp>
#include <boost/enable_shared_from_this.hpp>

struct SearchResultTreeNode : public boost::enable_shared_from_this<SearchResultTreeNode>
{
public:
	typedef unsigned int NodesIndex;

	SearchResultTreeNode(){}
	SearchResultTreeNode(NodesIndex index):iCurrentIndex(index){}
	void clear();
	void find_node(unsigned int searchedNode, std::list<boost::shared_ptr<SearchResultTreeNode> > &foundNodes);
public:
	typedef std::map< NodesIndex, boost::shared_ptr<SearchResultTreeNode> >	ChildrenMap;
	//index of second value in range covered with this node words
	boost::optional<NodesIndex> iCurrentIndex;
	boost::shared_ptr<SearchResultTreeNode>	parent;
	ChildrenMap children;
	std::list<std::string>  words;
};
