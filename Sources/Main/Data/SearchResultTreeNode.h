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
typedef unsigned int StartingIndex;
typedef boost::shared_ptr<SearchResultTreeNode>  SharedTreeNodes;
typedef std::list<SharedTreeNodes> TreeNodesList;
typedef std::list<SuccessWord>	WordsList;

class EdgesList : public std::pair<WordsList, SharedTreeNodes>
{
public:
  WordsList & words_list() { return first; };
  const WordsList & cwords_list() const { return first; };
  SharedTreeNodes target_node() { return second; }
  void set_target_node(SharedTreeNodes node) { second = node; }
};
//typedef std::pair<WordsList, SharedTreeNodes> EdgesList;
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
  std::size_t getChildrenCount() { return children.size(); }

  WordSearchResult parseDFS(unsigned int endIndex);
public:
	//index of second value in range covered with this node words
	boost::optional<StartingIndex> iCurrentIndex;
	SharedTreeNodes	parent;
	ChildrenMap children;
	//WordsList words;
};
