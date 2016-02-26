//
//  Utilities.hpp
//  BitTests
//
//  Created by Bill Varcho on 2/24/16.
//  Copyright © 2016 Bill Varcho. All rights reserved.
//

#ifndef Utilities_hpp
#define Utilities_hpp

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include "ANN/ANN.h"
#include "MortonCode.hpp"

using std::string;
using std::vector;
using std::ifstream;
using std::stringstream;
using std::cout;
using std::endl;
using std::min;
using std::max;

class Utilities {
public:
    
    static ANNkd_tree *ConstructKDTree(vector<vector<double>>, int dim);
    static void ReadInGICPoints(vector<vector<double>> &pts,
                                double &mins,double &maxes,
                                int &dim, string gic_fp);
    
    static void ReadInPoints(vector<vector<double>> &pts, int dim, string fp);
    // Used mainly for testing
    static void CreateRandomPoints(vector<vector<double>> &pts,
                                   vector<double> &mins,
                                   vector<double> &maxes,
                                   int dim, int n);
    static void WriteCollapsesToFile(string fp);
};

#endif /* Utilities_hpp */
