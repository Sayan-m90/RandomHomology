//
//  Barcode.hpp
//  RandomHomology
//
//  Created by Bill Varcho on 3/10/16.
//  Copyright © 2016 Bill Varcho. All rights reserved.
//

#ifndef Barcode_hpp
#define Barcode_hpp

#include <stdio.h>

class Barcode {
public:
    int dim;
    double start;
    double end;
    Barcode();
    
    bool operator < (Barcode const &other) {
        return ((other.end - other.start) < (end - start));
    }
    
    static bool Compare(Barcode* const a, Barcode* const b) {
        return ((b->end - b->start) < (a->end - a->start));
    }
};

#endif /* Barcode_hpp */
