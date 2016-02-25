//
//  RandomHomology.hpp
//  BitTests
//
//  Created by Bill Varcho on 2/24/16.
//  Copyright © 2016 Bill Varcho. All rights reserved.
//

#ifndef RandomHomology_hpp
#define RandomHomology_hpp

#include <stdio.h>
#include <vector>
#include "MortonCode.hpp"
#include "Utilities.hpp"

class RandomHomology {
public:
    int dim;
    MortonCode *mc;
    RandomHomology(vector<vector<double>> &points,
                   vector<double> &min_bounds,
                   vector<double> &max_bounds, int dim);
    ~RandomHomology() {};
    void run(double alpha);
};

#endif /* RandomHomology_hpp */
