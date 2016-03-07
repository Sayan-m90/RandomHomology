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
    // int dim = 3;
    // vector<vector<double>> points;
    // vector<double> mins;
    // vector<double> maxes;
    // double min, max;
    // Utilities::CreateRandomPoints(points, mins, maxes, dim, 10000);
    
    //GIC g("/Users/billvarcho/Desktop/Data/Torus/out_torus_complex.txt");
    GIC g("/Users/billvarcho/Desktop/Data/BunnyDense/out_bunny_complex.txt");
    vector<Operation*> collapses;
    RandomHomology r(g);
    r.run(.7, collapses);
    for (const auto t: collapses) {
        t->Print();
    }
    
    //    MortonCode *mc = new MortonCode(g);
    Viewer v1;
    Viewer v2;
    //    //v1.DrawPoints(g.pts);
    //    //v1.DrawMortonCode(*mc);
    //v1.DrawGIC(g);
    v2.ViewCollapses(g, collapses);
}
