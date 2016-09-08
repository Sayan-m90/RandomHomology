//
//  Viewer.hpp
//  RandomHomology
//
//  Created by Bill Varcho on 2/28/16.
//  Copyright © 2016 Bill Varcho. All rights reserved.
//

#ifndef Viewer_hpp
#define Viewer_hpp
#define GLM_SWIZZLE

#include <stdio.h>

#include <GL/glew.h>
#ifdef __APPLE__
#include <OpenGL/gl3.h>
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <math.h>
#include <thread>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtx/quaternion.hpp"
#include <glm/gtc/type_ptr.hpp>
#include "Barcode.hpp"
#include "Collapse.hpp"
#include "Constants.hpp"
#include "GIC.hpp"
#include "MortonCode.hpp"
#include "Vertex.hpp"

using std::vector;
using glm::vec4;
using glm::vec3;
using glm::cross;
using glm::normalize;
using glm::mat4;
using glm::quat;

struct Mouse{
    bool is_down;
    int last_X, last_Y;
    int dx, dy;
    int x, y;
};

enum DRAW_LOOP { POINT_LOOP, GIC_LOOP, COLLAPSE_LOOP, BARCODE_LOOP };

enum DRAW_TYPE { POINTS_ONLY, POINTS_AND_LINES, ALL};

class Viewer {
private:
    bool draw_gic = false;
    bool draw_barcode = false;
    bool first_mouse = true;
    bool keys[1024];
    bool points_given = false;
    float time = 0.0;
    float view_scale;
    int display_type;
    int max_time;
    GLenum mode = GL_POINTS; // Drawing Mode (Points, Lines, Tris)
    GLfloat deltaTime = 0.0f;  // Time between current frame and last frame
    GLfloat lastFrame = 0.0f;  // Time of last frame
    GLfloat lastX = 400, lastY = 300;
    GLfloat fov;
    GLfloat yaw = 0.0, pitch = 0.0;
    GLint num_points, dim;
    GLint WIDTH, HEIGHT;
    GLuint position_ID, color_ID, normal_ID;
    GLuint shaderProgram;
    mat4 model, projection, view;
    vec3 cam_pos, cam_up, cam_front;
    quat tmpRot;
    quat mRot;
    GLFWwindow *window;
    int keyCode;
    int draw_type = 0;
    int barcode_dim;
    vector<vec4> barcode_colors;
    Mouse mouse;
    string filepath;
    vector<Vertex> vertices;
    vector<int> line_indices;
    vector<int> pt_indices;
    vector<int> tri_indices;
    string dim_permutation_string;
    bool pause;
    float pause_time;
    bool freeze_rotate;
    bool is_panning;
    
    // Read a shader source from a file
    // store the shader source in a std::vector<char>
    void read_shader_src(const char *fname, std::vector<char> &buffer);

    // Compile a shader
    GLuint load_and_compile_shader(const char *fname, GLenum shaderType);

    // Create a program from two shaders
    GLuint create_program(const char *path_vert_shader, const char *path_frag_shader);

    // Called when the window is resized
    static void window_resized(GLFWwindow* window, int width, int height);
    
    // Called for keyboard events
    void keyboard(int key, int action);

    // Render scene
    //void display(GLuint &vao);
    
    void GetBarCodeColors(vec4& start, vec4& end, int dim);
    void AddBarcodeTitle(string s);
    void AddBarcodeDimTitle(int dim, vec3 pos, vec3 col);

    // Initialize the data to be rendered
    void initialize(GLuint &vao);
    
    // GLFW Window Callbacks
    static void mouse_down_callback(GLFWwindow* window, int button, int action, int mods);
    static void mouse_drag_callback(GLFWwindow* window, int x, int y);
    static void mouse_move_callback(GLFWwindow* window, double x, double y);
    static void error_callback(int error, const char* description);
    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
    
    void DoMovement();
    void DrawFont(string text, void* font, GLint x, GLint y, glm::vec3 col);
    
    void MainLoop(DRAW_LOOP type);
    void DisplayPoint(GLuint &vao);
    void DisplayGIC(GLuint &vao);
    void DisplayCollapses(GLuint &vao);
    void DisplayBarcodes(GLuint &vao);
    void DrawText();
    
public:
    Viewer();
    ~Viewer() {};
    void DrawPoints(vector<vector<float>> pts);
    void DrawMortonCode(MortonCode &c);
    void DrawGIC(GIC &g);
    void ViewCollapses(GIC &g, vector<Operation *> c);
    void ViewBarcode(vector<vector<Barcode *>> &bc);
};


#endif /* Viewer_hpp */
