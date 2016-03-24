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

RandomHomology::RandomHomology(GIC &g) {
    dim = g.dim;
    initial_points = g.pts;
    indices = g.indices;
    mc = new MortonCode(g);
    kd_tree = Utilities::ConstructKDTree(initial_points, dim);
}

// exponential backoff on kdtree
void RandomHomology::collapseToClosest(MortonPoint p,
                                       vector<Operation*> &collapses) {
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

        for (int d = 0; d < dim; d++) {
            query_point[d] = p.point[d];
        }
        
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
            // id hasn't been removed,so collapse to it
            if (it == removed_ids.end()) {
                //  This details the collapses
                Collapse *c = new Collapse(p.p_id, nn_ids[current_index]);
                collapses.push_back(c);
                j = check_cout;
                neighbor_found = true;
            }
            current_index += 1;
        }
        i += 1;
    }
}

void RandomHomology::run(double alpha, vector<Operation*> &collapses) {
    int count = 0;
    // initial inserts
    for (int i = 0; i < indices.size(); i++) {
        for (int j = 0; j < indices[i].size() /(i+1); j++) {
            vector<int> simp;
            for (int k = 0; k < i+1; k++) {
                simp.push_back(indices[i][j*(i+1) + k]);
            }
            Insert *s = new Insert(simp);
            collapses.push_back(s);
        }
    }
    
    // add first timestamp (to separate initial inserts)
    Timestamp *t = new Timestamp(0);
    collapses.push_back(t);
    
    while (mc->size() > 1) {
        // this is for every iteration
        int s = mc->size();
        // create another queue
        vector<MortonPoint> keep_pts;
        vector<MortonPoint> remove_pts;
        for (int i = 0; i < s; i++) {
            MortonPoint n = mc->nextPoint();
            // TODO(me): modify this to respect input parameter alpha
            if (i%10 != 0) { //  somewhat arbitrary, should include alpha
                keep_pts.push_back(n);
            } else {
                remove_pts.push_back(n);
            }
        }
        
        for (MortonPoint p : remove_pts) {
            collapseToClosest(p, collapses);
        }
        
        mc->setPoints(keep_pts);
        count += 1;
        Timestamp *t = new Timestamp(count + 1);
        collapses.push_back(t);
        //cout << "# " << count + 1 << endl;
    }
}
