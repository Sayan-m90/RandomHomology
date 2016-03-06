//
//  Collapse.cpp
//  RandomHomology
//
//  Created by Bill Varcho on 3/5/16.
//  Copyright © 2016 Bill Varcho. All rights reserved.
//

#include "Collapse.hpp"

Insert::Insert() {
    v_id = -1;
}

Insert::Insert(int id) {
    v_id = id;
}

void Insert::Print() {
    cout << "insert " << v_id << endl;
}

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

Timestamp::Timestamp() {
    v_time = -1;
}

Timestamp::Timestamp(float time) {
    v_time = time;
}

void Timestamp::Print() {
    cout << "# " << v_time << endl;
}