//
//  Viewer.cpp
//  RandomHomology
//
//  Created by Bill Varcho on 2/28/16.
//  Copyright © 2016 Bill Varcho. All rights reserved.
//

#include "Viewer.hpp"

Viewer::Viewer() {
    tmpRot = glm::quat(1,0,0,0);
    mRot = glm::quat(1,0,0,0);
    keyCode = -1;
    WIDTH = 800;
    HEIGHT = 600;
    mouse = { .is_down = true, .last_X=0, .last_Y=0,
        .dx =0, .dy = 0, .x = 0, .y = 0 };
    view_scale = 1.0;
}

void Viewer::DisplayPoint(GLuint &vao) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(shaderProgram);
    
    GLfloat currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
    do_movement();
    
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
    
    glDrawArrays(mode, 0, num_points);
    glDrawArrays(GL_POINTS, 0, num_points);
    
    // Honestly I don't think I need to do this since I'm deleteing the
    // element_array buffers above
    glBindVertexArray(0);
    glBindBuffer( GL_ARRAY_BUFFER, 0);
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0);
    
    DrawText();
    
    // Swap front and back buffers
    glfwSwapBuffers(window);
}

void Viewer::DisplayCollapses(GLuint &vao) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(shaderProgram);
    glLineWidth(1.0);
    
    GLfloat currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
    do_movement();
    
    time += 5.0 * deltaTime;
    if (time > 1.0) {
        time = time - 1.0;
        Vertex::updateIndices();
    }
    
    GLint timeLocation = glGetUniformLocation(shaderProgram, "t");
    GLfloat mod_time = fmodf(currentFrame, 1.0f);
    glUniform1f(timeLocation, mod_time);
    
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
    position_ID = glGetAttribLocation(shaderProgram, "position");
    next_positionID = glGetAttribLocation(shaderProgram, "next_position");
    color_ID = glGetAttribLocation(shaderProgram,"color");
    normal_ID = glGetAttribLocation(shaderProgram,"normal");
    glEnableVertexAttribArray(next_positionID);
    glEnableVertexAttribArray(position_ID);
    glEnableVertexAttribArray(color_ID);
    glEnableVertexAttribArray(normal_ID);
    
    glVertexAttribPointer( color_ID, 4, GL_FLOAT, GL_FALSE,
                          sizeof(Vertex),(void *)Vertex::offsetColor());
    
    glVertexAttribPointer( normal_ID, 3, GL_FLOAT, GL_FALSE,
                          sizeof(Vertex),(void *)Vertex::offsetNormal());
    
    glVertexAttribPointer( position_ID, 3, GL_FLOAT, GL_FALSE,
                          sizeof(Vertex), (void *)Vertex::offset_POS() );
    glVertexAttribPointer( next_positionID, 3, GL_FLOAT, GL_FALSE,
                          sizeof(Vertex), (void *)Vertex::offset_NEXT_POS());
    
    GLuint elementID_points, elementID_lines, elementID_tris;
    
    // Draw TRIS
    // Below used for transparent drawing
//    glEnable(GL_CULL_FACE);
//    glDisable(GL_CULL_FACE);
//    glEnable(GL_BLEND);
//    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glGenBuffers(1, &elementID_tris);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementID_tris);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, tri_indices.size()*sizeof(GLuint),
                 &tri_indices.front(), GL_STATIC_DRAW);
    glDrawElements(GL_TRIANGLES, tri_indices.size(), GL_UNSIGNED_INT, 0);

    
    glVertexAttribPointer( color_ID, 4, GL_FLOAT, GL_FALSE,
                          sizeof(Vertex),(void *)Vertex::offsetColorOutline());
    
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
    
    
    // DELETE THE BUFFERS TO
    glDeleteBuffers(1, &elementID_points);
    glDeleteBuffers(1, &elementID_lines);
    glDeleteBuffers(1, &elementID_tris);
    glDeleteBuffers(1, &vbo);
    
    // Honestly I don't think I need to do this since I'm deleteing the
    // element_array buffers above
    glBindVertexArray(0);
    glBindBuffer( GL_ARRAY_BUFFER, 0);
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0);
    
    DrawText();
    
    // Swap front and back buffers
    glfwSwapBuffers(window);
}

void Viewer::DisplayGIC(GLuint &vao) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(shaderProgram);
    
    GLfloat currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
    do_movement();
    
    time += deltaTime;
    if (time > 1.0) {
        time = time - 1.0;
    }
    
    GLint timeLocation = glGetUniformLocation(shaderProgram, "t");
    GLfloat mod_time = fmodf(currentFrame, 1.0f);
    glUniform1f(timeLocation, mod_time);

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
    position_ID = glGetAttribLocation(shaderProgram, "position");
    next_positionID = glGetAttribLocation(shaderProgram, "next_position");
    color_ID = glGetAttribLocation(shaderProgram,"color");
    normal_ID = glGetAttribLocation(shaderProgram,"normal");
    glEnableVertexAttribArray(next_positionID);
    glEnableVertexAttribArray(position_ID);
    glEnableVertexAttribArray(color_ID);
    glEnableVertexAttribArray(normal_ID);
    
    glVertexAttribPointer( color_ID, 4, GL_FLOAT, GL_FALSE,
                          sizeof(Vertex),(void *)Vertex::offsetColor());
    
    glVertexAttribPointer( normal_ID, 3, GL_FLOAT, GL_FALSE,
                          sizeof(Vertex),(void *)Vertex::offsetNormal());
    
    glVertexAttribPointer( position_ID, 3, GL_FLOAT, GL_FALSE,
                          sizeof(Vertex), (void *)Vertex::offset_POS() );
    glVertexAttribPointer( next_positionID, 3, GL_FLOAT, GL_FALSE,
                          sizeof(Vertex), (void *)Vertex::offset_NEXT_POS());
    
    GLuint elementID_points, elementID_lines, elementID_tris;
    
    // Draw TRIS
    // Below used for transparent drawing
    //    glEnable(GL_CULL_FACE);
    //    glDisable(GL_CULL_FACE);
    //    glEnable(GL_BLEND);
    //    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glGenBuffers(1, &elementID_tris);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementID_tris);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, tri_indices.size()*sizeof(GLuint),
                 &tri_indices.front(), GL_STATIC_DRAW);
    glDrawElements(GL_TRIANGLES, tri_indices.size(), GL_UNSIGNED_INT, 0);
    
    
    glVertexAttribPointer( color_ID, 4, GL_FLOAT, GL_FALSE,
                          sizeof(Vertex),(void *)Vertex::offsetColorOutline());
    
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
    
    
    // DELETE THE BUFFERS TO
    glDeleteBuffers(1, &elementID_points);
    glDeleteBuffers(1, &elementID_lines);
    glDeleteBuffers(1, &elementID_tris);
    glDeleteBuffers(1, &vbo);
    
    // Honestly I don't think I need to do this since I'm deleteing the
    // element_array buffers above
    glBindVertexArray(0);
    glBindBuffer( GL_ARRAY_BUFFER, 0);
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0);
    
    DrawText();
    
    // Swap front and back buffers
    glfwSwapBuffers(window);
}

void Viewer::DrawText() {
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
}

void Viewer::MainLoop(DRAW_LOOP type) {
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

    // Use grey to clear the screen
    glClearColor(.1, 0.1, 0.1, 1);

    // initialize a vertex array object
    GLuint vao;

    // Initialize the data to be rendered
    initialize(vao);

    // Create a rendering loop
    while(!glfwWindowShouldClose(window)) {
        switch (type) {
            case POINT_LOOP:
                DisplayPoint(vao);
                break;
            case GIC_LOOP:
                DisplayGIC(vao);
                break;
            case COLLAPSE_LOOP:
                DisplayCollapses(vao);
                break;
            default:
                DisplayPoint(vao);
                break;
        }
        
        // Poll for events
        glfwPollEvents();
    }
    
    // Terminate GLFW
    glfwTerminate();
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
        shaderProgram = create_program(Constants::VERTEX_SHADER_GIC,
                                       Constants::FRAGMENT_SHADER_GIC);
    } else {
        shaderProgram = create_program(Constants::VERTEX_SHADER,
                                       Constants::FRAGMENT_SHADER);
    }
    
    // Get the location of the attributes that enters in the vertex shader
    position_ID = glGetAttribLocation(shaderProgram, "position");
    color_ID = glGetAttribLocation(shaderProgram,"color");
    normal_ID = glGetAttribLocation(shaderProgram,"normal");
    
    glEnableVertexAttribArray(position_ID);
    glEnableVertexAttribArray(color_ID);
    glEnableVertexAttribArray(normal_ID);
    glVertexAttribPointer( position_ID, 3, GL_FLOAT, GL_FALSE,
                          sizeof(Vertex), 0 );
    glVertexAttribPointer( color_ID, 4, GL_FLOAT, GL_FALSE,
                          sizeof(Vertex),(void *)Vertex::offsetColor());
    glVertexAttribPointer( normal_ID, 3, GL_FLOAT, GL_FALSE,
                          sizeof(Vertex),(void *)Vertex::offsetNormal());

    
    glfwSetWindowUserPointer(window, this);
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_move_callback);
    glfwSetMouseButtonCallback(window, mouse_down_callback);
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
    Viewer *v = static_cast<Viewer *>(glfwGetWindowUserPointer(window));
    v->WIDTH = width;
    v->HEIGHT = height;
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
        glGetShaderInfoLog(shader, compilation_log.size(),
                           NULL, &compilation_log[0]);
        std::cerr << &compilation_log[0] << std::endl;
        glfwTerminate();
        exit(-1);
    }
    return shader;
}

// Create a program from two shaders
GLuint Viewer::create_program(const char *path_vert_shader,
                              const char *path_frag_shader) {
    // Load and compile the vertex and fragment shaders
    GLuint vertexShader = load_and_compile_shader(path_vert_shader,
                                                  GL_VERTEX_SHADER);
    GLuint fragmentShader = load_and_compile_shader(path_frag_shader,
                                                    GL_FRAGMENT_SHADER);
    
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
    Viewer *v = static_cast<Viewer *>(glfwGetWindowUserPointer(window));
    float v_old = v->view_scale;
    v->view_scale += .1 * yoffset;
    v->view_scale = fmax(0.0001, v->view_scale);
}

void Viewer::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    Viewer *v = static_cast<Viewer *>(glfwGetWindowUserPointer(window));
    if(action==GLFW_PRESS) {
        v->keyCode = key;
    }
    if(action==GLFW_RELEASE) {
        v->keyCode = -1;
    }
}

void Viewer::mouse_down_callback(GLFWwindow* window, int button, int action, int mods) {
    Viewer *v = static_cast<Viewer *>(glfwGetWindowUserPointer(window));
    if (action==GLFW_PRESS) {
        v->mouse.is_down=true;
        double x,y;
        glfwGetCursorPos(window, &x, &y);
        v->mouse.last_X = x;
        v->mouse.last_Y = y;
        v->tmpRot = v->mRot;
    }
    else{
        v->mouse.is_down = false;
    }
}

void Viewer::mouse_drag_callback(GLFWwindow* window, int x, int y) {
    Viewer *v = static_cast<Viewer *>(glfwGetWindowUserPointer(window));
    v->mouse.dx = Constants::MOUSE_SPEED * (v->mouse.x - v->mouse.last_X);
    v->mouse.dy = Constants::MOUSE_SPEED * (v->mouse.y - v->mouse.last_Y);
    float wx = (float) v->mouse.dx / (float) v->WIDTH;
    float wy = (float) v->mouse.dy / (float) v->HEIGHT;
    glm::vec3 axis = glm::cross( glm::vec3(0, 0, 1), glm::vec3(wx, -wy, 0) );
    glm::quat q = glm::angleAxis( glm::length(axis), glm::normalize(axis) );
    v->mRot = q * v->tmpRot;
}

void Viewer::mouse_move_callback(GLFWwindow* window, double x, double y) {
    Viewer *v = static_cast<Viewer *>(glfwGetWindowUserPointer(window));
    v->mouse.x = x;
    v->mouse.y = y;
    if (v->mouse.is_down) mouse_drag_callback(window, x,y);
}

void Viewer::DrawPoints(vector<vector<double>> pts) {
    vertices.clear();
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
    
    MainLoop(POINT_LOOP);
}

void Viewer::DrawMortonCode(MortonCode &mc) {
    vertices.clear();
    // set draw lines
    mode = GL_LINE_STRIP;
    vector<vector<double>> pts;
    while (!mc.empty()) {
        pts.push_back(mc.next());
    }
    DrawPoints(pts);
}

void Viewer::DrawGIC(GIC &g) {
    vertices.clear();
    filepath = g.fp;
    for (int i = 0; i < g.pts.size(); i++) {
        Vertex v;
        vector<double> pt = g.pts[i];
        v.position = glm::vec4(pt[0],
                               pt[1],
                               pt[2],
                               0.0);
        v.next_position = glm::vec4(pt[0],
                                    pt[1],
                                    pt[2],
                                    0.0);
        for (int i = 0; i < 2; i++) {
            v.positions[i] = glm::vec4(pt[0],
                                       pt[1],
                                       pt[2],
                                       0.0);
        }
        
        v.color = glm::vec4(.35, .56, 1.0, 1.0);
        v.outline_color = .6f * v.color;
        vertices.push_back(v);
    }    
    pt_indices = g.indices[0];
    line_indices = g.indices[1];
    tri_indices = g.indices[2];
    num_points = (GLint) g.pts.size();
    points_given = true;
    draw_gic = true;
    draw_type = 1;
    MainLoop(GIC_LOOP);
}


/*---------------------------------------------*
 | Helper function(s) for Displaying Collapses
 *---------------------------------------------*/
void collapse(int start, int end, vector<vector<int>> &v_map) {
    vector<int> s = v_map[start];
    vector<int> empty;
    v_map[start] = empty;
    v_map[end].insert(v_map[end].end(), s.begin(), s.end());
}

void Viewer::ViewCollapses(GIC &g, vector<Operation *> collapses) {
    vertices.clear();
    filepath = g.fp;
    for (int i = 0; i < g.pts.size(); i++) {
        Vertex v;
        vector<double> pt = g.pts[i];
        v.position = glm::vec4(pt[0],
                               pt[1],
                               pt[2],
                               0.0);
        v.color = glm::vec4(.91, .0, .15, 1.0);
        v.outline_color = .6f * v.color;
        vertices.push_back(v);
    }
    
    int time_index = 0;
    int collapse_index = 0;
    vector<vector<int>> vertex_map;
    while (time_index < Constants::MAX_NUM_COLLAPSES) {
        while (collapses[collapse_index]->Type() != TIME_OP){
            if (collapses[collapse_index]->Type() == COLLAPSE_OP) {
                Collapse *c = (Collapse *) collapses[collapse_index];
                collapse(c->v_start, c->v_target, vertex_map);
            } else if (collapses[collapse_index]->Type() == INSERT_OP) {
                vector<int> q;
                Insert *i = (Insert *) collapses[collapse_index];
                if (i->v_id.size() == 1) {
                    q.push_back(i->v_id[0]);
                    vertex_map.push_back(q);
                }
            }
            collapse_index++;
        };
        collapse_index++;
        // loop over vertex map setting positions of vertices
        for (int j = 0; j < vertex_map.size(); j++) {
            vector<int> pq = vertex_map[j];
            for (int i = 0; i < pq.size(); i++) {
                vertices[pq[i]].positions[time_index] = vertices[pq[0]].position;
            }
        }
        time_index++;
    }

    pt_indices = g.indices[0];
    line_indices = g.indices[1];
    tri_indices = g.indices[2];
    num_points = (GLint) g.pts.size();
    points_given = true;
    draw_gic = true;
    draw_type = 1;
    MainLoop(COLLAPSE_LOOP);
}


// TODO (me): implement
void ViewBarcode(vector<Barcode> &bc) {

}
