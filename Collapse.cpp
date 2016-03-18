//
//  Collapse.cpp
//  RandomHomology
//
//  Created by Bill Varcho on 3/5/16.
//  Copyright © 2016 Bill Varcho. All rights reserved.
//

#include "Collapse.hpp"

/*----------------------*
 |  Insert Operation
 *----------------------*/

Insert::Insert() {
}

Insert::Insert(int id) {
    v_id.push_back(id);
}

Insert::Insert(vector<int> ids) {
    for (int id : ids) {
        v_id.push_back(id);
    }
}

void Insert::Print() {
    cout << "insert ";
    for (int i = 0; i < v_id.size() - 1; i++) {
        cout << v_id[i] << " ";
    }
    cout << v_id[v_id.size() - 1] << endl;
}

OP_TYPE Insert::Type() {
    return INSERT_OP;
}


/*----------------------*
 |  Collapse Operation
 *----------------------*/

Collapse::Collapse() {
    v_start = -1;
    v_target = -1;
}

Collapse::Collapse(int start, int target) {
    v_start = start;
    v_target = target;
}

void Collapse::Print() {
    cout << "collapse " << v_start << " to " << v_target << endl;
}

OP_TYPE Collapse::Type() {
    return COLLAPSE_OP;
}


/*----------------------*
 |  Timestamp Operation
 *----------------------*/

Timestamp::Timestamp() {
    v_time = -1;
}

Timestamp::Timestamp(float time) {
    v_time = time;
}

void Timestamp::Print() {
    cout << "# " << v_time << endl;
}

OP_TYPE Timestamp::Type() {
    return TIME_OP;
}
