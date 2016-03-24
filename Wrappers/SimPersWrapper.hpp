//
//  SimPersWrapper.hpp
//  RandomHomology
//
//  Created by Bill Varcho on 3/10/16.
//  Copyright © 2016 Bill Varcho. All rights reserved.
//

#ifndef SimPersWrapper_hpp
#define SimPersWrapper_hpp

#include "Barcode.hpp"
#include "Collapse.hpp"
#include "SimplicialComplexSP.h"
#include <iostream>
#include <stdio.h>
#include <vector>
#include <cmath>
#include <sstream>
#include <unordered_set>

using std::vector;

class SimpersWrapper {
public:
    static void Run(vector<Operation *> c,
                    //vector<Barcode> &output,
                    string fp);
};

#endif /* SimPersWrapper_hpp */
