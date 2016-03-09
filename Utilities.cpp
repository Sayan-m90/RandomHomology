//
//  Utilities.cpp
//  BitTests
//
//  Created by Bill Varcho on 2/24/16.
//  Copyright © 2016 Bill Varcho. All rights reserved.
//

#include "Utilities.hpp"

void Utilities::CreateRandomPoints(vector<vector<double>> &pts,
                                   vector<double> &mins,
                                   vector<double> &maxes,
                                   int dim, int n) {
    for (int i = 0; i < n; i++) {
        vector<double> point;
        for (int j = 0; j < dim; j+=1) {
            double coord;
            coord = rand() / ((double) INT_MAX);
            coord = 2*coord - 1;
            point.push_back(coord);
            if (mins.size() > 0) {
                if (coord < mins[0]) {
                    mins[0] = coord;
                }
            } else { mins.push_back(coord);}
            if (maxes.size() > 0) {
                if (coord > maxes[0]) {
                    maxes[0] = coord;
                }
            } else { maxes.push_back(coord);}
        }
        pts.push_back(point);
    }
}

void Utilities::ReadInPoints(vector<vector<double>> &pts, string fp) {
    ifstream input(fp);
    if (input.is_open()) {
        while (!input.eof()) {
            std::string sx, sy, sz;
            double x, y, z;
            std::stringstream ss(std::stringstream::in |
                                 std::stringstream::out);
            string s;
            getline(input, s);
            vector<string> strs;
            boost::split(strs, s, boost::is_any_of(Constants::SEPARATOR_STR));
            vector<double> pt;
            for (size_t i = 0; i < strs.size(); i++) {
                if (strs[i].size() > 0) {
                    double coord = stod(strs[i]);
                    pt.push_back(coord);
                }
            }
            // add point to the vector
            pts.push_back(pt);
        }
    }
}

ANNkd_tree* Utilities::ConstructKDTree(vector<vector<double>> pts, int dim) {
    int max_points = pts.size();
    ANNpointArray data_points;
    data_points = annAllocPts(max_points, dim);
    
    // pts to data points
    for (int i = 0; i < max_points; i++) {
        ANNpoint new_p = annAllocPt(dim);
        vector<double> current_pt = pts[i];
        for (int d = 0; d < dim; d++) {
            new_p[d] = current_pt[d];
        }
        data_points[i] = new_p;
    }
    
    // create and return the kdtree
    return new ANNkd_tree(data_points,
                          max_points,
                          dim);
}
