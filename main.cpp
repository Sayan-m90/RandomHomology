//
//  main.cpp
//  BitTests
//
//  Created by Bill Varcho on 2/24/16.
//  Copyright © 2016 Bill Varcho. All rights reserved.
//

#include "Collapse.hpp"
#include "Constants.hpp"
#include "GIC.hpp"
#include "Wrappers/GICWrapper.hpp"
#include "Graphs/KNN_Graph.hpp"
#include "Graphs/NormalGraph.hpp"
#include "MortonCode.hpp"
#include "RandomHomology.hpp"
#include "Wrappers/SimPersWrapper.hpp"
#include "Utilities.hpp"
#include <time.h>
#include <iostream>
#include <vector>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem.hpp>
#include <boost/thread.hpp>

using std::cout;
using std::endl;
using std::priority_queue;
using std::vector;



int main(int argc,  char * argv[]) {

    //    boost::thread t(tempFunc, "Hello World", 21);
    //    cout << "Waiting for thread" << endl;
    //    t.join();
    //    cout << "Thread finished" << endl;
    
    
    // eventually need to get as command line arg
    
    string input_points_file;       // = "/Users/billvarcho/Documents/models/kitten.txt";
    cout<<"Enter file Name\n";
    cin>>input_points_file;
    ifstream f(input_points_file.c_str());
    if( f.good()==false)
    {
        cout<<"File does not exist.";
        return 0;
    }
    vector<string> strs;
    boost::split(strs, input_points_file, boost::is_any_of("."));
    string dir_file = strs[0];

    boost::filesystem::path dir(dir_file);
    if (!boost::filesystem::exists(dir_file)) {
        // create directory
        if(! boost::filesystem::create_directory(dir)) {
            cout << "Could not create output directory: FAILED" << endl;
            return 0;
        }
    }
    
    // split
    vector<string> dir_strings;
    boost::split(dir_strings, dir_file, boost::is_any_of("/"));
    string points_name = dir_strings[dir_strings.size() - 1];

    string complex_file = dir_file + "/" + points_name + "_complex.txt";
    string output_collapses = dir_file + "/" + points_name;
    cout<<"\nEnter Output Dimension of the data\n";

    int max_dim,neighbors;
    float subsample;
    cin>>max_dim;
    cout<<"\nEnter no of neighbors\n";
    cin>>neighbors;
    
    do{
    cout<<"\nEnter subsampling ratio (2-inf)\n";
    cin>>subsample;

    }while(subsample<2);
    
    const clock_t begin_time = clock();
    GICWrapper::Run(input_points_file, complex_file, max_dim, neighbors);

    f.close();
    GIC g(complex_file);
    fflush(stdin);
    cout<<"Out of GIC\n";
    //getchar();
    vector<Operation*> collapses;
    RandomHomology r(g);
    r.run(subsample, collapses);
    cout << "\n Collapse with GIC Total Timing without I/O:" <<float( clock () - begin_time ) /  CLOCKS_PER_SEC<<endl;
    Utilities::WriteCollapsesToFile(output_collapses, collapses);
    cout << "\n Collapse with GIC Total Timing :" <<float( clock () - begin_time ) /  CLOCKS_PER_SEC<<endl;
    // glutInit( & argc, argv );
    
    //Viewer v1, v2, v3;
     //v1.DrawGIC(g);
     //v2.ViewCollapses(g, collapses);
    //return 1;


    
    SimpersWrapper::Run( collapses, output_collapses);
    cout << "\n\n Total Running Time:" <<float( clock () - begin_time ) /  CLOCKS_PER_SEC<<endl;
    //secondary();
    //vector<vector<Barcode*>> barcodes;
    //string bc_fp = output_collapses + "_barcode";\
    //Utilities::ReadInBarcode(bc_fp, barcodes);
    //v3.ViewBarcode(barcodes);
    return 1;
}
