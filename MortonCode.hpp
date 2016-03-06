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
#include "GIC.hpp"
#include "Constants.hpp"

using std::vector;
using std::priority_queue;
using std::cout;
using std::endl;

class MortonPoint {
    //static int id;
 public:
    int p_id;
    vector<double> point;
    // for the bit interleaving we need len(point) * 64 bits
    // which are stored here (in Big Endian)
    vector<uint64_t> code;

    MortonPoint(vector<double> pt, int min, int max, int id);
    ~MortonPoint() {}
};

bool operator<(const MortonPoint &a, const MortonPoint &b);

class MortonCode {
 protected:
    double bbox_min, bbox_max;

 public:
    priority_queue<MortonPoint> queue;
    explicit MortonCode(const vector<vector<double>> &points);
    MortonCode(const vector<vector<double>> &points,
               const vector<double> &min_bounds,
               const vector<double> &max_bounds);
    MortonCode(GIC g);
    ~MortonCode() {}

    void add(vector<double> pt, int id);
    void add(MortonPoint pt);
    vector<double> next();
    MortonPoint nextPoint();
    void setPoints(vector<MortonPoint> pts);
    bool empty();
    int size();
};

#endif /* MortonCode_hpp */
