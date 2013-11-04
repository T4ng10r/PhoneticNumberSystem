#include <map>
#include <list>
#include <string>
#include <boost/optional.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <Data/CSubstituteSearchTypes.h>

struct SearchResultTreeNode;
//starting index of range
typedef unsigned int StartingIndex;
typedef boost::shared_ptr<SearchResultTreeNode>  SharedTreeNodes;
typedef std::list<SharedTreeNodes> TreeNodesList;
typedef std::list<std::string>	WordsList;
typedef std::pair<WordsList, SharedTreeNodes> EdgesList;
typedef std::map< StartingIndex, EdgesList >	ChildrenMap;

struct SearchResultTreeNode : public boost::enable_shared_from_this<SearchResultTreeNode>
{
public:
	SearchResultTreeNode(){}
	SearchResultTreeNode(StartingIndex index):iCurrentIndex(index){}
	void clear();
	void addNode(StartingIndex startInd, StartingIndex endInd, std::string word);
	//void find_node(unsigned int searchedNode, std::list<boost::shared_ptr<SearchResultTreeNode> > &foundNodes);
	TreeNodesList find_node(unsigned int searchedNode);
	WordSearchResult parseDFS(unsigned int endIndex);
public:
	//index of second value in range covered with this node words
	boost::optional<StartingIndex> iCurrentIndex;
	SharedTreeNodes	parent;
	ChildrenMap children;
	//WordsList words;
};
