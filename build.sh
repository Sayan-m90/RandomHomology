#!/bin/sh

LIBS="-lann -lGLEW -lglfw3 -lGL -lglut -lboost_system -lX11 -lXxf86vm -lpthread -ldl -lXrandr -lXinerama -lXcursor -lboost_filesystem -lboost_program_options"

g++ -o morton --std=c++11 *.cpp Wrappers/*.cpp SimPers/*.cpp Graphs/*.cpp GIComplex/*.cpp $LIBS 
