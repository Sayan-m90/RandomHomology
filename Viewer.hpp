//
//  Viewer.hpp
//  RandomHomology
//
//  Created by Bill Varcho on 2/28/16.
//  Copyright © 2016 Bill Varcho. All rights reserved.
//

#ifndef Viewer_hpp
#define Viewer_hpp

#include <stdio.h>

#include <OpenGL/gl3.h>
#include <OpenGL/glu.h>
#include <OpenGL/gl.h>
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>

#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <math.h>
#include <pthread.h>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtx/quaternion.hpp"
#include <glm/gtc/type_ptr.hpp>

#include "GIC.hpp"
#include "MortonCode.hpp"
#include "Vertex.hpp"
#include "Collapse.hpp"

using std::vector;

struct Mouse{
    bool isDown;
    int lastX, lastY;
    int dx, dy;
    int x, y;
};

class Viewer {
private:
    GLFWwindow *window;
    GLuint shaderProgram;
    GLenum mode = GL_POINTS; // Drawing Mode (Points, Lines, Tris)
    glm::mat4 model, view, projection;
    glm::vec3 cam_pos, cam_up, cam_front;
    bool keys[1024];
    GLfloat deltaTime = 0.0f;  // Time between current frame and last frame
    GLfloat lastFrame = 0.0f;  // Time of last frame
    GLfloat lastX = 400, lastY = 300;
    GLfloat fov;
    GLfloat yaw = 0.0, pitch = 0.0;
    GLint num_points, dim;
    bool firstMouse = true;
    bool points_given = false;
    bool draw_gic = false;
    vector<Vertex> vertices;
    vector<int> pt_indices;
    vector<int> line_indices;
    vector<int> tri_indices;
    //vector<GLfloat> vertices_position;
    //vector<GLfloat> vertices_color;
    GLuint positionID, colorID, normalID;
    string filepath;
    float time = 0.0;

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
    void display(GLuint &vao);

    // Initialize the data to be rendered
    void initialize(GLuint &vao);

    // callback for errors
    void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

    // mouse callback
    void mouse_callback(GLFWwindow* window, double xpos, double ypos);

    // mouse callback
    void mouse_down_callback(GLFWwindow* window, double xpos, double ypos);

    static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

    // Error callback for logging errors
    static void error_callback(int error, const char* description);

    void do_movement();
    void draw_font(string text, void* font, GLint x, GLint y, glm::vec3 col);

    static void OnKeyDown(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void OnMouseDown(GLFWwindow* window, int button, int action, int mods);
    static void OnMouseMove(GLFWwindow* window, double x, double y);
    static void onMouseDrag(int x, int y);

    void MainLoop();

public:
    static Mouse mouse;
    static int keyCode;
    static glm::quat tmpRot;
    static glm::quat mRot;
    static GLint WIDTH, HEIGHT;
    static float view_scale;
    
    void DrawPoints(vector<vector<double>> pts);
    void DrawMortonCode(MortonCode &c);
    void DrawGIC(GIC &g);
    void ViewCollapses(vector<vector<double>> pts, vector<Collapse *> c);
};


#endif /* Viewer_hpp */
