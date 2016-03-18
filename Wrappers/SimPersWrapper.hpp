//
//  SimPersWrapper.hpp
//  RandomHomology
//
//  Created by Bill Varcho on 3/10/16.
//  Copyright © 2016 Bill Varcho. All rights reserved.
//

#ifndef SimPersWrapper_hpp
#define SimPersWrapper_hpp

#include <stdio.h>
#include <vector>

#include "Barcode.hpp"
#include "Collapse.hpp"

using std::vector;

class SimpersWrapper {
    static void Run(vector<Collapse *> c, vector<Barcode> &output);
};

#endif /* SimPersWrapper_hpp */
