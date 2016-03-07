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

    GIC g("/Users/billvarcho/Desktop/Data/out_bunny_complex.txt");
    std::vector<Operation*> collapses;
    RandomHomology r(g);
    r.run(.7, collapses);
    for (const auto t: collapses) {
        t->Print();
    }
 
}
