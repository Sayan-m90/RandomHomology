//
//  RandomHomology.cpp
//  BitTests
//
//  Created by Bill Varcho on 2/24/16.
//  Copyright © 2016 Bill Varcho. All rights reserved.
//

#include "RandomHomology.hpp"

RandomHomology::RandomHomology(vector<vector<double>> &points,
                               vector<double> &min_bounds,
                               vector<double> &max_bounds, int dim) {
    MortonCode *mc = new MortonCode(points, min_bounds, max_bounds);
}

void RandomHomology::run(double alpha) {}
