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
            } else {
                mins.push_back(coord);
            }
            if (maxes.size() > 0) {
                if (coord > maxes[0]) {
                    maxes[0] = coord;
                }
            } else {
                maxes.push_back(coord);
            }
        }
        pts.push_back(point);
    }
}

// TODO(me): Generalize for higher dimensions
void Utilities::ReadInGICPoints(vector<vector<double>> &pts,
                                double &min, double &maxes,
                                int &dim, string fp) {
    ifstream input(fp);
    bool done = false;
    int num_points;

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

        int point_count = 0;
        while (point_count < num_points) {
            done = input.eof();
            std::string sx, sy, sz;
            double x, y, z;
            std::stringstream ss(std::stringstream::in |
                                  std::stringstream::out);
            getline(input, sx, ' ');
            getline(input, sy, ' ');
            getline(input, sz);
            ss << sx << sy << sz;

            x = stod(sx);
            y = stod(sy);
            z = stod(sz);

            // create point
            vector<double> pt;
            pt.push_back(x);
            pt.push_back(y);
            pt.push_back(z);

            // add point to the vector
            pts.push_back(pt);

            point_count += 1;
        }
    }
}
