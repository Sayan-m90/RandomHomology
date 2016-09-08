//
//  Vertex.hpp
//  RandomHomology
//
//  Created by Bill Varcho on 2/29/16.
//  Copyright © 2016 Bill Varcho. All rights reserved.
//

#ifndef Vertex_hpp
#define Vertex_hpp

#include <stdio.h>
#include <vector>
#include "glm/glm.hpp"
#include "Constants.hpp"

using std::vector;

struct Vertex{
    glm::vec4 position;
    glm::vec3 normal;
    glm::vec4 color;
    glm::vec4 outline_color;
    glm::vec2 textureCoord;
    glm::vec4 next_position;
    // TODO(me): need better way of setting this
    glm::vec4 positions[90];
    static int POS_INDEX;
    static int NEXT_POS_INDEX;
    //Helper functions to get memory offsets for GPU
    static void updateIndices(int max_time);
    static size_t offsetNormal(){       return sizeof(glm::vec4);}
    static size_t offsetColor(){        return sizeof(glm::vec3) + offsetNormal();}
    static size_t offsetColorOutline(){ return sizeof(glm::vec4) + offsetColor();}
    static size_t offsetTexture(){      return sizeof(glm::vec4) + offsetColorOutline();}
    static size_t offsetNextPos(){      return sizeof(glm::vec2) + offsetTexture();}
    static size_t offset_POS() {        return offsetNextPos() + (POS_INDEX + 1) * sizeof(glm::vec4);}
    static size_t offset_NEXT_POS() {   return offsetNextPos() + (NEXT_POS_INDEX + 1) * sizeof(glm::vec4);}
};

#endif /* Vertex_hpp */
