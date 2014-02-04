//
//  main.cpp
//  osxtest
//
//  Created by Alexander on 16/01/14.
//  Copyright (c) 2014 Alexander. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include <SDL2/SDL.h>
#include <OpenGL/gl.h>
#define GLM_FORCE_RADIANS
#include "glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

using namespace std;

int main(int argc, const char * argv[])
{
    SDL_Init(SDL_INIT_VIDEO); // Init SDL2
    
    // Create a window. Window mode MUST include SDL_WINDOW_OPENGL for use with OpenGL.
    SDL_Window *window = SDL_CreateWindow(
                                          "SDL2/OpenGL Demo", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1280, 720, SDL_WINDOW_OPENGL|SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);


    int w, h, pw, ph;
    SDL_GetWindowSize(window, &w, &h);
    SDL_GL_GetDrawableSize(window, &pw, &ph);
    
    printf("winodwsize: %ix%i (actual: %ix%i)\n", w, h, pw, ph);
    

    // Create an OpenGL context associated with the window.
    SDL_GLContext glcontext = SDL_GL_CreateContext(window);
    
    //std::printf("\"%s\" with glsl version \"%s\" rendering on \"%s\"\n", glGetString(GL_VERSION), glGetString(GL_SHADING_LANGUAGE_VERSION), glGetString(GL_RENDERER));
    
    int shaderGLPtr = 0;
    int vertexShaderGLPtr = 0, fragmentShaderGLPtr = 0;
    fragmentShaderGLPtr = glCreateShader(GL_FRAGMENT_SHADER);
    vertexShaderGLPtr = glCreateShader(GL_VERTEX_SHADER);
    shaderGLPtr = glCreateProgram();
    
    
    
    char * msgbuffer = new char[1024];
    int rlength = 0;
    
    {
        std::ifstream file("resources/test.frag", std::ios::binary);
        if(file.good() == false) {
            cout << "couldn't open file" << endl;
            exit(-1);
        }
        file.seekg(0, std::ios::end);
        std::streamsize size = file.tellg();
        file.seekg(0, std::ios::beg);
        
        vector<char> buffer(size);
        if (file.read((char *)buffer.data(), size))
        {
            cout << "read operation succeeded" << endl;
        }
        buffer.push_back('\0');
        
        const char *c_str = buffer.data();
        glShaderSource(fragmentShaderGLPtr, 1, &c_str, NULL);
    }
    
    glCompileShader(fragmentShaderGLPtr);
    glGetShaderInfoLog(fragmentShaderGLPtr, 1024, &rlength, msgbuffer);
    cout << msgbuffer << endl;
    
    {
        std::ifstream file("resources/test.vert", std::ios::binary);
        if(file.good() == false) {
            cout << "couldn't open file" << endl;
            exit(-1);
        }
        file.seekg(0, std::ios::end);
        std::streamsize size = file.tellg();
        file.seekg(0, std::ios::beg);
        
        vector<char> buffer(size);
        if (file.read((char *)buffer.data(), size))
        {
            cout << "read operation succeeded" << endl;
        }
        buffer.push_back('\0');
        
        const char *c_str = buffer.data();
        glShaderSource(vertexShaderGLPtr, 1, &c_str, NULL);
    }

    glCompileShader(vertexShaderGLPtr);
    glGetShaderInfoLog(vertexShaderGLPtr, 1024, &rlength, msgbuffer);
    cout << msgbuffer << endl;

    
    glAttachShader(shaderGLPtr, vertexShaderGLPtr);
    glAttachShader(shaderGLPtr, fragmentShaderGLPtr);
    glLinkProgram(shaderGLPtr);
    
    glGetProgramInfoLog(shaderGLPtr, 1024, &rlength, msgbuffer);
    cout << msgbuffer << endl;
    
    glValidateProgram(shaderGLPtr);
    glGetProgramInfoLog(shaderGLPtr, 1024, &rlength, msgbuffer);
    cout << msgbuffer << endl;
    
    glm::mat4 Projection = glm::perspectiveFov(90.0f, (float)pw, (float)ph, 1.0f, 1000.0f);
    glm::mat4 ViewTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -100.0f));
    glm::mat4 View = ViewTranslate;
    glm::mat4 Model = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f));
    glm::mat4 MVP = Projection * View * Model;
    
    int uniform_matrix_mvp = glGetUniformLocation(shaderGLPtr, "umvp");
    
    
    // ... can be used alongside SDL2.
    SDL_Event e;
    float x = 0.0, y = 30.0;
    bool keeprunning = true;
    
    while (keeprunning) {
        //update loop
        while (SDL_PollEvent(&e) != 0) {
            if(e.type == SDL_KEYDOWN || e.type == SDL_QUIT)
                keeprunning = false;
        }
        
        //render loop
        {            
            glUseProgram(shaderGLPtr);
            glUniformMatrix4fv(uniform_matrix_mvp, 1, GL_FALSE, glm::value_ptr(MVP));
            
            glClearColor(0,0,0,1);          // Draw with OpenGL.
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            //glRotatef(10.0,0.0,0.0,1.0);
            // Note that the glBegin() ... glEnd() OpenGL style used below is actually
            // obsolete, but it will do for example purposes. For more information, see
            // SDL_GL_GetProcAddress() or find an OpenGL extension loading library.
            glBegin(GL_TRIANGLES);
            
            glColor3f(1.0,0.0,0.0); glVertex2f(x, y+90.0);
            glColor3f(0.0,1.0,0.0); glVertex2f(x+90.0, y-90.0);
            glColor3f(0.0,0.0,1.0); glVertex2f(x-90.0, y-90.0);
            glEnd();
            
            
            SDL_GL_SwapWindow(window);  // Swap the window/buffer to display the result.
            SDL_Delay(15);              // Pause briefly before moving on to the next cycle.
            
        }
    }
 
    
    // Once finished with OpenGL functions, the SDL_GLContext can be deleted.
    SDL_GL_DeleteContext(glcontext);  
    
    // Done! Close the window, clean-up and exit the program. 
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}

