//
//  Vertex.cpp
//  RandomHomology
//
//  Created by Bill Varcho on 2/29/16.
//  Copyright © 2016 Bill Varcho. All rights reserved.
//

#include "Vertex.hpp"
#include <algorithm>

int Vertex::POS_INDEX = 0;
int Vertex::NEXT_POS_INDEX = 1;

using std::min;

void Vertex::updateIndices(int max_time) {
    int time = min(max_time + 2, Constants::MAX_NUM_COLLAPSES);
    POS_INDEX = (POS_INDEX + 1) % time;
    NEXT_POS_INDEX = (NEXT_POS_INDEX + 1) % time;
}