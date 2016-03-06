//
//  Vertex.cpp
//  RandomHomology
//
//  Created by Bill Varcho on 2/29/16.
//  Copyright © 2016 Bill Varcho. All rights reserved.
//

#include "Vertex.hpp"

int Vertex::POS_INDEX = 0;
int Vertex::NEXT_POS_INDEX = 1;

void Vertex::updateIndices() {
    POS_INDEX = (POS_INDEX + 1) % 10;
    NEXT_POS_INDEX = (NEXT_POS_INDEX + 1) % 10;
}