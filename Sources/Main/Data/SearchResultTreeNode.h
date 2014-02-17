#include <map>
#include <list>
#include <set>
#include <string>
#include <boost/optional.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <Data/CSubstituteSearchTypes.h>

struct SearchResultTreeNode;
//starting index of range
typedef boost::shared_ptr<SearchResultTreeNode>  SharedTreeNodes;
typedef std::list<SharedTreeNodes> TreeNodesList;

class EdgesList : public std::pair<WordsList, SharedTreeNodes>
{
public:
  WordsList & words_list() { return first; };
  const WordsList & cwords_list() const { return first; };
  SharedTreeNodes target_node() { return second; }
  void set_target_node(SharedTreeNodes node) { second = node; }
};

//first - ID of node to which this entry points
//second - list of words leading to given node
typedef std::map< StartingIndex, EdgesList >	ChildrenMap;

struct SearchResultTreeNode : public boost::enable_shared_from_this<SearchResultTreeNode>
{
public:
	SearchResultTreeNode(){}
	SearchResultTreeNode(StartingIndex index):iCurrentIndex(index){}
	void clear();
	void addNode(StartingIndex startInd, StartingIndex endInd, std::string word);
	TreeNodesList find_node(StartingIndex searchedNode);
	std::size_t getChildrenCount() { return children.size(); }

  WordsList parseDFS(StartingIndex endIndex);
public:
	//index of second value in range covered with this node words
	boost::optional<StartingIndex> iCurrentIndex;
	SharedTreeNodes	parent;
	ChildrenMap children;
	//WordsList words;
};
