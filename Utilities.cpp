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

// TODO(me): Generalize for higher dimensions
// Deduped code (can probably delete later)
void Utilities::ReadInGICPoints(vector<vector<double>> &pts,
                                double &min_n, double &max_n,
                                int &dim, string fp) {
    ifstream input(fp);
    bool done = false;
    int num_points;
    min_n = std::numeric_limits<double>::max();
    max_n = std::numeric_limits<double>::min();
    
    if (input.is_open()) {
        // get the first line (dim, num pts)
        string s_dim, s_num, tmp;
        stringstream ss(stringstream::in | stringstream::out);
        getline(input, s_dim, ' ');
        getline(input, s_num);
        ss << s_dim << s_num;
        
        dim = stoi(s_dim);
        if (dim != 3) {
            throw std::runtime_error("Can only read in 3D point data for now.");
        }
        num_points = stoi(s_num);
        
        // TODO(me): read in points separated by spaces and tabs
        int point_count = 0;
        while (point_count < num_points) {
            done = input.eof();
            std::string sx, sy, sz;
            double x, y, z;
            std::stringstream ss(std::stringstream::in |
                                 std::stringstream::out);
            string s;
            getline(input, s);
            vector<string> strs;
            boost::split(strs, s, boost::is_any_of("\t "));
            vector<double> pt;
            for (size_t i = 0; i < strs.size(); i++) {
                if (strs[i].size() > 0) {
                    double coord = stod(strs[i]);
                    pt.push_back(coord);
                    min_n = min(min_n, coord);
                    max_n = max(max_n, coord);
                }
            }
            
            min_n = min(min_n, min(x, min(y, z)));
            max_n = max(max_n, max(x, max(y, z)));
            
            // add point to the vector
            pts.push_back(pt);
            point_count += 1;
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
