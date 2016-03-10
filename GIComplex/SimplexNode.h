/*
(c) 2012 Fengtao Fan
*/
#ifndef _SIMPLICIAL_TREE_NODE_H_
#define _SIMPLICIAL_TREE_NODE_H_

/* boost shared pointer */
#include <boost/shared_ptr.hpp>

#include <map>

/* declaration of simpicial tree node*/

class SimplicialTreeNode;

/* shared pointer to simplicial tree node */

typedef boost::shared_ptr<SimplicialTreeNode> SimplicialTreeNode_ptr;

/* sibling structure for each node */

typedef std::map<int, SimplicialTreeNode_ptr> Map_int_stnPtr;

/* shared pointer to <int, stn_ptr> mapping*/

typedef boost::shared_ptr<Map_int_stnPtr> Map_int_stnPtr_ptr;

class SimplicialTreeNode
{
public:
	// constructor
	SimplicialTreeNode() :
		last_label(-1),
		index_in_filtration(-1),
		parent_ptr(),
		children_map_ptr(),
		next_circular_ptr() //, prev_circular_ptr()
	{
	}

	SimplicialTreeNode(const int v_index,
						const int idx_filtration,
		SimplicialTreeNode_ptr  in_parent_ptr, //& changed for ubuntu
		Map_int_stnPtr_ptr  in_children_map_ptr, //&
		SimplicialTreeNode_ptr in_next_circular_ptr ) : //&
				last_label(v_index),
				index_in_filtration(idx_filtration),
				parent_ptr(in_parent_ptr),
				children_map_ptr(in_children_map_ptr),
				next_circular_ptr(in_next_circular_ptr)
	{
	}

public:
	int last_label;
	int index_in_filtration;
	SimplicialTreeNode_ptr parent_ptr;
	Map_int_stnPtr_ptr children_map_ptr;
	SimplicialTreeNode_ptr next_circular_ptr;
};

#endif // _SIMPLICIAL_TREE_NODE_H_
