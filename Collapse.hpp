//
//  Collapse.hpp
//  RandomHomology
//
//  Created by Bill Varcho on 3/5/16.
//  Copyright © 2016 Bill Varcho. All rights reserved.
//

#ifndef Collapse_hpp
#define Collapse_hpp

#include <stdio.h>
#include <iostream>

using std::cout;
using std::endl;

enum OP_TYPE {INSERT_OP, COLLAPSE_OP, TIME_OP, NULL_OP};

class Operation {
public:
    virtual void Print() = 0;
    virtual OP_TYPE Type() = 0;
};

class Insert : public Operation {
public:
    int v_id;
    Insert();
    Insert(int v_id);
    ~Insert() {}
    virtual void Print();
    virtual OP_TYPE Type();
};

class Collapse : public Operation {
public:
    int v_start, v_target;
    Collapse();
    Collapse(int start, int end);
    ~Collapse() {}
    virtual void Print();
    virtual OP_TYPE Type();
};

class Timestamp : public Operation {
public:
    float v_time;
    Timestamp();
    Timestamp(float time);
    ~Timestamp() {}
    virtual void Print();
    virtual OP_TYPE Type();
};

#endif /* Collapse_hpp */
