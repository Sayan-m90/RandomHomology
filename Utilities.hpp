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
#include <boost/algorithm/string.hpp>
#include "ANN/ANN.h"
#include "MortonCode.hpp"
#include "Viewer.hpp"
#include "Barcode.hpp"

using std::string;
using std::vector;
using std::ifstream;
using std::ofstream;
using std::stringstream;
using std::cout;
using std::endl;
using std::min;
using std::max;

class Utilities {
public:
    
    static ANNkd_tree *ConstructKDTree(vector<vector<double>>, int dim);
    static void ReadInPoints(vector<vector<double>> &pts, string fp);
    // Used mainly for testing
    static void CreateRandomPoints(vector<vector<double>> &pts,
                                   vector<double> &mins,
                                   vector<double> &maxes,
                                   int dim, int n);
    static void WriteCollapsesToFile(string fp, vector<Operation*> &collapses);
    static void ReadInBarcode(string fp, vector<vector<Barcode*>> &bc);
};

#endif /* Utilities_hpp */
