//
//  RandomHomology.cpp
//  BitTests
//
//  Created by Bill Varcho on 2/24/16.
//  Copyright © 2016 Bill Varcho. All rights reserved.
//

#include "RandomHomology.hpp"

RandomHomology::RandomHomology(vector<vector<double>> &points,
                               vector<double> &min_bounds,
                               vector<double> &max_bounds, int _dim) {
    dim = _dim;
    initial_points = points;
    mc = new MortonCode(points, min_bounds, max_bounds);
    kd_tree = Utilities::ConstructKDTree(points, dim);
}

// exponential backoff on kdtree
void RandomHomology::collapseToClosest(MortonPoint p) {
    removed_ids.insert(p.p_id);
    
    bool neighbor_found = false;
    int current_index = 0;
    int i = 0;
    while (!neighbor_found) {
        
        // DO ann Query here
        ANNpoint query_point;
        ANNidxArray nn_ids;
        ANNdistArray dists;
        int k = (int) pow(2, i);
        double eps = .1;
        query_point = annAllocPt(dim, 0.);
        nn_ids = new ANNidx[k];
        dists = new ANNdist[k];
        
        query_point[0] = p.point[0];
        query_point[1] = p.point[1];
        query_point[2] = p.point[2];
        kd_tree->annkSearch(query_point,
                            k,
                            nn_ids,
                            dists,
                            eps);
        
        int check_cout = ceil(pow(2, i-1));
        // check for first point remaining
        for (int j = 0; j < check_cout; j++) {
            set<int>::iterator it;
            it = removed_ids.find(nn_ids[current_index]);
            if (it == removed_ids.end()) { // id hasn't been removed yet, so collapse to it
                // This details the collapses
                //cout << p.p_id << '\t' << nn_ids[current_index] << endl;
                
                cout << "collapse " << p.p_id << " to " << nn_ids[current_index] << endl;
                j = check_cout;
                neighbor_found = true;
            }
            current_index += 1;
        }
        i += 1;
    }
}

void RandomHomology::run(double alpha) {
    
    int count = 0;
    while (mc->size() > 3) {
        // this is for every iteration
        int s = mc->size();
        // create another queue
        vector<MortonPoint> keep_pts;
        vector<MortonPoint> remove_pts;
        for (int i = 0; i < s; i++) {
            MortonPoint n = mc->nextPoint();
            
            if (i%3 != 0) { // somewhat arbitrary, should include alpha
                keep_pts.push_back(n);
            } else {
                remove_pts.push_back(n);
            }
        }
        
        // for debugging
        /*for (MortonPoint pt: keep_pts) {
            cout << pt.point[0] << '\t' << pt.point[1] << '\t' << pt.point[2] << endl;
        }*/

        for (MortonPoint p : remove_pts) {
            collapseToClosest(p);
        }
        
        
        mc->setPoints(keep_pts);
        count += 1;
        cout << "# " << count << endl;///<< endl<< endl<< endl;
    }
}
