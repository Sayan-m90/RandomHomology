//
//  MortonCode.hpp
//  BitTests
//
//  Created by Bill Varcho on 2/24/16.
//  Copyright © 2016 Bill Varcho. All rights reserved.
//

#ifndef MortonCode_hpp
#define MortonCode_hpp

#include <stdio.h>
#include <algorithm>
#include <iostream>
#include <queue>
#include <vector>

using std::vector;
using std::priority_queue;
using std::cout;
using std::endl;

class MortonPoint {
    static int id;
 public:
    int p_id;
    vector<double> point;
    // for the bit interleaving we need len(point) * 64 bits
    // which are stored here (in Big Endian)
    vector<uint64_t> code;

    MortonPoint(vector<double> pt, int min, int max);
    ~MortonPoint() {}
};

class MortonCode {
 protected:
    priority_queue<MortonPoint> queue;
    double bbox_min, bbox_max;

 public:
    explicit MortonCode(const vector<vector<double>> &points);
    MortonCode(const vector<vector<double>> &points,
               const vector<double> &min_bounds,
               const vector<double> &max_bounds);
    ~MortonCode() {}

    void add(vector<double> pt);
    vector<double> next();
    bool empty();
};

#endif /* MortonCode_hpp */
