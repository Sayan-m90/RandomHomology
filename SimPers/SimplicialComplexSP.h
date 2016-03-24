/*
(c) 2015 Fengtao Fan, Dayu Shi
*/
#ifndef _SIMPLICIAL_TREE_H_
#define _SIMPLICIAL_TREE_H_

#include "SimplexNodeSP.h"

#include <fstream>
#include <string>
#include <sstream>
#include <iostream>
#include <cstring>
#include <boost/unordered_set.hpp>
#include <boost/unordered_map.hpp>

#define MAX_DIM 100

typedef boost::shared_ptr<AnnotationMatrix> AnnotationMatrixPtr;
typedef boost::shared_ptr<std::unordered_map<int, SimplicialTreeNode_ptr> > LabelsDictionaryPtr;

extern vector<float> vecFiltrationScale;
extern int time_in_each_filtration_step;

/* declaration of simpicial tree*/
template<class T>
class SimplicialTree
{
public:
	/*constructor */
	// default constructor
	SimplicialTree() : dim(-1), EuclideanDataPtr(NULL)
	{
		vecTS.assign(MAX_DIM, 0);
	}
	//
	void UpdateAnnotationArray(const int simplex_dim);
	ListNodePtr find_annotation(SimplicialTreeNode_ptr p);
	SimplicialTreeNode_ptr InsertSimplexWithAnnotation(std::vector<int> &simplex_vertices, ListNodePtr simplex_ann);
	SimplicialTreeNode_ptr ElementaryInsersion(std::vector<int> &simplex_vertices);
	/*deconstructor */
	~SimplicialTree()
	{}
	// size statistics of the complex
	int SimplexDim(const SimplicialTreeNode_ptr simplex);
	int ComplexSize();
	int dDimSimplicesSize(const int d);
	void Report();
	// 
	void retrieve_vertex_indices(const SimplicialTreeNode_ptr &simplex, vector<int> &simplex_vertices);
	SimplicialTreeNode_ptr find(vector<int> &simplex_vertices);
	void insert_into_circular_list(const int simplex_dim, const SimplicialTreeNode_ptr & simplex);
	SimplicialTreeNode_ptr insert_into_simplicial_tree(std::vector<int> &simplex_vertices);
	// retrieve boundary
	bool Boundary(const SimplicialTreeNode_ptr& sigma, std::vector<SimplicialTreeNode_ptr> &bdries);
	// retrieve coboundary
	bool is_link_condition_satisfied(SimplicialTreeNode_ptr &edge, boost::unordered_set<SimplicialTreeNode_ptr> &intersectedLinks);
	bool is_coface(SimplicialTreeNode_ptr &sigma, vector<int> &simplex_vertices);
	void retrieve_simplices_from_subtrees(const SimplicialTreeNode_ptr &sigma, vector<SimplicialTreeNode_ptr> &coBdries, bool dfs_visit = false);
	void CoDimensionOneFaces(const SimplicialTreeNode_ptr& sigma, std::vector<SimplicialTreeNode_ptr> &codim_one_faces);
	bool CoBoundary(const SimplicialTreeNode_ptr &sigma, std::vector<SimplicialTreeNode_ptr> &coBdries, bool dfs_visit = false);
	void LinkSubcomplex(const SimplicialTreeNode_ptr &sigma, std::vector<SimplicialTreeNode_ptr> &simplexLink);
	void LinsubcomplexIntersection(SimplicialTreeNode_ptr & a, SimplicialTreeNode_ptr & b, boost::unordered_set<SimplicialTreeNode_ptr> &intersectedLinks);
	// cone over the subcomplexes
	void AddExtraSimplicesToSatisfyLinkCondition(SimplicialTreeNode_ptr &edge, boost::unordered_set<SimplicialTreeNode_ptr> &intersectedLinks);
	void EdgeContraction(SimplicialTreeNode_ptr &edge, const int preserve_vertex_label);
	void remove_simplex_from_both_complex_and_ufdForest(SimplicialTreeNode_ptr sigma, bool bUpdatePers = true);
	void remove_simplex_from_complex(SimplicialTreeNode_ptr sigma);
	void rename_simplices_in_subtree(SimplicialTreeNode_ptr sigma, const int new_label);
	// annotation transfer 
	void AnnotationTransfer(SimplicialTreeNode_ptr &edge, const int preserve_vertex_label);
	// construct the complex
	//virtual bool Construction() = 0; 
	void ElementaryCollapse(const int remove_label, const int preserve_label);
	/* ------------- simplicial complex I/O ---------------- */
	string annotation_to_string(SimplicialTreeNode_ptr sigma);
	ListNodePtr string_to_annotation(string &line);
	void ReadComplex(const char* pFileName);
	void ReadComplexWithAnnotation(const char* pFileName);
	void ReadSimplicialMap(const char* pFileName, vector<pair<int, int> > &vertex_map);
	void WriteComplex(const char* pFileName);
	void WriteComplexWithAnnotation(const char* pFileName);
	void PrintComplexWithAnnotation();
	//
	void WriteStatisticsToFile(const char* pFileName);
	//
	/* ----------------persistence related functions ---------------*/
	void SnapshotHomologicalFeatures(vector<std::unordered_set<int> > &hom_info);
	void PerformSimplicialCollapse(vector<pair<int, int> > &vertex_map, vector<int> &updated_vertex_map);
	void InitializeByRenamingIncomingComplex(SimplicialTree<T> &src, vector<int> &vertex_map);
	//relabeling instead of copying
	void RelabelingVertices(std::unordered_map<int, int> &vertex_map);
	void AddRemainingSimpliciesFromFile(const char* pFileName);
	void CheckPersistence(vector<std::unordered_set<int> > &homo_info);
	//

public:
	T* EuclideanDataPtr; // data used for constructing this complex, like the underlying graph for Rips complex
	int dim; // dimension of the simpicial complex  
	std::vector<int> simplex_sizes;
	//
	std::vector<LabelsDictionaryPtr> labels_dict_in_each_dim; // circular list for each 
	// union-find-deletion data structure
	UnionFindDeletion ufd;
	// Annotation in each dimension
	std::vector<AnnotationMatrixPtr> annotations;
	//max index of image vertices in vertex map
	int maxImageVertex;
	//time stamps in each dimension
	std::vector<long long> vecTS;

	//clear function
	void clearData()
	{
		this->dim = 0;
		this->simplex_sizes.clear();
		this->labels_dict_in_each_dim.clear();
		//ufd
		this->annotations.clear();
		this->vecTS.assign(MAX_DIM, 0);
	}
	void clearMemory();		//clear memory of annotation matrices, simplicial trees and udf trees

	bool check_parent_children(SimplicialTreeNode_ptr sigma) {
		for (std::map<int, SimplicialTreeNode_ptr>::iterator mIter = sigma->children.begin();
			mIter != sigma->children.end(); ++mIter) {
			if (mIter->first != mIter->second->label) {
				cout << "label is not consistent " << endl;
				return false;
			}
			if (mIter->second->parent != sigma) {
				cout << "parent is not consistent" << endl;
				return false;
			}
		}
		//int simplex_dim = SimplexDim(sigma);
		//if(annotations[simplex_dim]->ann_mat.find(find_annotation(sigma)) == annotations[simplex_dim]->ann_mat.end()) {
		//	cout << "sigma annotation is not found" << endl;
		//	return false;
		//}
		return true;
	}
	bool check_ufd_elem(SimplicialTreeNode_ptr sigma) {
		if (!sigma->tree_node) {
			cout << "empty tree_node" << endl;
			return false;
		}
		else {
			if (sigma->tree_node->elem != sigma) {
				while (sigma) {
					cout << sigma->label << " ";
					sigma = sigma->parent;
				}
				cout << endl;
				cout << "tree node is not consistent" << endl;
				return false;
			}
		}
		return true;
	}
	bool check_ufd_anno() {
		for (int i = 0; i < annotations.size(); ++i) {
			std::unordered_map<ListNodePtr, TreeRootNodePtr, hash_ListNodePtr, equal_ListNodePtr>::iterator  mIter = annotations[i]->ann_mat.begin();
			for (; mIter != annotations[i]->ann_mat.end(); ++mIter) {
				if (mIter->second->attribute != mIter->first) {
					cout << "ufd and anno is not consistent" << endl;
					return false;
				}
			}
		}
		return true;
	}
	int check_status() {
		// check children and parent
		// check tree_node and elem
		// check root and annotation
		cout << "check status" << endl;
		check_ufd_anno();
		vector<int> simplex_vertices;
		simplex_vertices.reserve(dim + 1);
		// visit each simplex through the labels_diction variables
		for (int i = 0; i < labels_dict_in_each_dim.size(); ++i) {

			std::unordered_map<int, SimplicialTreeNode_ptr> & labels_dict = *(labels_dict_in_each_dim[i]);
			//
			std::unordered_map<int, SimplicialTreeNode_ptr>::iterator mIter = labels_dict.begin();
			if (i == 0) {
				// handle vertices
				for (; mIter != labels_dict.end(); ++mIter) {
					//
					if (!check_parent_children(mIter->second) || !check_ufd_elem(mIter->second)) {
						exit(0);
					}
				}
			}
			else {
				for (; mIter != labels_dict.end(); ++mIter) {
					SimplicialTreeNode_ptr trav = mIter->second;
					do{
						if (!check_parent_children(trav) || !check_ufd_elem(trav)) {
							exit(0);
						}
						//
						trav = trav->next_circular_ptr;
					} while (trav != mIter->second);
				}
			}
		}
		return 1;

	}
private:
	struct SimplicialTreeNodePtrLessThan {
		bool operator()(const SimplicialTreeNode_ptr lhs, const SimplicialTreeNode_ptr rhs) const {
			int lhs_size = 0, rhs_size = 0;
			SimplicialTreeNode_ptr trav = lhs;
			while (trav) {
				++lhs_size;
				trav = trav->parent;
			}
			trav = rhs;
			while (trav) {
				++rhs_size;
				trav = trav->parent;
			}
			return lhs_size < rhs_size;
		}
	};
	void merge_two_sorted_arrays(vector<int> &A, vector<int> &B, vector<int> &res) {
		res.resize(A.size() + B.size());
		int a = (int)A.size() - 1, b = (int)B.size() - 1, c = (int)res.size() - 1;
		while (a >= 0 && b >= 0) {
			res[c--] = max(A[a], B[b]);
			A[a] > B[b] ? --a : --b;
		}
		while (a >= 0) {
			res[c--] = A[a--];
		}
		while (b >= 0) {
			res[c--] = B[b--];
		}
		return;
	}
	void delete_from_circular_list(SimplicialTreeNode_ptr &sigma, const int simplex_dim) {
		std::unordered_map<int, SimplicialTreeNode_ptr> &head = (*labels_dict_in_each_dim[simplex_dim]);
		if (simplex_dim == 0 || sigma->next_circular_ptr == sigma) {
			// the last label in the this row 
			head.erase(sigma->label);
		}
		else {
			SimplicialTreeNode_ptr prev = head[sigma->label];
			while (prev->next_circular_ptr != sigma) {
				prev = prev->next_circular_ptr;
			}
			prev->next_circular_ptr = sigma->next_circular_ptr;
			if (head[sigma->label] == sigma) {
				// 
				head[sigma->label] = sigma->next_circular_ptr;
			}
		}
		sigma->next_circular_ptr.reset();
	}
	//
	bool is_zero_annotation(ListNodePtr &ptr) {
		if (!ptr) {
			return false; // empty 
		}
		return (ptr->next == ptr);
	}
};

template<typename T>
inline int SimplicialTree<T>::SimplexDim(const SimplicialTreeNode_ptr simplex) {
	int simplex_dim = -1;
	SimplicialTreeNode_ptr trav = simplex;
	while (trav) {
		++simplex_dim;
		trav = trav->parent;
	}
	return simplex_dim;
}
template<typename T>
inline int SimplicialTree<T>::ComplexSize()
{
	int total_size = 0;
	for (int i = 0; i < simplex_sizes.size(); i++){
		total_size += simplex_sizes[i];
	}
	return total_size;
}

template<typename T>
inline int SimplicialTree<T>::dDimSimplicesSize(const int d)
{
	if (d > dim || d < 0)
	{
		std::cout << "Querying dimension exceeding the Complex Dimension " << std::endl;
		return  -1;
	}
	return simplex_sizes[d];
}

template<typename T>
inline void SimplicialTree<T>::Report()
{
	for (int i = 0; i < dim + 1; i++)
	{
		std::cout << "DIM " << i << " : " << simplex_sizes[i] << std::endl;
	}
	std::cout << "total size : " << ComplexSize() << std::endl;
	return;
}
template<typename T>
ListNodePtr SimplicialTree<T>::find_annotation(SimplicialTreeNode_ptr p){
	if (p->tree_node) {
		TreeRootNodePtr root = ufd.Find(p->tree_node);
		return root->attribute;
	}
	else {
		cout << "Simplex has no annotation available" << endl;
		exit(0);
	}
	return ListNodePtr();
}
template<typename T>
ListNodePtr SimplicialTree<T>::string_to_annotation(string &line) {
	int start = 0;
	while (start < line.size() && line[start] != '[') {
		++start;
	}
	if (start == line.size()) {
		// invalid annoation
		return ListNodePtr();
	}
	int nonzero_bits = 0;
	// make dummy head
	ListNodePtr dummy_head = boost::make_shared<ListNode>();
	ListNodePtr trav = dummy_head;
	//
	stringstream sstr(stringstream::in);
	//
	int row = 0, val = 0;
	for (int i = start; i < line.size(); ++i) {
		if (line[i] == '<') {
			start = i;
		}
		else {
			if (line[i] == '>') {

				sstr.str(string(line.begin() + start + 1, line.begin() + i));
				//cout << sstr.str() << endl;
				sstr >> row >> val;
				//
				trav->next = boost::make_shared<ListNode>(row, val);
				trav = trav->next;
				//
				sstr.str("");
				sstr.clear();
			}
		}
	}
	trav->next = dummy_head;
	//
	return dummy_head;
}
template<typename T>
string SimplicialTree<T>::annotation_to_string(SimplicialTreeNode_ptr sigma) {
	std::stringstream sstr(std::stringstream::in | std::stringstream::out);
	ListNodePtr ann = find_annotation(sigma);
	if (!ann) {
		cout << "annotation is not valid " << endl;
		exit(0);
	}
	ListNodePtr trav(ann->next);
	sstr << "[";
	while (trav != ann) {
		sstr << "<" << trav->row << " " << 1 << ">" << (trav->next == ann ? "" : " ");
		trav = trav->next;
	}
	sstr << "]";
	return sstr.str();
}
template<typename T>
void SimplicialTree<T>::retrieve_vertex_indices(const SimplicialTreeNode_ptr &simplex, vector<int> &simplex_vertices) {
	if (simplex) {
		SimplicialTreeNode_ptr trav = simplex;
		while (trav) {
			simplex_vertices.push_back(trav->label);
			trav = trav->parent;
		}
		reverse(simplex_vertices.begin(), simplex_vertices.end());
	}
	return;
}
template<typename T>
SimplicialTreeNode_ptr SimplicialTree<T>::find(vector<int> &simplex_vertices){
	// preq: numbers in simplex_vertices are sorted either in increasing order
	if (simplex_vertices.empty() || labels_dict_in_each_dim.empty()) {
		// no vertices in input
		// or the complex is empty 
		return SimplicialTreeNode_ptr();
	}
	int label = simplex_vertices.front();
	LabelsDictionaryPtr vertices = labels_dict_in_each_dim.front();
	SimplicialTreeNode_ptr pIter = (vertices->find(label) == vertices->end() ? SimplicialTreeNode_ptr() : (*vertices)[label]);
	if (pIter) {
		for (int i = 1; i < simplex_vertices.size(); ++i) {
			label = simplex_vertices[i];
			if (pIter->children.empty() || pIter->children.find(label) == pIter->children.end()) {
				return SimplicialTreeNode_ptr();
			}
			pIter = pIter->children[label];
		}
	}
	return pIter;
}
template<typename T>
SimplicialTreeNode_ptr SimplicialTree<T>::insert_into_simplicial_tree(std::vector<int> &simplex_vertices) {
	// preq:	1) simplex_vertices is sorted
	//			2) all boundaries are inserted already
	//			3) this is a new simplex
	const int simplex_dim = (int)simplex_vertices.size() - 1;
	//
	if (simplex_sizes.size() <= simplex_dim) {
		simplex_sizes.push_back(1);
		// creater the dictonary (hash) when insert the first element
		labels_dict_in_each_dim.push_back(boost::make_shared<unordered_map<int, SimplicialTreeNode_ptr> >());
		++dim;
	}
	else {
		++simplex_sizes[simplex_dim];
	}
	//
	unordered_map<int, SimplicialTreeNode_ptr> & vertices = (*labels_dict_in_each_dim.front());
	//
	if (simplex_dim == 0) {
		int ver_num = simplex_vertices[0];
		// it is a new vertex
		//SimplicialTreeNode_ptr simplex(boost::make_shared<SimplicialTreeNode>(ver_num, ver_num));
		SimplicialTreeNode_ptr simplex(boost::make_shared<SimplicialTreeNode>(ver_num, ++time_in_each_filtration_step));
		vertices[ver_num] = simplex;
		//
		return vertices[ver_num];
	}

	SimplicialTreeNode_ptr pIter = vertices[simplex_vertices[0]];
	for (int i = 1; i < simplex_vertices.size() - 1; ++i) {
		pIter = pIter->children[simplex_vertices[i]];
	}
	if (pIter->children.find(simplex_vertices.back()) == pIter->children.end()) {
		// this simplex is not existed
		SimplicialTreeNode_ptr simplex(boost::make_shared<SimplicialTreeNode>(simplex_vertices.back(), ++time_in_each_filtration_step));
		pIter->children[simplex_vertices.back()] = simplex;
		// 
		simplex->parent = pIter;
		//
		insert_into_circular_list(simplex_dim, simplex);
		//
	}
	return pIter->children[simplex_vertices.back()];
}
template<typename T>
void SimplicialTree<T>::UpdateAnnotationArray(const int simplex_dim) {
	if (annotations.size() < simplex_dim + 1) {
		annotations.push_back(boost::make_shared<AnnotationMatrix>());
		annotations.back()->timeStamp = this->vecTS[simplex_dim];
		annotations.back()->annoDim = simplex_dim;
	}
	return;
}

template<typename T>
SimplicialTreeNode_ptr SimplicialTree<T>::InsertSimplexWithAnnotation(std::vector<int> &simplex_vertices, ListNodePtr simplex_ann) {
	SimplicialTreeNode_ptr simplex = find(simplex_vertices);
	if (!simplex) {
		// new simplex
		// 1) insert it into the simplicial tree
		simplex = insert_into_simplicial_tree(simplex_vertices);
		//
		int simplex_dim = (int)simplex_vertices.size() - 1;
		UpdateAnnotationArray(simplex_dim);
		//
		TreeRootNodePtr root = ufd.MakeSet(simplex);
		annotations[simplex_dim]->Insert(simplex_ann, root, ufd);
		//  
	}
	return simplex;
}
template<typename T>
SimplicialTreeNode_ptr SimplicialTree<T>::ElementaryInsersion(std::vector<int> &simplex_vertices) {
	// Preq: its faces are inserted
	// ensure that the annotations in required dimensions are allocated
	SimplicialTreeNode_ptr simplex = find(simplex_vertices);
    
	if (!simplex) {
		// new simplex
		// 1) insert it into the simplicial tree
		simplex = insert_into_simplicial_tree(simplex_vertices);
		//
		int simplex_dim = (int)simplex_vertices.size() - 1;
		UpdateAnnotationArray(simplex_dim);
		//
		if (simplex_dim == 0) {
			// this is a vertex  
			// put it into the union find data struture;
			TreeRootNodePtr root = ufd.MakeSet(simplex); // connect simplex with tree_node
			ListNodePtr newCycleAnno;
			newCycleAnno = annotations[0]->create_cocycle(root, ufd); // connect tree_root_node with annotation
			//update persistences in dim simplex_dim
			int newTS = annotations[simplex_dim]->lowest_one(newCycleAnno);
			if (persistences.size() < simplex_dim + 1)
				persistences.resize(simplex_dim + 1);
			persistences[simplex_dim].insert(std::make_pair(newTS, std::make_pair(filtration_step, -1)));
			//   
			//check_status();
		}
		else {
			// 2) take the boundary of this simplex;
			vector<SimplicialTreeNode_ptr> boundaries;
			Boundary(simplex, boundaries);
			// 3) check the annotation sum of these boundary simplices
			ListNodePtr sum = annotations[simplex_dim - 1]->DeepCopyAnnotationColumn(find_annotation(boundaries.front()));
			int dead_bit = -1;
			for (int i = 1; i < boundaries.size(); ++i){
				ListNodePtr tempLNP = find_annotation(boundaries[i]);
				dead_bit = annotations[simplex_dim - 1]->sum_two_annotation_with_changed_dst(sum, tempLNP);
			}
			//empty annotation
			if (dead_bit == -1) {
				// 4.1) the sum of boundary annotations is zero
				//		create a new cocyle
				TreeRootNodePtr root = ufd.MakeSet(simplex); // connect simplex with tree_node
				ListNodePtr newCycleAnno;
				newCycleAnno = annotations[simplex_dim]->create_cocycle(root, ufd, annotations[simplex_dim]->empty()); // connect tree_root_node with annotation
				//update persistences in dim simplex_dim
				int newTS = annotations[simplex_dim]->lowest_one(newCycleAnno);
				if (persistences.size() < simplex_dim + 1)
					persistences.resize(simplex_dim + 1);
				persistences[simplex_dim].insert(std::make_pair(newTS, std::make_pair(filtration_step, -1)));
			}
			else {
				// 4.2) the sum of boundary annotations is not zero
				//		kill the cocycle represented by the last nonzero bit  
				annotations[simplex_dim - 1]->kill_cocycle_last_nonzero_bit(dead_bit, sum, ufd);
				//update persistences in dim simplex_dim - 1
				std::unordered_map<int, pair<int, int>>::iterator itPer = persistences[simplex_dim - 1].find(dead_bit);
				(itPer->second).second = filtration_step;
				int diff = filtration_step - (itPer->second).first;
				if (diff <= iThreshold && diff >= 0)
					persistences[simplex_dim - 1].erase(itPer);
				//
				// assign zero annotation to this simplex
				TreeRootNodePtr root = ufd.MakeSet(simplex);
				ListNodePtr zero_ann = annotations[simplex_dim]->make_zero_annotation();
				annotations[simplex_dim]->Insert(zero_ann, root, ufd);
			}
			//check_status();
			//break the cycle of "sum" annotation for deletion
			sum->next.reset();
		}
	}
	return simplex;
}
template <class T>
bool SimplicialTree<T>::Boundary(const SimplicialTreeNode_ptr& sigma, std::vector<SimplicialTreeNode_ptr> &bdries)
{
	// traverse back to the root to get all boundary faces
	SimplicialTreeNode_ptr pIter(sigma);
	SimplicialTreeNode_ptr pBdryFace;
	//
	unordered_map<int, SimplicialTreeNode_ptr> & vertices = (*labels_dict_in_each_dim.front());
	//
	// vertex simplex doesn't have boundary
	// the half vertex set
	std::set<int> tail_ver_index_set;
	if (pIter->parent)
	{// simplex of dim > 0
		do
		{// the boundary face without current vertex 
			// go from its parent to visit all tail vertex in tail_ver_index_set
			// at the end of visit, the boundary face pointer is reached.
			if (pIter->parent)
			{
				pBdryFace = pIter->parent;
			}
			else
			{// at the moment of removing the first vertex
				// require tail_ver_index_set NOT empty
				pBdryFace = vertices[*tail_ver_index_set.begin()];
				tail_ver_index_set.erase(tail_ver_index_set.begin());
			}
			//
			for (std::set<int>::iterator sIter = tail_ver_index_set.begin();
				sIter != tail_ver_index_set.end();
				sIter++)
			{//
				pBdryFace = (pBdryFace->children)[*sIter];
			}
			//
			bdries.push_back(pBdryFace);
            
			//
			tail_ver_index_set.insert(pIter->label);
			// traverse back one more step
			pIter = pIter->parent;
		} while (pIter);
	}
	/******************************************/
	return true;
}
template <class T>
void SimplicialTree<T>::insert_into_circular_list(const int simplex_dim, const SimplicialTreeNode_ptr & simplex)
{
	if (simplex_dim > 0) {
		unordered_map<int, SimplicialTreeNode_ptr> & dict = (*labels_dict_in_each_dim[simplex_dim]);
		if (dict.find(simplex->label) == dict.end()) {
			// it is first one
			dict[simplex->label] = simplex;
			simplex->next_circular_ptr = simplex;
		}
		else {
			// insert just behind the head element
			simplex->next_circular_ptr = dict[simplex->label]->next_circular_ptr;
			dict[simplex->label]->next_circular_ptr = simplex;
		}
	}
	// no need to handle the vertices
	return;
}
template <typename T>
void SimplicialTree<T>::retrieve_simplices_from_subtrees(const SimplicialTreeNode_ptr &sigma,
	vector<SimplicialTreeNode_ptr> &coBdries, bool dfs_visit) {
	//
	if (dfs_visit) {
		// do a bfs visit to get all cofaces
		boost::unordered_set<SimplicialTreeNode_ptr> visited;
		stack<SimplicialTreeNode_ptr> S;
		S.push(sigma);
		visited.insert(sigma);
		while (!S.empty()) {
			SimplicialTreeNode_ptr curr = S.top();
			bool to_be_visited = true;
			if (!curr->children.empty()) {
				for (map<int, SimplicialTreeNode_ptr>::iterator mIter = curr->children.begin();
					mIter != curr->children.end(); ++mIter) {
					if (visited.find(mIter->second) == visited.end()) {
						to_be_visited = false;
						S.push(mIter->second);
						visited.insert(mIter->second);
					}
				}
			}
			if (to_be_visited) {
				coBdries.push_back(curr);
				S.pop();
			}
		}
	}
	else {
		queue<SimplicialTreeNode_ptr> Q;
		Q.push(sigma);
		while (!Q.empty()) {
			SimplicialTreeNode_ptr curr = Q.front();
			Q.pop();
			coBdries.push_back(curr);
			for (map<int, SimplicialTreeNode_ptr>::iterator mIter = curr->children.begin();
				mIter != curr->children.end(); ++mIter) {
				Q.push(mIter->second);
			}
		}
	}
	return;
}
template<typename T>
bool SimplicialTree<T>::is_coface(SimplicialTreeNode_ptr &sigma, vector<int> &simplex_vertices) {
	// labels in simplex_vertices and sigma are sorted
	SimplicialTreeNode_ptr trav = sigma;
	int i = (int)simplex_vertices.size() - 1;
	while (trav && i >= 0) {
		if (trav->label == simplex_vertices[i]) {
			trav = trav->parent;
			--i;
		}
		else {
			if (trav->label < simplex_vertices[i]) {
				// the lagest remaining labels is smaller than the current traget
				// failed.
				return false;
			}
			trav = trav->parent;
		}
	}
	return (i < 0);
}
template <typename T>
void SimplicialTree<T>::CoDimensionOneFaces(const SimplicialTreeNode_ptr& sigma, std::vector<SimplicialTreeNode_ptr> &codim_one_faces) {
	vector<int> simplex_vertices;
	retrieve_vertex_indices(sigma, simplex_vertices);
	int simplex_dim = (int)simplex_vertices.size() - 1;
	//
	for (map<int, SimplicialTreeNode_ptr>::iterator mIter = sigma->children.begin();
		mIter != sigma->children.end(); ++mIter) {
		codim_one_faces.push_back(mIter->second);
	}
	//
	int label = simplex_vertices.back();
	unordered_map<int, SimplicialTreeNode_ptr> & labels_dict = (*labels_dict_in_each_dim[simplex_dim + 1]);
	if (labels_dict.find(label) != labels_dict.end()) {
		// visit all simplices in current dimension containing label
		SimplicialTreeNode_ptr trav = labels_dict[label];
		SimplicialTreeNode_ptr start = trav;
		do {
			// check it contains all labels in the input simplex or not
			if (is_coface(trav, simplex_vertices)) {
				codim_one_faces.push_back(trav);
			}
			trav = trav->next_circular_ptr;
		} while (trav != start);
	}
	return;
}
template <typename T>
bool SimplicialTree<T>::CoBoundary(const SimplicialTreeNode_ptr& sigma, std::vector<SimplicialTreeNode_ptr> &coBdries, bool dfs_visit){
	// visit subtress using bfs
	vector<int> simplex_vertices;
	retrieve_vertex_indices(sigma, simplex_vertices);
	int simplex_dim = (int)simplex_vertices.size() - 1;
	//
	for (map<int, SimplicialTreeNode_ptr>::iterator mIter = sigma->children.begin();
		mIter != sigma->children.end(); ++mIter) {
		retrieve_simplices_from_subtrees(mIter->second, coBdries, dfs_visit);
	}
	//
	int label = simplex_vertices.back();
	for (int i = simplex_dim + 1; i < labels_dict_in_each_dim.size(); ++i) {
		unordered_map<int, SimplicialTreeNode_ptr> & labels_dict = (*labels_dict_in_each_dim[i]);
		if (labels_dict.find(label) != labels_dict.end()) {
			// visit all simplices in current dimension containing label
			SimplicialTreeNode_ptr trav = labels_dict[label];
			SimplicialTreeNode_ptr start = trav;
			do {
				// check it contains all labels in the input simplex or not
				if (is_coface(trav, simplex_vertices)) {
					retrieve_simplices_from_subtrees(trav, coBdries, dfs_visit);
				}
				trav = trav->next_circular_ptr;
			} while (trav != start);
		}
	}
	return true;
}
template <typename T>
void SimplicialTree<T>::LinkSubcomplex(const SimplicialTreeNode_ptr &sigma, std::vector<SimplicialTreeNode_ptr> &simplexLink) {
	CoBoundary(sigma, simplexLink);
	vector<int> simplex_vertices;
	retrieve_vertex_indices(sigma, simplex_vertices);
	vector<int> link_simplex_vertices;
	for (int i = 0; i < simplexLink.size(); ++i) {
		SimplicialTreeNode_ptr trav = simplexLink[i];
		int index = (int)simplex_vertices.size() - 1;
		while (trav) {
			if (index >= 0 && trav->label == simplex_vertices[index]) {
				trav = trav->parent;
				--index;
			}
			else {
				if (index < 0 || trav->label > simplex_vertices[index]) {
					link_simplex_vertices.push_back(trav->label);
					trav = trav->parent;
				}
				else {
					cout << "Not a coface in getting link simplex" << endl;
					exit(0);
				}
			}
		}
		//
		reverse(link_simplex_vertices.begin(), link_simplex_vertices.end());
		simplexLink[i] = find(link_simplex_vertices);
		link_simplex_vertices.clear();
	}
	return;
}
template <typename T>
void SimplicialTree<T>::LinsubcomplexIntersection(SimplicialTreeNode_ptr & a, SimplicialTreeNode_ptr & b, boost::unordered_set<SimplicialTreeNode_ptr> &intersectedLinks) {
	// no simplices in the subtree of sigma are in the intersection
	// 
	vector<SimplicialTreeNode_ptr> a_link_subcomplex;
	vector<SimplicialTreeNode_ptr> b_link_subcomplex;
	LinkSubcomplex(a, a_link_subcomplex);
	LinkSubcomplex(b, b_link_subcomplex);
	vector<SimplicialTreeNode_ptr> * smaller = &a_link_subcomplex;
	vector<SimplicialTreeNode_ptr> * larger = &b_link_subcomplex;
	if (a_link_subcomplex.size() > b_link_subcomplex.size()) {
		smaller = &b_link_subcomplex;
		larger = &a_link_subcomplex;
	}
	for (int i = 0; i < smaller->size(); ++i) {
		intersectedLinks.insert((*smaller)[i]);
	}
	int left = 0, right = (int)larger->size();
	while (left < right) {
		if (intersectedLinks.find((*larger)[left]) != intersectedLinks.end()) {
			++left;
		}
		else {
			swap((*larger)[left], (*larger)[right - 1]);
			--right;
		}
	}
	intersectedLinks.clear();
	for (int i = 0; i < left; ++i) {
		intersectedLinks.insert((*larger)[i]);
	}
	// 
	return;
}
template<typename T>
bool SimplicialTree<T>::is_link_condition_satisfied(SimplicialTreeNode_ptr &edge, boost::unordered_set<SimplicialTreeNode_ptr> &intersectedLinks) {
	unordered_map<int, SimplicialTreeNode_ptr> & vertices = (*labels_dict_in_each_dim[0]);
	SimplicialTreeNode_ptr a = vertices[edge->label]; // vertex with larger label
	SimplicialTreeNode_ptr b = vertices[edge->parent->label]; // vertex with smaller label
	LinsubcomplexIntersection(a, b, intersectedLinks);
	vector<SimplicialTreeNode_ptr> edgeLinkSubcomplex;
	LinkSubcomplex(edge, edgeLinkSubcomplex);
	// by theory, edge link subcomplex is a subset of the intersection subcomplex
	for (int i = 0; i < edgeLinkSubcomplex.size(); ++i) {
		if (intersectedLinks.find(edgeLinkSubcomplex[i]) != intersectedLinks.end()) {
			intersectedLinks.erase(edgeLinkSubcomplex[i]);
		}
	}
	return intersectedLinks.empty();
}
template<typename T>
void SimplicialTree<T>::AddExtraSimplicesToSatisfyLinkCondition(SimplicialTreeNode_ptr &edge,
	boost::unordered_set<SimplicialTreeNode_ptr> &intersectedLinks){
	// since simplices in the intersection subcomplex are from the cofaces of the endpoints of the edge
	// only need to add simplicies [a b *]
	vector<int> edge_vertices(2, edge->parent->label);
	edge_vertices.back() = edge->label;
	//
	vector<int> link_simplex_vertices;
	vector<int> simplex_vertices;
	link_simplex_vertices.reserve(dim + 1);
	simplex_vertices.reserve(2 * dim + 2);

	vector<SimplicialTreeNode_ptr> baseSubcomplex(intersectedLinks.begin(), intersectedLinks.end());
	// no need to sort as simplices are ordered by dimension because of bfs visit
	if (baseSubcomplex.size() > 1) {
		sort(baseSubcomplex.begin(), baseSubcomplex.end(), SimplicialTreeNodePtrLessThan());
	}
	// use the fact that the intersection link subcomplexes of two vertices is a subcomplex as well
	for (int i = 0; i < baseSubcomplex.size(); ++i) {
		link_simplex_vertices.clear();
		simplex_vertices.clear();
		retrieve_vertex_indices(baseSubcomplex[i], link_simplex_vertices);
		merge_two_sorted_arrays(edge_vertices, link_simplex_vertices, simplex_vertices);
		//
		ElementaryInsersion(simplex_vertices);
	}
	return;
}
template<typename T>
void SimplicialTree<T>::AnnotationTransfer(SimplicialTreeNode_ptr &edge, const int preserve_vertex_label) {
	// remove the cofaces of the edge which contain both vertices
	//cout << "AnnotationTransfer" << endl;
	const int removed_vertex_label = (edge->label == preserve_vertex_label ? edge->parent->label : edge->label);
	vector<SimplicialTreeNode_ptr> edge_cofaces;
	vector<SimplicialTreeNode_ptr> codim_one_faces;
	CoBoundary(edge, edge_cofaces, true);
	edge_cofaces.push_back(edge);
	// cofaces are ordered with dimension larger first 
	vector<int> simplex_vertices;
	simplex_vertices.reserve(dim + 1);
	for (int i = 0; i < edge_cofaces.size(); ++i) {
		// every simplex in the coface is a vanishing simplex
		// if the vanishing simpelx has non-zero annotation
		ListNodePtr vanishing_simplex_annotation = find_annotation(edge_cofaces[i]);
		if (!is_zero_annotation(vanishing_simplex_annotation)) {
			int simplex_dim = SimplexDim(edge_cofaces[i]);
			//check_status();
			vanishing_simplex_annotation = annotations[simplex_dim]->DeepCopyAnnotationColumn(vanishing_simplex_annotation);
			simplex_vertices.clear();
			retrieve_vertex_indices(edge_cofaces[i], simplex_vertices);
			// find the mirror simplex
			for (int j = 0; j < simplex_vertices.size(); ++j) {
				if (simplex_vertices[j] > removed_vertex_label) {
					simplex_vertices[j - 1] = simplex_vertices[j];
				}
			}
			simplex_vertices.resize(simplex_vertices.size() - 1);
			//
			SimplicialTreeNode_ptr mirror_simplex = find(simplex_vertices);
			// obtain all codimension one faces of the mirror simplex
			codim_one_faces.clear();
			CoDimensionOneFaces(mirror_simplex, codim_one_faces);
			//
			// add the annotation of the vanishing simplex to all codimension one faces of the mirror simplex
			for (int j = 0; j < codim_one_faces.size(); ++j) {
				//
				//cout << j << "\t"; check_status(); 
				ListNodePtr codim_one_face_annotation = annotations[simplex_dim]->DeepCopyAnnotationColumn(find_annotation(codim_one_faces[j]));
				// delete the tree and the annotation it is pointed to
				if (ufd.Is_singleton(codim_one_faces[j]->tree_node)) {
					// after deletion also delete the annotation
					TreeRootNodePtr root = ufd.Find(codim_one_faces[j]->tree_node);
					//annotations[simplex_dim]->Delete(root->attribute);
					annotations[simplex_dim]->clearNode(root->attribute, false);
				}
				ufd.Delete(codim_one_faces[j]->tree_node);
				//sigma->tree_node.reset(); reset in ufd.Delete

				annotations[simplex_dim]->sum_two_annotation_with_changed_dst(codim_one_face_annotation, vanishing_simplex_annotation);
				//
				// insert the simplex with new annotation into the union-find-deletion forest and annotation matrix
				TreeRootNodePtr root = ufd.MakeSet(codim_one_faces[j]);
				annotations[simplex_dim]->Insert(codim_one_face_annotation, root, ufd);
				//
			}
		}
	}
	//cout << " out AnnotationTransfer" << endl;
	return;
}
template<typename T>
void SimplicialTree<T>::ElementaryCollapse(const int remove_label, const int preserve_label) {
	if (remove_label == preserve_label)
		return;
	vector<int> simplex_vertices(2, remove_label);
	remove_label < preserve_label ? simplex_vertices[1] = preserve_label : simplex_vertices[0] = preserve_label;
	SimplicialTreeNode_ptr edge_simplex = find(simplex_vertices);
	if (!edge_simplex) {
		// the edge is not exist
		edge_simplex = ElementaryInsersion(simplex_vertices);
	}
	// 
	boost::unordered_set<SimplicialTreeNode_ptr> intersectedLinkSubcomplex;
	if (!is_link_condition_satisfied(edge_simplex, intersectedLinkSubcomplex)) {
		// insert necessary simplices to ensure the link condition
		AddExtraSimplicesToSatisfyLinkCondition(edge_simplex, intersectedLinkSubcomplex);
	}
	// apply annotation transfer
	AnnotationTransfer(edge_simplex, preserve_label);
	// perform the edge contraction
	EdgeContraction(edge_simplex, preserve_label);
	//
	return;
}
template<typename T>
void SimplicialTree<T>::EdgeContraction(SimplicialTreeNode_ptr &edge, const int preserve_vertex_label) {
	unordered_map<int, SimplicialTreeNode_ptr> & vertices = (*labels_dict_in_each_dim.front());
	SimplicialTreeNode_ptr preserved_vertex = vertices[preserve_vertex_label];
	SimplicialTreeNode_ptr removed_vertex = vertices[edge->label == preserve_vertex_label ? edge->parent->label : edge->label];
	// remove the cofaces of the edge which contain both vertices
	vector<SimplicialTreeNode_ptr> cofaces;
	CoBoundary(edge, cofaces, true);
	// cofaces are ordered with dimension larger first
	cofaces.push_back(edge); // delete this edge as well
	for (int i = 0; i < cofaces.size(); ++i) {
		remove_simplex_from_both_complex_and_ufdForest(cofaces[i]);
		//PrintComplexWithAnnotation();
	}
	// handle the vertex first
	
	rename_simplices_in_subtree(removed_vertex, preserve_vertex_label);
	//PrintComplexWithAnnotation();
	// 
	int simplex_dim = 0;
	int label = removed_vertex->label;
	for (int i = simplex_dim + 1; i < labels_dict_in_each_dim.size(); ++i) {
		unordered_map<int, SimplicialTreeNode_ptr> & labels_dict = (*labels_dict_in_each_dim[i]);
		while (labels_dict.find(label) != labels_dict.end()) {
			// visit all simplices in current dimension containing the removed label 
			SimplicialTreeNode_ptr trav = labels_dict[label];
			rename_simplices_in_subtree(trav, preserve_vertex_label);
			//PrintComplexWithAnnotation();
		}
	}
	// update the union-find-deletion forest and annotation matrix
	return;
}
template<typename T>
void SimplicialTree<T>::rename_simplices_in_subtree(SimplicialTreeNode_ptr sigma, const int new_label){
	// record the deletion option for the simplices
	// 1) remove only from the simplicial tree -- false
	// 2) remove both from the simplicial tree and union-find-deletion forest --- true
	boost::unordered_map<SimplicialTreeNode_ptr, pair<SimplicialTreeNode_ptr, bool> > flags;
	stack<SimplicialTreeNode_ptr> S;
	vector<int> simplex_vertices;
	retrieve_vertex_indices(sigma, simplex_vertices);
	// sigma->label is replaced with new_label
	simplex_vertices.back() = new_label;
	sort(simplex_vertices.begin(), simplex_vertices.end());
	SimplicialTreeNode_ptr newNode = find(simplex_vertices);
	if (!newNode) {
		// this is a new simplex to be inserted
		//insert the the new one into the simplicial tree
		newNode = insert_into_simplicial_tree(simplex_vertices);
		newNode->tree_node = sigma->tree_node;
		sigma->tree_node->elem = newNode;
		sigma->tree_node.reset();
		// 
		flags[sigma] = pair<SimplicialTreeNode_ptr, bool>(newNode, false); // only delete from simplicial tree
	}
	else {
		flags[sigma] = pair<SimplicialTreeNode_ptr, bool>(newNode, true); // delete from both simplicial tree and union-find-deletion forest
	}

	S.push(sigma);
	while (!S.empty()) {
		SimplicialTreeNode_ptr curr = S.top();
		simplex_vertices.clear();
		retrieve_vertex_indices(flags[curr].first, simplex_vertices);
		//
		if (!curr->children.empty() && flags.find(curr->children.begin()->second) == flags.end()) {
			// all childrens are not visited 
			// check the subtrees
			simplex_vertices.push_back(0); // one dimension higher simplex
			vector<int> ordered_children;
			ordered_children.reserve(curr->children.size());
			for (map<int, SimplicialTreeNode_ptr>::iterator mIter = curr->children.begin();
				mIter != curr->children.end(); ++mIter) {
				ordered_children.push_back(mIter->first);
			}
			// 
			int read = 0;
			vector<int> backup_simplex = simplex_vertices;
			///////////
			while (read < ordered_children.size()) {
				SimplicialTreeNode_ptr child = curr->children[ordered_children[read]];
				// update the simplex vertices array
				simplex_vertices = backup_simplex;
				simplex_vertices.back() = child->label;
				if (simplex_vertices.back() < simplex_vertices[simplex_vertices.size() - 2]) {
					swap(simplex_vertices.back(), simplex_vertices[simplex_vertices.size() - 2]);
				}
				newNode = find(simplex_vertices);
				//
				if (!newNode) {
					if (ordered_children[read] > new_label) {
						// no need to generate a new node;
						// move this node and all children to the new position
						SimplicialTreeNode_ptr parent = flags[curr].first;
						parent->children[child->label] = child;
						// remove it from it parent
						child->parent->children.erase(child->label);
						// update its new parent
						child->parent = parent;
						//do not move write pointer
					}
					else {
						// copy the node in union-find-deletion forest and 
						newNode = insert_into_simplicial_tree(simplex_vertices);
						newNode->tree_node = child->tree_node;
						child->tree_node->elem = newNode;
						child->tree_node.reset();
						//
						S.push(child);
						flags[child] = pair<SimplicialTreeNode_ptr, bool>(newNode, false); // only delete from simplicial tree 
					}
				}
				else {
					S.push(child);
					flags[child] = pair<SimplicialTreeNode_ptr, bool>(newNode, true); // delete from both simplicial tree and union-find-deletion forest
				}
				++read;
			}
		}
		else {
			// no child or all children are visited before  
			// second visit
			// delete from the simplicial tree
			S.pop();
			// perform the deletion action
			if (flags[curr].second) {
				// remove it from both simplicial tree and union-find-deletion forest
				remove_simplex_from_both_complex_and_ufdForest(curr);
			}
			else {
				remove_simplex_from_complex(curr);
			}
		}
	}
	return;
}
template<typename T>
void SimplicialTree<T>::remove_simplex_from_complex(SimplicialTreeNode_ptr sigma){
	// Preq: all cofaces are deleted already
	// remove the tree node in union-find-deletion data structure
	if (!sigma->children.empty()) {
		cout << "can not be deleted as some cofaces exists" << endl;
		exit(0);
	}
	int simplex_dim = SimplexDim(sigma);
	//sigma->tree_node.reset(); reset in ufd.Delete
	// parent-children relation
	sigma->parent->children.erase(sigma->label);
	sigma->parent.reset();
	//delete from the circular list or the unordered map
	delete_from_circular_list(sigma, simplex_dim);
	// update size info
	--simplex_sizes[simplex_dim];
	if (simplex_sizes[simplex_dim] == 0) {
		simplex_sizes.resize(simplex_dim);
		--dim;
		labels_dict_in_each_dim.resize(simplex_dim);
		//keep the time stamp for annotation matrix of dim: simplex_dim
		this->vecTS[simplex_dim] = annotations[simplex_dim]->timeStamp;
		annotations.resize(simplex_dim);
	}
	return;
}

template<typename T>
void SimplicialTree<T>::remove_simplex_from_both_complex_and_ufdForest(SimplicialTreeNode_ptr sigma, bool bUpdatePers){
	// Preq: all cofaces are deleted already
	// remove the tree node in union-find-deletion data structure
	if (!sigma->children.empty()) {
		cout << "can not be deleted as some cofaces exists" << endl;
		exit(0);
	}
	int simplex_dim = SimplexDim(sigma);
	if (ufd.Is_singleton(sigma->tree_node)) {
		// after deletion also delete the annotation
		TreeRootNodePtr root = ufd.Find(sigma->tree_node);
		annotations[simplex_dim]->clearNode(root->attribute, bUpdatePers);
	}
	ufd.Delete(sigma->tree_node);
	//sigma->tree_node.reset(); reset in ufd.Delete
	// parent-children relation
	if (simplex_dim > 0) {
		// the parent of a vertex is null
		sigma->parent->children.erase(sigma->label);
		sigma->parent.reset();
	}
	//delete from the circular list or the unordered map
	delete_from_circular_list(sigma, simplex_dim);
	//break cycle
	//sigma->tree_node.reset();
	// update size info
	--simplex_sizes[simplex_dim];
	if (simplex_sizes[simplex_dim] == 0) {
		simplex_sizes.resize(simplex_dim);
		--dim;
		//labels_dict_in_each_dim[simplex_dim].reset();
		labels_dict_in_each_dim.resize(simplex_dim);
		//keep the time stamp for annotation matrix of dim: simplex_dim
		this->vecTS[simplex_dim] = annotations[simplex_dim]->timeStamp;
		annotations.resize(simplex_dim);
	}
	return;
}
/*----------------Persistence Related operations-----------------------------------*/
template <typename T>
void SimplicialTree<T>::SnapshotHomologicalFeatures(vector<unordered_set<int> > &hom_info) {
	for (int i = 0; i < annotations.size(); ++i) {
		unordered_set<int> cycles;
		for (unordered_map<int, ListNodePtr>::iterator mIter = annotations[i]->row_ptr.begin();
			mIter != annotations[i]->row_ptr.end(); ++mIter) {
			cycles.insert(mIter->first);
		}
		hom_info.push_back(cycles);
	}
	return;
}
template <typename T>
void SimplicialTree<T>::CheckPersistence(vector<unordered_set<int> > &homo_info) {
	for (int i = (int)annotations.size(); i < homo_info.size(); ++i) {
		// all homology cycles with dimension higher than the dimension of the complex are killed
		homo_info[i].clear();
	}
	for (int i = 0; i < annotations.size(); ++i) {
		if (i < homo_info.size()) {
			vector<int> erase_set;
			erase_set.reserve(homo_info[i].size() + 1);
			for (unordered_set<int>::iterator sIter = homo_info[i].begin(); sIter != homo_info[i].end(); ++sIter) {
				if (annotations[i]->row_ptr.find(*sIter) == annotations[i]->row_ptr.end()) {
					erase_set.push_back(*sIter);
				}
			}
			for (int j = 0; j < erase_set.size(); ++j) {
				homo_info[i].erase(erase_set[j]);
			}
		}
		else {
			break;
		}
	}
	return;
}
template <typename T>
void SimplicialTree<T>::PerformSimplicialCollapse(vector<pair<int, int> > &vertex_map, vector<int> &updated_vertex_map) {
	// record the non-singleton pre-images of the vertex_map
	//vector<int> empVec;
	vector<vector<int>> preimages(maxImageVertex+1);
	for (int i = 0; i < vertex_map.size(); ++i) {
		preimages[vertex_map[i].second].push_back(vertex_map[i].first);
	}
	for (int i = 0; i < preimages.size(); ++i) {
		//cout << mIter->first << endl;
		if (!preimages[i].empty())
		{
			if (preimages[i].size() > 1) {
				//sort(mIter->second.begin(), mIter->second.end());
				//for (int i = 0; i < mIter->second.size() - 1; ++i) {
				//	ElementaryCollapse((mIter->second)[i], mIter->second.back());
				//} 
				for (int j = preimages[i].size() - 1; j > 0; --j) {
					//check_status();
					ElementaryCollapse(preimages[i][j], preimages[i].front());
					//cout << "after " << endl;
					//check_status();
				}

			}
			updated_vertex_map[preimages[i].front()] = i;
		}
	}
	return;
}
template <typename T>
void SimplicialTree<T>::clearMemory()		//clear memory of annotation matrices, simplicial trees and udf trees
{
	//clear simplicial trees, ufd trees and annotation matrices
	for (int i = labels_dict_in_each_dim.size() - 1; i >= 0; --i) {

		unordered_map<int, SimplicialTreeNode_ptr> & labels_dict = *(labels_dict_in_each_dim[i]);
		//
		unordered_map<int, SimplicialTreeNode_ptr>::iterator mIter = labels_dict.begin();
		if (i == 0)
		{
			// handle vertices
			while (true)
			{
				if (labels_dict.size() == 1)
				{
					this->remove_simplex_from_both_complex_and_ufdForest(labels_dict.begin()->second, false);
					break;
				}
				this->remove_simplex_from_both_complex_and_ufdForest(labels_dict.begin()->second, false);
			}
		}
		else {
			while (true)
			{
				SimplicialTreeNode_ptr trav = labels_dict.begin()->second;
				SimplicialTreeNode_ptr next;
				if (labels_dict.size() == 1)
				{
					while (trav != trav->next_circular_ptr)
					{
						next = trav->next_circular_ptr;
						this->remove_simplex_from_both_complex_and_ufdForest(trav, false);
						trav = next;
					}
					this->remove_simplex_from_both_complex_and_ufdForest(trav, false);
					break;
				}
				while (trav != trav->next_circular_ptr)
				{
					next = trav->next_circular_ptr;
					this->remove_simplex_from_both_complex_and_ufdForest(trav, false);
					trav = next;
				}
				this->remove_simplex_from_both_complex_and_ufdForest(trav, false);
			}
		}
	}
}

//relabeling vertices
template <typename T>
void SimplicialTree<T>::RelabelingVertices(unordered_map<int, int> &vertex_map)
{
	std::vector<LabelsDictionaryPtr> new_labels_dict_in_each_dim;
	for (int i = 0; i < this->labels_dict_in_each_dim.size(); ++i)
	{
		unordered_map<int, SimplicialTreeNode_ptr> & labels_dict = *(labels_dict_in_each_dim[i]);
		//add a new dimension in new_labels_dict_in_each_dim
		new_labels_dict_in_each_dim.push_back(boost::make_shared<unordered_map<int, SimplicialTreeNode_ptr> >());
		//relabeling each simplical tree node
		//also relabel its children map
		unordered_map<int, SimplicialTreeNode_ptr>::iterator mIter = labels_dict.begin();
		for (; mIter != labels_dict.end(); ++mIter)
		{
			if (i == 0)
			{
				//handle vertices
				//update current tree node label 
				mIter->second->label = vertex_map[mIter->first];
				//update children map
				if (mIter->second->children.size() != 0)
				{
					std::map<int, SimplicialTreeNode_ptr> newChildren;
					std::map<int, SimplicialTreeNode_ptr>::iterator mIterChildren = mIter->second->children.begin();
					for (; mIterChildren != mIter->second->children.end(); ++mIterChildren)
					{
						newChildren[vertex_map[mIterChildren->first]] = mIterChildren->second;
					}
					mIter->second->children = newChildren;
				}
			}
			else
			{
				SimplicialTreeNode_ptr trav = mIter->second;
				do
				{
					//update current tree node label
					trav->label = vertex_map[mIter->first];
					if (trav->children.size() != 0)
					{
						//update children map
						std::map<int, SimplicialTreeNode_ptr> newChildren;
						std::map<int, SimplicialTreeNode_ptr>::iterator mIterChildren = trav->children.begin();
						for (; mIterChildren != trav->children.end(); ++mIterChildren)
						{
							newChildren[vertex_map[mIterChildren->first]] = mIterChildren->second;
						}
						trav->children = newChildren;
					}
					//next tree node
					trav = trav->next_circular_ptr;
				} while (trav != mIter->second);
			}
			//copy new labels to new_labels_dict in each dimension
			//add a new dimension in new_labels_dict_in_each_dim
			unordered_map<int, SimplicialTreeNode_ptr> & new_labels_dict = *(new_labels_dict_in_each_dim[i]);
			new_labels_dict[vertex_map[mIter->first]] = labels_dict[mIter->first];
		}
	}
	//relabeling labels_dict_in_each_dim by copying new_labels_dict_in_each_dim back
	//labels_dict_in_each_dim.clear();
	labels_dict_in_each_dim = new_labels_dict_in_each_dim;
	return;
}


template <typename T>
void SimplicialTree<T>::InitializeByRenamingIncomingComplex(SimplicialTree<T> &src, vector<int> &vertex_map){
	//clear before use
	clearData();

	vector<int> simplex_vertices;
	simplex_vertices.reserve(dim + 1);
	// visit each simplex through the labels_diction variables
	for (int i = 0; i < src.labels_dict_in_each_dim.size(); ++i) {

		unordered_map<int, SimplicialTreeNode_ptr> & labels_dict = *(src.labels_dict_in_each_dim[i]);
		//
		unordered_map<int, SimplicialTreeNode_ptr>::iterator mIter = labels_dict.begin();
		if (i == 0) {
			// handle vertices
			simplex_vertices.resize(1);
			for (; mIter != labels_dict.end(); ++mIter) {
				ListNodePtr simplex_ann = src.annotations[i]->DeepCopyAnnotationColumn(src.find_annotation(mIter->second));
				simplex_vertices.front() = vertex_map[mIter->first];
				//
				InsertSimplexWithAnnotation(simplex_vertices, simplex_ann);
			}
		}
		else {
			for (; mIter != labels_dict.end(); ++mIter) {
				SimplicialTreeNode_ptr trav = mIter->second;
				do{
					simplex_vertices.clear();
					src.retrieve_vertex_indices(trav, simplex_vertices);
					for (int j = 0; j < simplex_vertices.size(); ++j) {
						simplex_vertices[j] = vertex_map[simplex_vertices[j]];
					}
					sort(simplex_vertices.begin(), simplex_vertices.end());
					// 
					ListNodePtr simplex_ann = src.annotations[i]->DeepCopyAnnotationColumn(src.find_annotation(trav));
					//  
					InsertSimplexWithAnnotation(simplex_vertices, simplex_ann);
					//
					trav = trav->next_circular_ptr;
				} while (trav != mIter->second);
			}
		}
	}
	//copy annotation time stamps
	for (int i = 0; i < src.annotations.size(); ++i)
	{
		this->annotations[i]->timeStamp = src.annotations[i]->timeStamp;
	}
	this->vecTS = src.vecTS;
	return;
}
template <typename T>
void SimplicialTree<T>::AddRemainingSimpliciesFromFile(const char* pFileName) {
	/*
	# each line is a simplex with sorted integer vertex labels
	# ver_i < ver_j if i < j
	# ver_0 ver_1 ... ver_dim
	The following is a triangle (triangle.txt)
	0
	1
	2
	0 1
	1 2
	2 3
	1 2 3
	*/
	ReadComplex(pFileName);
	return;
}
/*----------------simplicial complex I/O-----------------------------------*/
template <typename T>
void SimplicialTree<T>::ReadSimplicialMap(const char* pFileName, vector<pair<int, int> > &vertex_map) {
	/*
	# Each line is a pair of vertices
	# Format: v_i v_j
	# Remark: v_i in the domain complex is mapped to v_j in the range complex
	The following is the map for edge contraction <1, 2> to 2
	1 2
	2 2
	*/
	this->maxImageVertex = -1;
	std::ifstream ifile;
	ifile.open(pFileName, std::ifstream::in | std::ifstream::binary);
	//
	//std::string sBuf;
	//std::stringstream sstr(std::stringstream::in | std::stringstream::out);
	//
	if (ifile.is_open()) {
		//      /*
		//       * Get the size of the file
		//       */ 
		//ifile.seekg(0, ifile.end);
		//      long long iFileSize = ifile.tellg();
		//ifile.seekg(0, ifile.beg);

		////copy the whole file into file buffer
		//char* fBuf = new char[iFileSize + 1];
		//ifile.read(fBuf, iFileSize);
		//// add extra symbol
		//fBuf[iFileSize] = '\n';
		////
		//sBuf.assign(fBuf);
		//cout << sBuf << endl;
		//sstr.str(sBuf);

		//// close file
		////ifile.close();
		//ifile.seekg(0,std::ios::beg);
		//sBuf.clear();
		////deallocate memory
		//delete [] fBuf;
		///*start reading*/
		///*read simplex from each line*/
		string line;
		istringstream iss;
		//first line for current time stamp
		getline(ifile, line);
		iss.str(line);
		string delim;
		float dFilScale;
		iss >> delim >> dFilScale;
		vecFiltrationScale.push_back(dFilScale);
		//following lines of vertex maps
		while (getline(ifile, line)) {
			iss.str(line);
			int src = 0, dst = 0;
			if (iss >> src >> dst) {
				vertex_map.push_back(pair<int, int>(src, dst));
				//update max image vertex
				if (dst > maxImageVertex)
					maxImageVertex = dst;
			}
			line.clear();
			iss.clear();
			iss.str("");
		}
		ifile.close();
	}
	else {
		std::cout << "Can NOT open file: " << pFileName << std::endl;
		exit(0);
	}
}
template <typename T>
void SimplicialTree<T>::ReadComplexWithAnnotation(const char* pFileName) {
	/*
	# The first line contains an integer which is the total number of simplices in this simplicial complex
	# Each later line is a simplex with increasingly sorted integer vertex labels and its annotation []
	# format: ver_0 ver_1 ... ver_dim [<a_1 val_1> <a_2 val_2> ... <a_n val_n>]
	#			ver_i < ver_j if i < j,
	#			<a_i val_i> : means the a_i-th bit has non-zero value val_i,
	#			[] means zero annotation
	# Remark: annotation is assumed to be under Z_2
	The following is an empty triangle (triangle.txt)
	7           --> number of simplices
	0 [<0 1>]
	1 [<0 1>]
	2 [<0 1>]
	0 1 []
	1 2 []
	0 2 [<0 1>]
	*/
	std::ifstream ifile;
	ifile.open(pFileName, std::ifstream::in | std::ifstream::binary);
	//
	std::string sBuf;
	std::stringstream sstr(std::stringstream::in | std::stringstream::out);
	//
	if (ifile.is_open()) {
		/*
		 * Get the size of the file
		 */
		ifile.seekg(0, std::ios::end);
		long long iFileSize = ifile.tellg();
		ifile.seekg(0, std::ios::beg);

		//copy the whole file into file buffer
		char* fBuf = new char[iFileSize + 1];
		ifile.read(fBuf, iFileSize);
		// add extra symbol
		fBuf[iFileSize] = '\n';
		//
		sBuf.assign(fBuf);
		sstr.str(sBuf);

		// close file
		ifile.close();
		sBuf.clear();
		//deallocate memory
		delete[] fBuf;
		/*start reading*/
		/*read simplex from each line*/
		string line;
		istringstream iss;
		vector<int> simplex_vertices;
		simplex_vertices.reserve(100);
		if (getline(sstr, line)) {
			iss.str(line);
			int simplex_count = 0;
			iss >> simplex_count;
			iss.clear();
			iss.str("");
			while (getline(sstr, line)) {
				simplex_vertices.clear();
				iss.str(line);
				int vertex = 0;
				while (iss >> vertex) {
					simplex_vertices.push_back(vertex);
				}
				if (simplex_count > 0 && !simplex_vertices.empty()) {
					--simplex_count;
					ListNodePtr simplex_ann = string_to_annotation(line);
					InsertSimplexWithAnnotation(simplex_vertices, simplex_ann);
					//initialize time stamp for each annotation matrix
					int simplexDimension = simplex_vertices.size() - 1;
					long long lastBit = annotations[simplexDimension]->lowest_one(simplex_ann);
					if (lastBit >= annotations[simplexDimension]->timeStamp)
						annotations[simplexDimension]->timeStamp = lastBit + 1;
				}
				line.clear();
				iss.clear();
				iss.str("");
			}
		}
	}
	else {
		std::cout << "Can NOT open file: " << pFileName << std::endl;
		exit(0);
	}
	return;
}
template <typename T>
void SimplicialTree<T>::ReadComplex(const char* pFileName) {
	/*
	# The first line contains an integer which is the total number of simplices in this simplicial complex
	# Each later line is a simplex with increasingly sorted integer vertex labels
	# Format: ver_0 ver_1 ... ver_dim
	# Remark:	1) ver_i < ver_j if i < j
	#			2) the simplices in the file are ordered such that the boundary faces of any simplex appear before itself
	#			3) the simple order satisfying this requirement is to sort the simplices by the increasingly order of their dimensions

	The following is a triangle (triangle.txt)
	7		---> number of simplices
	0		---> vertex 0
	1		---> vertex 1
	2		---> vertex 2
	0 1		---> edge <0, 1>
	1 2		---> edge <1, 2>
	0 2		---> edge <0, 2>
	0 1 2	---> triangle <0, 1, 2>
	*/
	std::ifstream ifile;
	ifile.open(pFileName, std::ifstream::in | std::ifstream::binary);
	//
	std::string sBuf;
	std::stringstream sstr(std::stringstream::in | std::stringstream::out);
	//
	if (ifile.is_open()) {
		/*
		 * Get the size of the file
		 */
		ifile.seekg(0, std::ios::end);
		long long iFileSize = ifile.tellg();
		ifile.seekg(0, std::ios::beg);

		//copy the whole file into file buffer
		char* fBuf = new char[iFileSize + 1];
		ifile.read(fBuf, iFileSize);
		// add extra symbol
		fBuf[iFileSize] = '\n';
		//
		sBuf.assign(fBuf);
		sstr.str(sBuf);

		// close file
		ifile.close();
		sBuf.clear();
		//deallocate memory
		delete[] fBuf;
		/*start reading*/
		/*read simplex from each line*/
		string line;
		istringstream iss;
		vector<int> simplex_vertices;
		simplex_vertices.reserve(100);
		if (getline(sstr, line)) {
			iss.str(line);
			int simplex_count = 0;
			iss >> simplex_count;
			iss.clear();
			iss.str("");
			while (getline(sstr, line)) {
				simplex_vertices.clear();
				iss.str(line);
				int vertex = 0;
				while (iss >> vertex) {
					simplex_vertices.push_back(vertex);
				}
				if (simplex_count > 0 && !simplex_vertices.empty()) {
					--simplex_count;
					ElementaryInsersion(simplex_vertices);
				}
				line.clear();
				iss.clear();
				iss.str("");
			}
		}
	}
	else {
		std::cout << "Can NOT open file: " << pFileName << std::endl;
		exit(0);
	}
}

template <class T>
void SimplicialTree<T>::WriteStatisticsToFile(const char* pFileName)
{
	std::cout << "Writing statistics < " << pFileName << " >" << std::endl;
	std::ofstream ofile;
	ofile.open(pFileName, std::ifstream::out);
	//
	std::stringstream sstr(std::stringstream::in | std::stringstream::out);
	if (ofile.is_open())
	{
		//sstr << pFileName << std::endl;

		for (int i = 0; i < dim + 1; i++)
		{
			sstr << "DIM " << i << " : " << simplex_sizes[i] << std::endl;
		}
		sstr << "Total size : " << ComplexSize() << std::endl;
		//
		//ofile << sstr.rdbuf();
		ofile.write(sstr.str().c_str(), sstr.str().size());
		//
		ofile.close();
		sstr.clear();
	}
	else
	{
		std::cout << "Can NOT open file " << pFileName << std::endl;
		exit(0);
	}
	std::cout << "---- Done ----" << std::endl << std::endl;
	return;
}
template<typename T>
void SimplicialTree<T>::PrintComplexWithAnnotation() {
	vector<int> simplex_vertices;
	simplex_vertices.reserve(dim + 1);
	// visit each simplex through the labels_diction variables
	for (int i = 0; i < labels_dict_in_each_dim.size(); ++i) {

		unordered_map<int, SimplicialTreeNode_ptr> & labels_dict = *(labels_dict_in_each_dim[i]);
		// assigning the bits with new indices
		unordered_map<int, int> newIndices;
		vector<int> nonzero_bits;
		nonzero_bits.reserve(annotations[i]->row_ptr.size());
		for (unordered_map<int, ListNodePtr>::iterator bitIter = annotations[i]->row_ptr.begin();
			bitIter != annotations[i]->row_ptr.end(); ++bitIter) {
			nonzero_bits.push_back(bitIter->first);
		}
		sort(nonzero_bits.begin(), nonzero_bits.end());
		for (int j = 0; j < nonzero_bits.size(); ++j) {
			newIndices[nonzero_bits[j]] = j;
		}
		//
		unordered_map<int, SimplicialTreeNode_ptr>::iterator mIter = labels_dict.begin();
		if (i == 0) {
			// handle vertices
			for (; mIter != labels_dict.end(); ++mIter) {
				cout << mIter->first << " " << annotation_to_string(mIter->second, newIndices) << endl;
			}
		}
		else {
			for (; mIter != labels_dict.end(); ++mIter) {
				SimplicialTreeNode_ptr trav = mIter->second;
				do{
					simplex_vertices.clear();
					retrieve_vertex_indices(trav, simplex_vertices);
					for (int j = 0; j < simplex_vertices.size(); ++j) {
						cout << simplex_vertices[j] << (j == simplex_vertices.size() - 1 ? "" : " ");
					}
					cout << " " << annotation_to_string(trav, newIndices) << endl;
					//
					trav = trav->next_circular_ptr;
				} while (trav != mIter->second);
			}
		}
	}
}
template<typename T>
void SimplicialTree<T>::WriteComplexWithAnnotation(const char* pFileName) {
	/*
	# The first line contains an integer which is the total number of simplices in this simplicial complex
	# Each later line is a simplex with increasingly sorted integer vertex labels and its annotation []
	# format:
	# ver_0 ver_1 ... ver_dim [<a_1 val_1> <a_2 val_2> ... <a_n val_n>]
	# Remark 1:	1) ver_i < ver_j if i < j,
	#			2) <a_i val_i> : means the a_i-th bit has non-zero value val_i,
	#			3) [] means zero annotation
	#			4) the simplices in the file are ordered such that the boundary faces of any simplex appear before itself
	#			5) the simple order satisfying this requirement is to sort the simplices by the increasingly order of their dimensions

	# Remark 2: annotation is assumed to be under Z_2
	The following is an empty triangle (triangle.txt)
	6		--> number of simplices
	0 []
	1 []
	2 []
	0 1 []
	1 2 []
	0 2 [<0 1>]
	*/
	std::ofstream ofile;
	ofile.open(pFileName, std::ifstream::out);
	//
	std::stringstream sstr(std::stringstream::in | std::stringstream::out);
	if (ofile.is_open())
	{
		vector<int> simplex_vertices;
		simplex_vertices.reserve(dim + 1);
		// visit each simplex through the labels_diction variables
		sstr << ComplexSize() << endl;
		for (int i = 0; i < labels_dict_in_each_dim.size(); ++i) {

			unordered_map<int, SimplicialTreeNode_ptr> & labels_dict = *(labels_dict_in_each_dim[i]);
			// assigning the bits with new indices
			//unordered_map<int, int> newIndices;
			//vector<int> nonzero_bits;
			//nonzero_bits.reserve(annotations[i]->row_ptr.size());
			//for (unordered_map<int, ListNodePtr>::iterator bitIter = annotations[i]->row_ptr.begin();
			//	bitIter != annotations[i]->row_ptr.end(); ++bitIter) {
			//	nonzero_bits.push_back(bitIter->first);
			//}
			//sort(nonzero_bits.begin(), nonzero_bits.end());
			//for (int j = 0; j < nonzero_bits.size(); ++j) {
			//	newIndices[nonzero_bits[j]] = j;
			//}
			//
			unordered_map<int, SimplicialTreeNode_ptr>::iterator mIter = labels_dict.begin();
			if (i == 0) {
				// handle vertices
				for (; mIter != labels_dict.end(); ++mIter) {
					sstr << mIter->first << " " << annotation_to_string(mIter->second) << endl;
				}
			}
			else {
				for (; mIter != labels_dict.end(); ++mIter) {
					SimplicialTreeNode_ptr trav = mIter->second;
					do{
						simplex_vertices.clear();
						retrieve_vertex_indices(trav, simplex_vertices);
						for (int j = 0; j < simplex_vertices.size(); ++j) {
							sstr << simplex_vertices[j] << (j == simplex_vertices.size() - 1 ? "" : " ");
						}
						sstr << " " << annotation_to_string(trav) << endl;
						//
						trav = trav->next_circular_ptr;
					} while (trav != mIter->second);
				}
			}
		}
		//
		//ofile << sstr.rdbuf();
		ofile.write(sstr.str().c_str(), sstr.str().size());
		//
		ofile.close();
		sstr.clear();
	}
	else
	{
		std::cout << "Can NOT open file " << pFileName << std::endl;
		exit(0);
	}
}
template<typename T>
void SimplicialTree<T>::WriteComplex(const char* pFileName) {
	/*
	# The first line contains an integer which is the total number of simplices in this simplicial complex
	# Each later line is a simplex with increasingly sorted integer vertex labels
	# Format: ver_0 ver_1 ... ver_dim
	# Remark: ver_i < ver_j if i < j
	The following is a triangle (triangle.txt)
	7			-->number of simplices
	0
	1
	2
	0 1
	1 2
	0 2
	0 1 2
	*/
	std::ofstream ofile;
	ofile.open(pFileName, std::ifstream::out);
	//
	std::stringstream sstr(std::stringstream::in | std::stringstream::out);
	if (ofile.is_open())
	{
		vector<int> simplex_vertices;
		simplex_vertices.reserve(dim + 1);
		// visit each simplex through the labels_diction variables
		sstr << ComplexSize() << endl;
		for (int i = 0; i < labels_dict_in_each_dim.size(); ++i) {

			unordered_map<int, SimplicialTreeNode_ptr> & labels_dict = *(labels_dict_in_each_dim[i]);
			unordered_map<int, SimplicialTreeNode_ptr>::iterator mIter = labels_dict.begin();
			if (i == 0) {
				// handle vertices
				for (; mIter != labels_dict.end(); ++mIter) {
					sstr << mIter->first << endl;
				}
			}
			else {
				for (; mIter != labels_dict.end(); ++mIter) {
					SimplicialTreeNode_ptr trav = mIter->second;
					do{
						simplex_vertices.clear();
						retrieve_vertex_indices(trav, simplex_vertices);
						for (int j = 0; j < simplex_vertices.size(); ++j) {
							sstr << simplex_vertices[j] << (j == simplex_vertices.size() - 1 ? "" : " ");
						}
						sstr << endl;
						//
						trav = trav->next_circular_ptr;
					} while (trav != mIter->second);
				}
			}
		}
		//
		//ofile << sstr.rdbuf();
		ofile.write(sstr.str().c_str(), sstr.str().size());
		//
		ofile.close();
		sstr.clear();
	}
	else
	{
		std::cout << "Can NOT open file " << pFileName << std::endl;
		exit(0);
	}
}
#endif // _SIMPLICIAL_TREE_H_
