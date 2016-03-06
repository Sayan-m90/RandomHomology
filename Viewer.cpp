//
//  Viewer.cpp
//  RandomHomology
//
//  Created by Bill Varcho on 2/28/16.
//  Copyright © 2016 Bill Varcho. All rights reserved.
//

#include "Viewer.hpp"

// Initialize static members (probably bad code should refactor)
int Viewer::keyCode = -1;
glm::quat Viewer::tmpRot = glm::quat(1,0,0,0);
glm::quat Viewer::mRot = glm::quat(1,0,0,0);
GLint Viewer::WIDTH = 800;
GLint Viewer::HEIGHT = 600;
Mouse Viewer::mouse = { .isDown = true, .lastX=0, .lastY=0,
                        .dx =0, .dy = 0, .x = 0, .y = 0 };

float Viewer::view_scale = 1.0;

void Viewer::MainLoop() {
    std::cout << "Drawing!" << std::endl;
    // Register a callback function for errors
    glfwSetErrorCallback(error_callback);
    // Initialize GLFW
    if ( !glfwInit()) {
        std::cerr << "Failed to initialize GLFW!" << std::endl;
        exit(-1);
    }
    
    // Open a window and attach an OpenGL rendering context to the window surface
    window = glfwCreateWindow(WIDTH, HEIGHT, "Data Viewer", NULL, NULL);
    if( !window ) {
        std::cerr << "Failed to open a window!" << std::endl;
        glfwTerminate();
        exit(-1);
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    // Use red to clear the screen
    glClearColor(.1, 0.1, 0.1, 1);

    // initialize a vertex array object
    GLuint vao;

    // Initialize the data to be rendered
    initialize(vao);

    // Create a rendering loop
    while(!glfwWindowShouldClose(window)) {
        display(vao);
        // Poll for events
        glfwPollEvents();
    }
    
    // Terminate GLFW
    glfwTerminate();
}

// Render scene
void Viewer::display(GLuint &vao) {
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(shaderProgram);
    
    GLfloat currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
    do_movement();
    
    if(draw_gic) {
        time += deltaTime;
        if (time > 1.0) {
            time = time - 1.0;
            Vertex::updateIndices();
            cout << Vertex::POS_INDEX << endl;
            cout << Vertex::NEXT_POS_INDEX << endl;
        }
        GLint timeLocation = glGetUniformLocation(shaderProgram, "t");
        GLfloat mod_time = fmodf(currentFrame, 1.0f);
        glUniform1f(timeLocation, mod_time);
    }
        
    // TODO(me): delete
    GLint vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
    glUniform4f(vertexColorLocation, 0.0f, 1.0f, 1.0f, 1.0f);

    glm::vec3 front;
    front.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
    front.y = sin(glm::radians(pitch));
    front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
    cam_front = glm::normalize(front);
    
    
    GLint modelLoc = glGetUniformLocation(shaderProgram, "model");
    GLint viewLoc = glGetUniformLocation(shaderProgram, "view");
    GLint projectionLoc = glGetUniformLocation(shaderProgram, "projection");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

    glBindVertexArray(vao);
    
    if (draw_gic) {
        
        // TODO (me): Dedup code
        // Create a Vector Buffer Object that will store the vertices on video memory
        GLuint vbo;
        glGenBuffers(1, &vbo);
        // Allocate space and upload the data from CPU to GPU
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex),
                     &vertices.front(), GL_STATIC_DRAW);
        // Get the location of the attributes that enters in the vertex shader
        GLuint next_positionID;
        positionID = glGetAttribLocation(shaderProgram, "position");
        next_positionID = glGetAttribLocation(shaderProgram, "next_position");
        colorID = glGetAttribLocation(shaderProgram,"color");
        normalID = glGetAttribLocation(shaderProgram,"normal");
        glEnableVertexAttribArray(next_positionID);
        glEnableVertexAttribArray(positionID);
        glEnableVertexAttribArray(colorID);
        glEnableVertexAttribArray(normalID);
        
        glVertexAttribPointer( colorID, 4, GL_FLOAT, GL_FALSE,
                              sizeof(Vertex),(void *)Vertex::offsetColor());
        glVertexAttribPointer( normalID, 3, GL_FLOAT, GL_FALSE,
                              sizeof(Vertex),(void *)Vertex::offsetNormal());
        
//        glVertexAttribPointer( positionID, 3, GL_FLOAT, GL_FALSE,
//                              sizeof(Vertex), 0 );
//        glVertexAttribPointer( next_positionID, 3, GL_FLOAT, GL_FALSE,
//                              sizeof(Vertex), (void *)Vertex::offsetNextPos());

        glVertexAttribPointer( positionID, 3, GL_FLOAT, GL_FALSE,
                              sizeof(Vertex), (void *)Vertex::offset_POS() );
        glVertexAttribPointer( next_positionID, 3, GL_FLOAT, GL_FALSE,
                              sizeof(Vertex), (void *)Vertex::offset_NEXT_POS());
        
        
        
        GLuint elementID_points, elementID_lines, elementID_tris;
        
        // Draw VERTS
        glGenBuffers(1, &elementID_points);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementID_points);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, pt_indices.size()*sizeof(GLuint),
                     &pt_indices.front(), GL_STATIC_DRAW);
        glDrawElements(GL_POINTS, pt_indices.size(), GL_UNSIGNED_INT, 0);

        // Draw EDGES
        glGenBuffers(1, &elementID_lines);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementID_lines);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, line_indices.size()*sizeof(GLuint),
                     &line_indices.front(), GL_STATIC_DRAW);
        glDrawElements(GL_LINES, line_indices.size(), GL_UNSIGNED_INT, 0);

        // Draw TRIS
        glGenBuffers(1, &elementID_tris);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementID_tris);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, tri_indices.size()*sizeof(GLuint),
                     &tri_indices.front(), GL_STATIC_DRAW);

        glDrawElements(GL_TRIANGLES, tri_indices.size(), GL_UNSIGNED_INT, 0);
        
        // DELETE THE BUFFERS TO
        glDeleteBuffers(1, &elementID_points);
        glDeleteBuffers(1, &elementID_lines);
        glDeleteBuffers(1, &elementID_tris);
        glDeleteBuffers(1, &vbo);
        
    } else {
        glDrawArrays(mode, 0, num_points);
        glDrawArrays(GL_POINTS, 0, num_points);
    }
    
    // Honestly I don't think I need to do this since I'm deleteing the
    // element_array buffers above
    glBindVertexArray(0);
    glBindBuffer( GL_ARRAY_BUFFER, 0);
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0);
    
    /*-------------------------*
     | Text Rendering
     *-------------------------*/
    
    glUseProgram(0);
    glColor3f(0.4, 0.4, 0.4);
    std::string s = "Homology";
    glWindowPos2i( 20, 2*HEIGHT - 40 );  // move in 10 pixels from the left and bottom edges
    for ( int i = 0; i < s.length(); ++i ) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, s[i]);
    }
    
    // draw filepath
    draw_font(filepath, GLUT_BITMAP_HELVETICA_18, 2 * WIDTH - 400,
              2 * HEIGHT - 20, glm::vec3(0.4, 0.4, 0.4));
    
    s = "X:";
    glColor3f(0.6, 0.0, 0.0);
    glWindowPos2i( 20, 100 );  // move in 10 pixels from the left and bottom edges
    for ( int i = 0; i < s.length(); ++i ) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, s[i]);
    }
    s = std::to_string((float)model[0][0]) ;
    glColor3f(0.8, 0.0, 0.0);
    glWindowPos2i( 50, 100 );  // move in 10 pixels from the left and bottom edges
    for ( int i = 0; i < s.length(); ++i ) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, s[i]);
    }
    glColor3f(0.0, 0.6, 0.0);
    s = "Y:";
    glWindowPos2i( 20, 66 );  // move in 10 pixels from the left and bottom edges
    for ( int i = 0; i < s.length(); ++i ) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, s[i]);
    }
    s = std::to_string((float)model[1][1]);
    glColor3f(0.0, 0.8, 0.0);
    glWindowPos2i( 50, 66 );  // move in 10 pixels from the left and bottom edges
    for ( int i = 0; i < s.length(); ++i ) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, s[i]);
    }
    s = "Z:";
    draw_font(s, GLUT_BITMAP_HELVETICA_18, 20, 33, glm::vec3(0.0, 0.0, 0.6));
    s = std::to_string((float)model[2][2]);
    draw_font(s, GLUT_BITMAP_HELVETICA_18, 50, 33, glm::vec3(0.0, 0.0, 0.8));
    
    glUseProgram(shaderProgram);

    // Swap front and back buffers
    glfwSwapBuffers(window);
}

void Viewer::draw_font(string text, void* font,
                       GLint x, GLint y, glm::vec3 col) {
    glColor3f(col.x, col.y, col.z);
    glWindowPos2i(x, y);  // move in 10 pixels from the left and bottom edges
    for ( int i = 0; i < text.length(); ++i ) {
        glutBitmapCharacter(font, text[i]);
    }
}

void Viewer::initialize(GLuint &vao) {
    // Use a Vertex Array Object
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glEnable(GL_PROGRAM_POINT_SIZE);
    glEnable(GL_DEPTH_TEST);

    // TODO(me): This just here for debugging, can delete later
    // create random points if no vertices specified
    if (vertices.size() == 0) {
        dim = 3;
        num_points = 100000;
        for (int j = 0; j<num_points; j++) {
            Vertex v;
            v.color = glm::vec4(1.);
            v.position = glm::vec4(1.);
            for (int i = 0; i < dim ; i++) {
                v.position[i] = 1.0f * (1.0f - 2. *(float)rand() / (float)INT_MAX);
            }
            vertices.push_back(v);
        }
    }

    // Create a Vector Buffer Object that will store the vertices on video memory
    GLuint vbo;
    glGenBuffers(1, &vbo);
    // Allocate space and upload the data from CPU to GPU
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex),
                 &vertices.front(), GL_STATIC_DRAW);

    GLuint shaderProgram;
    if (draw_gic) {
        shaderProgram = create_program("/Users/billvarcho/Documents/Xcode/BitTests/BitTests/vertexGIC.shader",
                                       "/Users/billvarcho/Documents/Xcode/BitTests/BitTests/fragmentGIC.shader");
    } else {
        shaderProgram = create_program("/Users/billvarcho/Documents/Xcode/BitTests/BitTests/vertex.shader",
                                        "/Users/billvarcho/Documents/Xcode/BitTests/BitTests/fragment.shader");
    }
    
    // Get the location of the attributes that enters in the vertex shader
    positionID = glGetAttribLocation(shaderProgram, "position");
    colorID = glGetAttribLocation(shaderProgram,"color");
    normalID = glGetAttribLocation(shaderProgram,"normal");
    
    glEnableVertexAttribArray(positionID);
    glEnableVertexAttribArray(colorID);
    glEnableVertexAttribArray(normalID);
    glVertexAttribPointer( positionID, 3, GL_FLOAT, GL_FALSE,
                          sizeof(Vertex), 0 );
    glVertexAttribPointer( colorID, 4, GL_FLOAT, GL_FALSE,
                          sizeof(Vertex),(void *)Vertex::offsetColor());
    glVertexAttribPointer( normalID, 3, GL_FLOAT, GL_FALSE,
                          sizeof(Vertex),(void *)Vertex::offsetNormal());

    glfwSetKeyCallback(window, OnKeyDown);
    glfwSetCursorPosCallback(window, OnMouseMove);
    glfwSetMouseButtonCallback(window, OnMouseDown);
    glfwSetWindowSizeCallback(window, window_resized);
    glfwSetScrollCallback(window, scroll_callback);
    
    // Initialize 3D viewport model, view, and projection mats
    projection = glm::perspective(45.0f, 1.0f , 0.01f, 100.0f);
    model = glm::rotate(model, -55.0f, glm::vec3(1.0f, 0.0f, 0.0f));
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

    cam_pos = glm::vec3(0.0f, 0.0f,  3.0f);
    cam_front = glm::vec3(0.0f, 0.0f, -1.0f);
    cam_up = glm::vec3(0.0f, 1.0f,  0.0f);
}

// Called when the window is resized
void Viewer::window_resized(GLFWwindow* window, int width, int height) {
    WIDTH = width;
    HEIGHT = height;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glfwSwapBuffers(window);
}

// Called for keyboard events
void Viewer::keyboard(int key, int action) {
    if(key == 'Q' && action == GLFW_PRESS) {
        glfwTerminate();
        exit(0);
    }
}

// Read a shader source from a file
// store the shader source in a std::vector<char>
void Viewer::read_shader_src(const char *fname, std::vector<char> &buffer) {
    std::ifstream in;
    in.open(fname, std::ios::binary);

    if(in.is_open()) {
        // Get the number of bytes stored in this file
        in.seekg(0, std::ios::end);
        size_t length = (size_t)in.tellg();

        // Go to start of the file
        in.seekg(0, std::ios::beg);

        // Read the content of the file in a buffer
        buffer.resize(length + 1);
        in.read(&buffer[0], length);
        in.close();
        // Add a valid C - string end
        buffer[length] = '\0';
    }
    else {
        std::cerr << "Unable to open " << fname << " I'm out!" << std::endl;
        exit(-1);
    }
}

// Compile a shader
GLuint Viewer::load_and_compile_shader(const char *fname, GLenum shaderType) {
    // Load a shader from an external file
    std::vector<char> buffer;
    read_shader_src(fname, buffer);
    const char *src = &buffer[0];

    // Compile the shader
    GLuint shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &src, NULL);
    glCompileShader(shader);
    // Check the result of the compilation
    GLint test;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &test);
    if(!test) {
        std::cerr << "Shader compilation failed with this message:" << std::endl;
        std::vector<char> compilation_log(512);
        glGetShaderInfoLog(shader, compilation_log.size(), NULL, &compilation_log[0]);
        std::cerr << &compilation_log[0] << std::endl;
        glfwTerminate();
        exit(-1);
    }
    return shader;
}

// Create a program from two shaders
GLuint Viewer::create_program(const char *path_vert_shader, const char *path_frag_shader) {
    // Load and compile the vertex and fragment shaders
    GLuint vertexShader = load_and_compile_shader(path_vert_shader, GL_VERTEX_SHADER);
    GLuint fragmentShader = load_and_compile_shader(path_frag_shader, GL_FRAGMENT_SHADER);
    
    // Attach the above shader to a program
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);

    // Flag the shaders for deletion
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Link and use the program
    glLinkProgram(shaderProgram);
    glUseProgram(shaderProgram);

    return shaderProgram;
}

void Viewer::error_callback(int error, const char* description) {
    fputs(description, stderr);
}

void Viewer::do_movement()
{
    // Camera controls
    GLfloat cameraSpeed = 5.0f * deltaTime;
    if (keys[GLFW_KEY_W])
        cam_pos += cameraSpeed * cam_front;
    if (keys[GLFW_KEY_S])
        cam_pos -= cameraSpeed * cam_front;
    if (keys[GLFW_KEY_A])
        cam_pos -= glm::normalize(glm::cross(cam_front, cam_up)) * cameraSpeed;
    if (keys[GLFW_KEY_D])
        cam_pos += glm::normalize(glm::cross(cam_front, cam_up)) * cameraSpeed;

    //cout << view_scale << endl;
    glm::mat4 scale = glm::scale( glm::mat4(), glm::vec3(1./ (view_scale+.1)));
    glm::mat4 rotate = glm::toMat4(mRot);
    glm::mat4 translate = glm::translate( glm::mat4(), glm::vec3(0,0,0));
    model = translate * rotate * scale;
    view = glm::lookAt(glm::vec3(0,0,10),glm::vec3(0,0,1),glm::vec3(0,1,0));
    projection = glm::perspective(45.0f, (float)WIDTH/(float)HEIGHT , 0.1f, 100.0f);
}

void Viewer::scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    float v_old = view_scale;
    view_scale += .1 * yoffset;
    view_scale = fmax(0.0001, view_scale);
}

void Viewer::OnKeyDown(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if(action==GLFW_PRESS) {
        keyCode=key;
    }
    if(action==GLFW_RELEASE) {
        keyCode=-1;
    }
}

void Viewer::OnMouseDown(GLFWwindow* window, int button, int action, int mods) {
    //std::cout << "Mouse Down" << std::endl;
    if (action==GLFW_PRESS) {
        mouse.isDown=true;
        double x,y;
        glfwGetCursorPos(window, &x, &y);
        mouse.lastX = x;
        mouse.lastY = y;
        tmpRot = mRot;
    }
    else{
        mouse.isDown = false;
    }
}

void Viewer::onMouseDrag(int x, int y) {
    mouse.dx = 100 * (mouse.x - mouse.lastX);
    mouse.dy = 100 * (mouse.y - mouse.lastY);

    float wx = (float)mouse.dx / (float) WIDTH;
    float wy = (float)mouse.dy / (float) HEIGHT;
    glm::vec3 axis = glm::cross( glm::vec3(0,0,1), glm::vec3(wx,-wy,0) );
    glm::quat q = glm::angleAxis( glm::length(axis), glm::normalize(axis) );
    mRot= q*tmpRot;
}

void Viewer::OnMouseMove(GLFWwindow* window, double x, double y) {
    mouse.x = x;
    mouse.y = y;
    if (mouse.isDown) onMouseDrag(x,y);
}

void Viewer::DrawPoints(vector<vector<double>> pts) {
    num_points = pts.size();
    points_given = true;
    for (int i = 0; i < pts.size(); i++) {
        Vertex v;
        v.position = glm::vec4(pts[i][0], pts[i][1], pts[i][2], 0);
        float t = (float) i/ (float) pts.size();
        v.color = glm::vec4(t, t, t, 1.);
        //for (int j = 0; j < pts[i].size(); j++) {
        //    //vertices_position.push_back((GLfloat) pts[i][j]);
        //}
        vertices.push_back(v);
    }
    MainLoop();
}

void Viewer::DrawMortonCode(MortonCode &mc) {
    // set draw lines
    mode = GL_LINE_STRIP;
    vector<vector<double>> pts;
    while (!mc.empty()) {
        pts.push_back(mc.next());
    }
    DrawPoints(pts);
}

void Viewer::DrawGIC(GIC &g) {
    filepath = g.fp;
    for (int i = 0; i < g.pts.size(); i++) {
        Vertex v;
        vector<double> pt = g.pts[i];
        //cout << pt[0] << '\t' << pt[1] << '\t' << pt[2] << '\t' << endl;
        v.position = glm::vec4(pt[0],
                               pt[1],
                               pt[2],
                               0.0);
        v.color = glm::vec4(1., 0.0, 0.0, .3);
        v.next_position = glm::vec4((float)rand()/(float)RAND_MAX,
                                    (float)rand()/(float)RAND_MAX,
                                    (float)rand()/(float)RAND_MAX,
                                    (float)rand()/(float)RAND_MAX);
        
        for (int i = 0; i < 10; i++) {
            if (i == 0) {
                v.positions[i] = glm::vec4(pt[0],
                                           pt[1],
                                           pt[2],
                                           0.0);
            } else {
                v.positions[i] = glm::vec4(1.0 * (float)rand()/(float)RAND_MAX - 0.5 + pt[0],
                                           1.0 * (float)rand()/(float)RAND_MAX - 0.5 + pt[1],
                                           1.0 * (float)rand()/(float)RAND_MAX - 0.5 + pt[2],
                                           1.0 * (float)rand()/(float)RAND_MAX);
            }
        }
        
        vertices.push_back(v);
    }
    
    pt_indices = g.indices[0];
    line_indices = g.indices[1];
    tri_indices = g.indices[2];
    num_points = (GLint) g.pts.size();

    points_given = true;
    draw_gic = true;
    
    MainLoop();
 }

//void Viewer::ViewCollapses(vector<vector<double>> pts, vector<Collapse *> c) {
//    filepath = "TODO";
//    for (int i = 0; i < pts.size(); i++) {
//        Vertex v;
//        vector<double> pt = pts[i];
//        //cout << pt[0] << '\t' << pt[1] << '\t' << pt[2] << '\t' << endl;
//        v.position = glm::vec4(pt[0],
//                               pt[1],
//                               pt[2],
//                               0.0);
//        v.color = glm::vec4(1., 0.0, 0.0, .3);
//        v.next_position = glm::vec4((float)rand()/(float)RAND_MAX,
//                                    (float)rand()/(float)RAND_MAX,
//                                    (float)rand()/(float)RAND_MAX,
//                                    (float)rand()/(float)RAND_MAX);
//        
//        for (int i = 0; i < 10; i++) {
//            if (i == 0) {
//                v.positions[i] = glm::vec4(pt[0],
//                                           pt[1],
//                                           pt[2],
//                                           0.0);
//            } else {
//                v.positions[i] = glm::vec4(1.0 * (float)rand()/(float)RAND_MAX - 0.5 + pt[0],
//                                           1.0 * (float)rand()/(float)RAND_MAX - 0.5 + pt[1],
//                                           1.0 * (float)rand()/(float)RAND_MAX - 0.5 + pt[2],
//                                           1.0 * (float)rand()/(float)RAND_MAX);
//            }
//        }
//        
//        vertices.push_back(v);
//    }
//    
//    pt_indices = g.indices[0];
//    line_indices = g.indices[1];
//    tri_indices = g.indices[2];
//    num_points = (GLint) g.pts.size();
//    
//    points_given = true;
//    draw_gic = true;
//    MainLoop();
//}
