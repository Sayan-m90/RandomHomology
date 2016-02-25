//
//  main.cpp
//  BitTests
//
//  Created by Bill Varcho on 2/24/16.
//  Copyright © 2016 Bill Varcho. All rights reserved.
//

#include <iostream>
#include <vector>
#include "MortonCode.hpp"

using std::vector;
using std::priority_queue;
using std::cout;
using std::endl;

int main(int argc, const char * argv[]) {

    // Set the Dimension, and create some random points
    int dim = 3;
    vector<double> mins;
    vector<double> maxes;
    vector<vector<double>> points;
    for (int i = 0; i < 4000; i++) {
        vector<double> point;
        for (int j = 0; j < dim; j+=1) {

            double coord;
            coord = rand() / ((double) INT_MAX);
            coord = 2*coord - 1;

            point.push_back(coord);

            if (mins.size() > 0) {
                if (coord < mins[0]) {
                    mins[0] = coord;
                }
            } else {
                mins.push_back(coord);
            }

            if (maxes.size() > 0) {
                if (coord > maxes[0]) {
                    maxes[0] = coord;
                }
            } else {
                maxes.push_back(coord);
            }
        }
        points.push_back(point);
    }

    // Create a Morton Code ordering of the points
    MortonCode *mc = new MortonCode(points, mins, maxes);

    // Print out the points in Morton Order to verify correctness
    while (!mc->empty()) {
        vector<double> p = mc->next();
        for (int j = 0; j < dim; j+=1) {
            cout << p[j] << ' ';
        }
        cout << endl;
    }
}
