//
//  main.cpp
//  BitTests
//
//  Created by Bill Varcho on 2/24/16.
//  Copyright © 2016 Bill Varcho. All rights reserved.
//

#include <iostream>
#include <vector>
#include "GIC.hpp"
#include "RandomHomology.hpp"
#include "MortonCode.hpp"
#include "Utilities.hpp"
#include "Collapse.hpp"
#include "Constants.hpp"

#include "KNN_Graph.hpp"
#include "SimPersWrapper.hpp"

using std::vector;
using std::priority_queue;
using std::cout;
using std::endl;

int main(int argc, const char * argv[]) {

//    //GIC g("/Users/billvarcho/Desktop/Data/Torus/simple_complex.txt");
//    //GIC g("/Users/billvarcho/Desktop/Data/out_bunny_complex.txt");
//    //GIC g("/Users/billvarcho/Desktop/Data/BunnyDense/tmp_complex.txt");
//    //GIC g("/Users/billvarcho/Desktop/DELETE_SAYAN/gICe10d20-4_complex.txt");

//    string input_complex = "/Users/billvarcho/Desktop/OLD/Data/DoubleTorus/out_complex.txt";
//    string output_collapses = "/Users/billvarcho/Desktop/Tests/bunny";


    string input_complex = "/Users/billvarcho/Desktop/Tests/FundamentalObjects/ThreeTorus_complex.txt";
    string output_collapses = "/Users/billvarcho/Desktop/Tests/FundamentalObjects/ThreeTorus";
    
    GIC g(input_complex);
    vector<Operation*> collapses;
    RandomHomology r(g);
    r.run(.7, collapses);
    //Utilities::WriteCollapsesToFile(output_collapses, collapses);
    
    Viewer v;
    //v.DrawGIC(g);
    v.ViewCollapses(g, collapses);

//    SimpersWrapper::Run(collapses, output_collapses);
//    // read in Simpers Output and format
//    vector<vector<Barcode*>> barcodes;
//    string bc_fp = output_collapses + "_out_file_2";
//    
//    Utilities::ReadInBarcode(bc_fp, barcodes);
}

// For sayan
//    vector<vector<double>> pts;
//    Utilities::ReadInPoints(pts, "/Users/billvarcho/Desktop/OLD/Data/bunny_points.txt");
//    MortonCode *mc = new MortonCode(pts);
//
//    for (int i = 0; i < pts.size(); i++) {
//        MortonPoint n = mc->nextPoint();
//        // TODO(me): modify this to respect input parameter alpha
//        if (i%10 != 0 & i%10 != 5) { //  somewhat arbitrary, should include alpha
//            cout << n.point[0] << " " << n.point[1] << " " << n.point[2] << endl;
//        }
//    }


//KNN_Graph *g = new KNN_Graph("/Users/billvarcho/Desktop/Tests/FundamentalObjects/Torus.txt", 8);
//g->writeToFile();

// display Barcodes in Simpers
//    cout << barcodes.size() << endl;
//    for (int i = 0; i < barcodes.size(); i++) {
//        for (int j = 0; j < barcodes[i].size(); j++) {
//            cout << barcodes[i][j]->start << " " << barcodes[i][j]->end << " " << i << endl;
//        }
//        cout << endl << endl;
//    }







//
////    MortonCode *mc = new MortonCode(g);
//
//    Viewer v;
//    Viewer v1;
//    Viewer v2;
//    //    //v1.DrawPoints(g.pts);
//    //    //v1.DrawMortonCode(*mc);
//    //v.DrawMortonCode(*mc);
//    v1.DrawGIC(g);
//    //v2.ViewCollapses(g, collapses);

