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
#include <SDL2_ttf/SDL_ttf.h>
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

    if(TTF_Init()==-1) {
        printf("TTF_Init: %s\n", TTF_GetError());
        exit(2);
    }
    if(!TTF_WasInit() && TTF_Init()==-1) {
        printf("TTF_Init: %s\n", TTF_GetError());
        exit(1);
    }

    // load font.ttf at size 16 into font
    TTF_Font *font;
    font=TTF_OpenFont("resources/arial.ttf", 56);
    if(!font) {
        printf("TTF_OpenFont: %s\n", TTF_GetError());
        // handle error
    }
    
    // Create an OpenGL context associated with the window.
    SDL_GLContext glcontext = SDL_GL_CreateContext(window);
    
    //std::printf("\"%s\" with glsl version \"%s\" rendering on \"%s\"\n", glGetString(GL_VERSION), glGetString(GL_SHADING_LANGUAGE_VERSION), glGetString(GL_RENDERER));
    
    glEnable(GL_TEXTURE_2D);
    
    // Render some UTF8 text in solid white to a new surface
    // then blit to the upper left of the screen
    // then free the text surface
    SDL_Color color={255,0,255};
    SDL_Surface *text_surface;
    unsigned int text_glref;
    if(!(text_surface=TTF_RenderText_Solid(font, "Hello World!", color))) {
        //handle error here, perhaps print TTF_GetError at least
    } else {
        //SDL_FillRect(text_surface, NULL, 0xffffffff);
        SDL_SaveBMP(text_surface, "surface.bmp");
        int colors = text_surface->format->BytesPerPixel;
        int texture_format;
        if (colors == 4) {   // alpha
            if (text_surface->format->Rmask == 0x000000ff)
                texture_format = GL_RGBA;
            else
                texture_format = GL_BGRA;
        } else {             // no alpha
            if (text_surface->format->Rmask == 0x000000ff)
                texture_format = GL_RGB;
            else
                texture_format = GL_BGR;
        }
        
        unsigned int ret;
        glActiveTexture(GL_TEXTURE0);
        glGenTextures(1, &ret);
        glBindTexture(GL_TEXTURE_2D, ret);
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,     GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,     GL_CLAMP_TO_EDGE);
        
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, text_surface->w, text_surface->h, 0, GL_BGRA, GL_UNSIGNED_BYTE, text_surface->pixels);
        
        text_glref = ret;
        
        //SDL_FreeSurface(text_surface);
    }
    
    int w, h, pw, ph;
    SDL_GetWindowSize(window, &w, &h);
    SDL_GL_GetDrawableSize(window, &pw, &ph);
    
    printf("winodwsize: %ix%i (actual: %ix%i)\n", w, h, pw, ph);
    
  
    int shaderGLPtr = 0;
    int vertexShaderGLPtr = 0, fragmentShaderGLPtr = 0;
    fragmentShaderGLPtr = glCreateShader(GL_FRAGMENT_SHADER);
    vertexShaderGLPtr = glCreateShader(GL_VERTEX_SHADER);
    shaderGLPtr = glCreateProgram();
    
    
    
    char * msgbuffer = new char[1024];
    int rlength = 0;
    
    //fragment shader
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
        
        glCompileShader(fragmentShaderGLPtr);
        
        int ecode;
        glGetShaderiv(fragmentShaderGLPtr, GL_COMPILE_STATUS, &ecode);
        if(ecode == GL_FALSE){
            cout << "compiling fragment shader error:";
            glGetShaderInfoLog(fragmentShaderGLPtr, 1024, &rlength, msgbuffer);
            cout << msgbuffer << endl;
        }
    }
    
    //vertex shader
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
        
        glCompileShader(vertexShaderGLPtr);
        
        int ecode;
        glGetShaderiv(vertexShaderGLPtr, GL_COMPILE_STATUS, &ecode);
        if(ecode == GL_FALSE) {
            cout << "compiling vertex shader error:";
            glGetShaderInfoLog(vertexShaderGLPtr, 1024, &rlength, msgbuffer);
            cout << msgbuffer << endl;
        }
    }
    
    //link both fragment and vertex shader into shaderprogram
    {
        glAttachShader(shaderGLPtr, vertexShaderGLPtr);
        glAttachShader(shaderGLPtr, fragmentShaderGLPtr);
        
        int ecode;
        
        glLinkProgram(shaderGLPtr);
        glGetProgramiv(shaderGLPtr, GL_LINK_STATUS, &ecode);
        if(ecode == GL_FALSE) {
            cout << "linking shaderprogram error:";
            glGetProgramInfoLog(shaderGLPtr, 1024, &rlength, msgbuffer);
            cout << msgbuffer << endl;
        }
        
        glValidateProgram(shaderGLPtr);
        glGetProgramiv(shaderGLPtr, GL_VALIDATE_STATUS, &ecode);
        if(ecode == GL_FALSE) {
            cout << "validating shaderprogram error:";
            glGetProgramInfoLog(shaderGLPtr, 1024, &rlength, msgbuffer);
            cout << msgbuffer << endl;
        }
    }
    
    
    
    glm::mat4 Projection = glm::perspectiveFov(90.0f, (float)pw, (float)ph, 1.0f, 1000.0f);
    glm::mat4 ViewTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -10.0f));
    glm::mat4 View = ViewTranslate;
    glm::mat4 Model = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f));
    glm::mat4 MVP = Projection * View * Model;
    
    int uniform_matrix_mvp = glGetUniformLocation(shaderGLPtr, "umvp");
    int uniform_deltatime = glGetUniformLocation(shaderGLPtr, "udt");
    
    /* Create a variable to hold the VBO identifier */
    GLuint triangleVBO;
    
    const unsigned int attrib_vertpos = glGetAttribLocation(shaderGLPtr, "attrib_vertexpos");
    const unsigned int attrib_texpos = glGetAttribLocation(shaderGLPtr, "attrib_texpos");

    /* Vertices of two triangles forming a quad (counter-clockwise winding) */
    float data[] = {
        0.0, 0.0, 0.0,    0.0, 0.0,
        10.0, 0.0, 0.0,   1.0, 0.0,
        0.0, 10.0, 0.0,    0.0, 1.0,
        
        10.0, 10.0, 0.0,    1.0, 1.0,
        0.0, 10.0, 0.0,   0.0, 1.0,
        10.0, 0.0, 0.0,    1.0, 0.0,
    };
    
    /* Create a new VBO and use the variable "triangleVBO" to store the VBO id */
    glGenBuffers(1, &triangleVBO);
    
    /* Make the new VBO active */
    glBindBuffer(GL_ARRAY_BUFFER, triangleVBO);
    
    /* Upload vertex data to the video device */
    glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);
    
    /* Specify that our coordinate data is going into attribute attrib_vertpos, and contains three floats per vertex */
    glVertexAttribPointer(attrib_vertpos, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), 0);
    glEnableVertexAttribArray(attrib_vertpos);
    
    /* Specify that our texture coordinate data is going into attribute attrib_texpos, and contains three floats per vertex */
    glVertexAttribPointer(attrib_texpos, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(attrib_texpos);
    
    /* Make the new VBO active. */
    glBindBuffer(GL_ARRAY_BUFFER, triangleVBO);

    SDL_Event e;
    bool keeprunning = true;
    
    while (keeprunning) {
        //update loop -- first work all the events
        while (SDL_PollEvent(&e) != 0) {
            if(e.type == SDL_KEYDOWN || e.type == SDL_QUIT)
                keeprunning = false;
        }//then do all the not-event stuff
        {
            int mousex, mousey;
            SDL_GetMouseState(&mousex, &mousey);
            int windowwidth, windowheight;
            SDL_GetWindowSize(window, &windowwidth, &windowheight);
            if(mousex < windowwidth * 0.1) {
                MVP = glm::translate(MVP, glm::vec3(1.0f, 0.0f, 0.0f));
            } else if (mousex > windowwidth - windowwidth * 0.1) {
                MVP = glm::translate(MVP, glm::vec3(-1.0f, 0.0f, 0.0f));
            }
            if(mousey < windowheight * 0.08) {
                MVP = glm::translate(MVP, glm::vec3(0.0f, -1.0f, 0.0f));
            } else if (mousey > windowheight - windowheight * 0.08) {
                MVP = glm::translate(MVP, glm::vec3(0.0f, 1.0f, 0.0f));
            }
        }
        
        //render loop
        {            
            glUseProgram(shaderGLPtr);
            glUniformMatrix4fv(uniform_matrix_mvp, 1, GL_FALSE, glm::value_ptr(MVP));
            glUniform1f(uniform_deltatime, SDL_GetTicks());
            
            
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, text_glref);
            glUniform1i(glGetUniformLocation(shaderGLPtr, "colorMap"), 0);
            
            glClearColor(0,0,0,1);          // Draw with OpenGL.
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            
            /* Actually draw the triangle, giving the number of vertices provided by invoke glDrawArrays
             while telling that our data is a triangle and we want to draw 0-3 vertices
             */
            glDrawArrays(GL_TRIANGLES, 0, 9);
            
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

