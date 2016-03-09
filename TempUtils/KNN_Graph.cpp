//
//  KNN_Graph.cpp
//  RandomHomology
//
//  Created by Bill Varcho on 3/8/16.
//  Copyright © 2016 Bill Varcho. All rights reserved.
//

#include "KNN_Graph.hpp"

/*----------------*
 | Constructors
 *----------------*/

void KNN_Graph::Construct() {
    if (points.size() > 0) {
        dim = points[0].size();
        
        // create ANN_KD tree from points
        KD_tree = Utilities::ConstructKDTree(points, dim);
        
        // for every point find K-NN and create an edge between them
        int index = 0;
        for (vector<double> point : points) {
            ANNpoint query_point;
            ANNidxArray nn_ids;
            ANNdistArray dists;
            double eps = .1;
            query_point = annAllocPt(KD_tree->theDim(), 0.);
            nn_ids = new ANNidx[K + 1];
            dists = new ANNdist[K + 1];
            
            for (int d = 0; d < KD_tree->theDim(); d++) {
                query_point[d] = point[d];
            }
            
            KD_tree->annkSearch(query_point,
                                K + 1,
                                nn_ids,
                                dists,
                                eps);
            
            for (int i = 0; i < K + 1; i++) {
                if (index != nn_ids[i]) {
                    int min_id, max_id;
                    min_id = min(index, nn_ids[i]);
                    max_id = max(index, nn_ids[i]);
                    edges.insert(pair<int, int>(min_id, max_id));
                }
            }
            index++;
        }
    } else {
        dim = 0;
    }
}

KNN_Graph::KNN_Graph(int K_) {
    K = K_;
    vector<vector<double>> pts;
    points = pts;
    Construct();
}

KNN_Graph::KNN_Graph(string fp, int K_) {
    K = K_;
    vector<vector<double>> pts;
    Utilities::ReadInPoints(pts, fp);
    points = pts;
    Construct();
}

KNN_Graph::KNN_Graph(vector<vector<double>> pts, int K_) {
    K = K_;
    points = pts;
    Construct();
}

void KNN_Graph::writeToFile() { // lazy and want to go to bed so just print
    cout << dim << " " << points.size() << endl;
    for (vector<double> pt : points) {
        for (int i = 0; i < pt.size(); i++) {
            cout << pt[i] << " ";
        }
        cout << endl;
    }
    for (pair<int, int> edge : edges) {
        cout << 2 << " " << edge.first << " " << edge.second << endl;
    }
}
