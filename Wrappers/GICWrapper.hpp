//
//  GICWrapper.hpp
//  RandomHomology
//
//  Created by Bill Varcho on 3/10/16.
//  Copyright © 2016 Bill Varcho. All rights reserved.
//

#ifndef GICWrapper_hpp
#define GICWrapper_hpp

#include <stdio.h>
#include "KNN_Graph.hpp"
#include "GIC.hpp"

class GICWrapper {
    static void Run(KNN_Graph *g, GIC &output);
};

#endif /* GICWrapper_hpp */
