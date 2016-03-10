/*
(c) 2013 Fengtao Fan
*/
#include "SimplicialComplexSP.h"
#include <fstream>
#include <sstream>
#include <string>

using namespace std;
 
void WritePersistence(const char* pFileName, vector<unordered_set<int> > &homo_info) {
	std::ofstream ofile;
	ofile.open(pFileName, std::ifstream::out);
  	//
 	std::stringstream sstr(std::stringstream::in | std::stringstream::out);
	if (ofile.is_open())
	{
		sstr << "Ranks of the persistent image of input simplicial map in all dimensions" << endl;
		for (int i = 0; i < homo_info.size(); ++i) {
			if (!homo_info[i].empty()) {
				sstr << "Dim " << i << ": " << homo_info[i].size() << " <";
				for (unordered_set<int>::iterator sIter = homo_info[i].begin(); sIter != homo_info[i].end(); ++sIter) {
					sstr << (sIter != homo_info[i].begin() ? ", " : "") << *sIter;
				}
				sstr << ">" << endl;
			}
			else {
				sstr << "Dim " << i << ": " << homo_info[i].size()  << endl;
			}
		}  
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
void ComputingPersistenceForSimplicialMap(const char* file_name_of_domain_complex, bool is_domain_complex_with_annotation,
										  const char* file_name_of_range_complex,
										  const char* file_name_of_simplicial_map,
										  const char* persistence_file_name, 
										  bool is_save_range_complex_with_annotation = false,
										  const char* new_range_complex_file_name = NULL ) {
	SimplicialTree<bool> domain_complex;
	cout << "Read simplicial complexes " << endl;
	if (is_domain_complex_with_annotation) {
		domain_complex.ReadComplexWithAnnotation(file_name_of_domain_complex);
	}
	else {
		domain_complex.ReadComplex(file_name_of_domain_complex);
	}
	//domain_complex.check_status();
	// record the homological cycles for the domain complex
	vector<unordered_set<int> > homo_info;
	domain_complex.SnapshotHomologicalFeatures(homo_info);
	// 
	vector<pair<int, int> > vertex_map;
	unordered_map<int, int> updated_vertex_map;
	domain_complex.ReadSimplicialMap(file_name_of_simplicial_map, vertex_map);
	// apply elementary collapses
	cout << "Evaluate the simplicial map " << endl;
	domain_complex.PerformSimplicialCollapse(vertex_map, updated_vertex_map);
	// 
	//domain_complex.check_status(); 
	// remove killed cycles from domain complex
	domain_complex.CheckPersistence(homo_info);
	//
	SimplicialTree<bool> range_complex; 
	// add the subcomplex of the range_complex which is the preimage of the simplicial map first
	range_complex.InitializeByRenamingIncomingComplex(domain_complex, updated_vertex_map);
	//range_complex.check_status();
	// add the rest of the range_complex
	range_complex.AddRemainingSimpliciesFromFile(file_name_of_range_complex);
	 
	// remove killed cycles from domain complex
	// assigning the bits in annotation with new ordered indices starting from 0
	//vector<unordered_map<int, int>> newIndices(range_complex.annotations.size());
	//for (int i = 0; i < range_complex.annotations.size(); ++i) {
	//	vector<int> nonzero_bits;
	//	nonzero_bits.reserve(range_complex.annotations[i]->row_ptr.size());
	//	for (unordered_map<int, ListNodePtr>::iterator bitIter = range_complex.annotations[i]->row_ptr.begin();
	//		bitIter != range_complex.annotations[i]->row_ptr.end(); ++bitIter) {
	//		nonzero_bits.push_back(bitIter->first);
	//	}
	//	sort(nonzero_bits.begin(), nonzero_bits.end());
	//	for (int j = 0; j < nonzero_bits.size(); ++j) {
	//		newIndices[i][nonzero_bits[j]] = j;
	//	}
	//}
	//
	range_complex.CheckPersistence(homo_info);
	//
	if (is_save_range_complex_with_annotation) {
		cout << "Save range complex with annotations in: " << new_range_complex_file_name << endl;
		range_complex.WriteComplexWithAnnotation(new_range_complex_file_name);
		/*
		string abfile(new_range_complex_file_name);
		abfile += "_two.txt";
		range_complex.WriteComplex(abfile.c_str());
		*/
	}
	//range_complex.WriteStatisticsToFile("size_info.txt");
	//
	cout << "Save results in: " << persistence_file_name << endl;
	WritePersistence(persistence_file_name, homo_info);
	//
	/*
	homo_info.clear();
	range_complex.SnapshotHomologicalFeatures(homo_info);
	string abfile(persistence_file_name);
	abfile += "_two.txt";
	WritePersistence(abfile.c_str(), homo_info); 
	*/
	return;
}
 