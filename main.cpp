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

using std::vector;
using std::priority_queue;
using std::cout;
using std::endl;

int main(int argc, const char * argv[]) {

//    int dim = 3;
//    vector<vector<double>> points;
//    vector<double> mins;
//    vector<double> maxes;
//    double min, max;
//    Utilities::CreateRandomPoints(points, mins, maxes, dim, 10000);
//    Utilities::ReadInGICPoints(points, min, max, dim,
//                                                   "/Users/billvarcho/Desktop/Data/Torus/torus.txt");
//    mins.push_back(min);
//    maxes.push_back(max);
//    // Create a Morton Code ordering of the points
//    MortonCode *mc = new MortonCode(points, mins, maxes);
//    Viewer v;
//    v.DrawMortonCode(*mc);

    GIC g("/Users/billvarcho/Desktop/Data/PinchedTorus/out_complex.txt");
    std::vector<Operation*> collapses;
    RandomHomology r(g);
    r.run(.7, collapses);
    for (const auto t: collapses) {
        t->Print();
    }
    MortonCode *mc = new MortonCode(g);
    Viewer v;
//    v.DrawPoints(g.pts);
    v.DrawMortonCode(*mc);
    
//    GIC g("/Users/billvarcho/Desktop/Data/outfile.txt_complex.txt");
//    //GIC g;
//    GIC g("/Users/billvarcho/Desktop/Data/Torus/out_torus_complex.txt");
//    Viewer v1;
//    v1.DrawGIC(g);
}
