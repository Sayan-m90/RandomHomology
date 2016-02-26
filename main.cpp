//
//  main.cpp
//  BitTests
//
//  Created by Bill Varcho on 2/24/16.
//  Copyright © 2016 Bill Varcho. All rights reserved.
//

#include <iostream>
#include <vector>
#include "RandomHomology.hpp"
#include "MortonCode.hpp"
#include "Utilities.hpp"

using std::vector;
using std::priority_queue;
using std::cout;
using std::endl;

int main(int argc, const char * argv[]) {

    /*
    int dim = 3;
    vector<vector<double>> points;
    vector<double> mins;
    vector<double> maxes;
    Utilities::CreateRandomPoints(points, mins, maxes, dim, 1000);
    
    // Create a Morton Code ordering of the points
    MortonCode *mc = new MortonCode(points, mins, maxes);

    // Print out the points in Morton Order to verify correctness
    while (!mc->empty()) {
        vector<double> p = mc->next();
        for (int j = 0; j < dim; j+=1) {
            cout << p[j] << ' ';
        }
        cout << endl;
    }*/
    
    vector<vector<double>> pts;
    vector<double> mins;
    vector<double> maxes;
    double min, max;
    int dim;
    // TODO()me: Set filepath as command line arg
    Utilities::ReadInGICPoints(pts, min, max, dim,
                    "/Users/billvarcho/Desktop/Data/pinched_torus.txt");
    mins.push_back(min);
    maxes.push_back(max);
    //cout << min << endl;
    //cout << max << endl;
    
    RandomHomology r(pts, mins, maxes, dim);
    r.run(.7);
}
