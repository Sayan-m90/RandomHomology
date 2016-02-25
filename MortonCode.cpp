//
//  MortonCode.cpp
//  BitTests
//
//  Created by Bill Varcho on 2/24/16.
//  Copyright © 2016 Bill Varcho. All rights reserved.
//

#include "MortonCode.hpp"

// Custom comparator for the large bitstrings create via the encoding
bool operator<(const MortonPoint &a, const MortonPoint &b) {
    for (int i = 0; i < a.code.size() - 1; i += 1) {
        if (a.code[i] != b.code[i]) {
            return a.code[i] < b.code[i];
        }
    }
    return a.code[a.code.size() - 1] < b.code[b.code.size() - 1];
}

/*------------------------*
 |  Constructors          |
 *------------------------*/

MortonPoint::MortonPoint(const vector<double> pt, int min, int max) {
    point = pt;
    vector<uint64_t> scaled_coords;

    // TODO(me): 100,000 multiplier is somewhat arbitrary for now, find
    // a better constant
    for (int i = 0; i < pt.size(); i++) {
        scaled_coords.push_back((uint64_t) 100000 * ((max - min)
                                               + pt[i]) / (2.));
        code.push_back(0);
    }

    // now need to set the Morton Code of the point
    int index = 0;  // current position in 64 bit string
    int count = 0;  // which index in code we are writing to
    int coord_index = 0;
    for (int j = 0; j< 64 * pt.size(); j += 1) {
        uint64_t mask = 1;
        mask = mask << (63 - index);
        uint64_t bit = (mask & scaled_coords[coord_index]) >> (63 - index);

        // now that we have the proper bit set the code
        code[count] = (code[count] << 1) | bit;

        if (j % pt.size() == pt.size() - 1) { index += 1; }
        if (j % 64 == 63) { count += 1;}
        // decrement coord index
        coord_index = (coord_index + 1) % (pt.size());
    }
}


/*-----------------------*
 |  Constructors         |
 *-----------------------*/

MortonCode::MortonCode(const vector<vector<double>> &points) {
    bbox_max = 1;
    bbox_min = -1;
    priority_queue<MortonPoint> q;
    for (vector<double> pt : points) {
        add(pt);
    }
}

MortonCode::MortonCode(const vector<vector<double>> &points,
                       const vector<double> &min_bounds,
                       const vector<double> &max_bounds) {
    bbox_min = *min_element(min_bounds.begin(), min_bounds.end()) + .1;
    bbox_max = *max_element(min_bounds.begin(), min_bounds.end()) - .1;
    priority_queue<MortonPoint> q;
    for (vector<double> pt : points) {
        add(pt);
    }
}

/*-----------------------*
 |  Class Methods        |
 *-----------------------*/

void MortonCode::add(vector<double> pt) {
    MortonPoint mp = MortonPoint(pt, bbox_min, bbox_max);
    queue.push(mp);
}

vector<double> MortonCode::next() {
    if (!empty()) {
        MortonPoint next_point = queue.top();
        queue.pop();
        return next_point.point;
    } else {
        vector<double> x;
        x.push_back(-1);
        return x;
    }
}

bool MortonCode::empty() {
    return !(queue.size() > 0);
}
