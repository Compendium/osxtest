//
//	main.cpp
//	osxtest
//
//	Created by Alexander on 16/01/14.
//	Copyright (c) 2014 Alexander. All rights reserved.
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

#include "CShader.h"

using namespace std;

int main(int argc, const char * argv[])
{
	SDL_Init(SDL_INIT_VIDEO); // Init SDL2
	
	// Create a window containing an OpenGL context, also allow HighDPI settings
	// on devices that support it
	SDL_Window *window = SDL_CreateWindow(
										  "OpenGL test, よ",
										  SDL_WINDOWPOS_UNDEFINED,
										  SDL_WINDOWPOS_UNDEFINED,
										  1280,
										  720,
										  SDL_WINDOW_OPENGL |
										  SDL_WINDOW_RESIZABLE |
										  SDL_WINDOW_ALLOW_HIGHDPI);
	
	// Create an OpenGL context associated with the window.
	SDL_GLContext glcontext = SDL_GL_CreateContext(window);
	
	std::printf("\"%s\" with glsl version \"%s\" rendering on \"%s\"\n",
				glGetString(GL_VERSION),
				glGetString(GL_SHADING_LANGUAGE_VERSION),
				glGetString(GL_RENDERER));
	
	int w, h, pw, ph;
	SDL_GetWindowSize(window, &w, &h);
	SDL_GL_GetDrawableSize(window, &pw, &ph);
	printf("winodwsize: %ix%i (actual: %ix%i)\n", w, h, pw, ph);
	
	if(TTF_Init()==-1) {
		printf("TTF_Init: %s\n", TTF_GetError());
		exit(2);
	}
	if(!TTF_WasInit() && TTF_Init()==-1) {
		printf("TTF_Init: %s\n", TTF_GetError());
		exit(1);
	}
	
	TTF_Font *font;
	int font_height = ph*0.1; //size in pixels, make text appear the same size
	//on all resolutions
	std::printf("font_height: %i\n", font_height);
	font=TTF_OpenFont("resources/hiragino.otf", font_height);
	if(!font) {
		printf("TTF_OpenFont: %s\n", TTF_GetError());
		// handle error
	}
	
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	
	
	// Render some UTF8 text in solid white to a new surface
	// then blit to the upper left of the screen
	// then free the text surface
	SDL_Color color={255,255,255};
	SDL_Surface *surface;
	int text_width = 0, text_height = 0;
	unsigned int text_glref;
	string text = "世界こんにちは！";
	string renderer =
		string(reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
	
	text += "\n(rendering on:\n";
	text += renderer;
	text += ")";
	text += "font size: ";
	text += std::to_string(font_height);
	text += "px";
	
	surface=TTF_RenderUTF8_Blended_Wrapped(font, text.c_str(), color, pw*2);
	
	if(!surface) {
		std::printf("TTF_RenderUTF8_Blended_Wrapped error; %s\n",
					TTF_GetError());
	} else {
		GLuint texture;			// This is a handle to our texture object
		GLenum texture_format = -1;
		GLint  nOfColors;
		
		// Check that the image's width is a power of 2
		if ( (surface->w & (surface->w - 1)) != 0 ) {
			printf("warning: image width is not a power of 2\n");
		}
		
		// Also check if the height is a power of 2
		if ( (surface->h & (surface->h - 1)) != 0 ) {
			printf("warning: image height is not a power of 2\n");
		}
		
		text_height = surface->h;
		text_width = surface->w;
		
		// get the number of channels in the SDL surface
		nOfColors = surface->format->BytesPerPixel;
		if (nOfColors == 4)		// contains an alpha channel
		{
			if (surface->format->Rmask == 0x000000ff)
				texture_format = GL_RGBA;
			else
				texture_format = GL_BGRA;
		} else if (nOfColors == 3)	   // no alpha channel
		{
			if (surface->format->Rmask == 0x000000ff)
				texture_format = GL_RGB;
			else
				texture_format = GL_BGR;
		} else {
			printf("warning: the image is not truecolor..\
				   this will probably break\n");
			// this error should not go unhandled
		}
		
		// Have OpenGL generate a texture object handle for us
		glGenTextures( 1, &texture );
		
		// Bind the texture object
		glBindTexture( GL_TEXTURE_2D, texture );
		
		// Set the texture's stretching properties
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
		
		// Edit the texture object's image data using the information SDL_Surface gives us
		glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA8, surface->w, surface->h, 0,
					 texture_format, GL_UNSIGNED_BYTE, surface->pixels );
		
		// Free the SDL_Surface only if it was successfully created
		if ( surface ) {
			SDL_FreeSurface( surface );
		}
		text_glref = texture;
	}
	

	CShader * textShader = new CShader();
	textShader->compile("resources/text.vert", "resources/text.frag");
	printf("%d\n", textShader->getref());
	
	
	//glm::mat4 Projection = glm::perspectiveFov(90.0f, (float)pw, (float)ph, 1.0f, 1000.0f);
	//glm::mat4 Projection = glm::ortho(-(pw/2.f), +(pw/2.f), -(ph/2.f), +(ph/2.f));
	glm::mat4 Projection = glm::ortho(0.0f, (float)pw, 0.0f, (float)ph);
	glm::mat4 ViewTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	glm::mat4 View = glm::mat4(1.0f) * ViewTranslate;
	glm::mat4 Model = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f));
	glm::mat4 MVP = Projection * View * Model;
	
	int uniform_matrix_mvp = glGetUniformLocation(textShader->getref(), "umvp");
	int uniform_deltatime = glGetUniformLocation(textShader->getref(), "udt");
	
	/* Create a variable to hold the VBO identifier */
	GLuint triangleVBO;
	
	const unsigned int attrib_vertpos = glGetAttribLocation(textShader->getref(), "attrib_vertexpos");
	const unsigned int attrib_texpos = glGetAttribLocation(textShader->getref(), "attrib_texpos");
	
	//ph, pw, text_height, text_width
	
	/* Vertices of two triangles forming a quad (counter-clockwise winding) */
	float dataText[] = {
		0.0, 0.0, 0.0,	  0.0, 1.0,
		(float)text_width, 0.0, 0.0,   1.0, 1.0,
		0.0, (float)text_height, 0.0,	 0.0, 0.0,
		
		(float)text_width, (float)text_height, 0.0,    1.0, 0.0,
		0.0, (float)text_height, 0.0,	0.0, 0.0,
		(float)text_width, 0.0, 0.0,	1.0, 1.0,
	};
	
	/* Create a new VBO and use the variable "triangleVBO" to store the VBO id */
	glGenBuffers(1, &triangleVBO);
	
	/* Make the new VBO active */
	glBindBuffer(GL_ARRAY_BUFFER, triangleVBO);
	
	/* Upload vertex data to the video device */
	glBufferData(GL_ARRAY_BUFFER, sizeof(dataText), dataText, GL_STATIC_DRAW);
	
	/* Specify that our coordinate data is going into attribute attrib_vertpos, and contains three floats per vertex */
	glVertexAttribPointer(attrib_vertpos, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), 0);
	glEnableVertexAttribArray(attrib_vertpos);
	
	/* Specify that our texture coordinate data is going into attribute attrib_texpos, and contains three floats per vertex */
	glVertexAttribPointer(attrib_texpos, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)(3*sizeof(float)));
	glEnableVertexAttribArray(attrib_texpos);
	
	/* Make the new VBO active. */
	glBindBuffer(GL_ARRAY_BUFFER, triangleVBO);
	
	
	CShader * pShader = new CShader();
	pShader->compile("resources/p.vert", "resources/p.frag");
	printf("%d\n", pShader->getref());
	
	glm::mat4 pProjection = glm::perspectiveFov(90.0f, (float)pw, (float)ph, 1.0f, 1000.0f);
    glm::mat4 pViewTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -5.0f));
    glm::mat4 pView = pViewTranslate;
    glm::mat4 pModel = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f));
    glm::mat4 pMVP = pProjection * pView * pModel;
    
    int puniform_matrix_mvp = glGetUniformLocation(pShader->getref(), "umvp");
    int puniform_deltatime = glGetUniformLocation(pShader->getref(), "udt");
    
    GLuint ptriangleVBO;
    
    const unsigned int pshaderAttribute = glGetAttribLocation(pShader->getref(), "attrib_vertexpos");
    
    const float NUM_OF_VERTICES_IN_DATA=3;
    
    float pdata[3][3] = {
        {  0.0, 1.0, 0.0   },
        { -1.0, -1.0, 0.0  },
        {  1.0, -1.0, 0.0  }
    };
    
    glGenBuffers(1, &ptriangleVBO);
    
    glBindBuffer(GL_ARRAY_BUFFER, ptriangleVBO);
    
    glBufferData(GL_ARRAY_BUFFER, NUM_OF_VERTICES_IN_DATA * 3 * sizeof(float), pdata, GL_STATIC_DRAW);
    
    glVertexAttribPointer(pshaderAttribute, 3, GL_FLOAT, GL_FALSE, 0, 0);
    
    glEnableVertexAttribArray(pshaderAttribute);
    
    glBindBuffer(GL_ARRAY_BUFFER, ptriangleVBO);
	
	
	
	
	
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
		pModel = glm::rotate(pModel, 0.050f, glm::vec3(0.f, 1.f, 0.f));
		pModel = glm::rotate(pModel, 0.001f, glm::vec3(1.f, 0.f, 0.f));
		pModel = glm::rotate(pModel, 0.005f, glm::vec3(0.f, 0.f, 1.f));
		pMVP = pProjection * pView * pModel;
		
		//render loop
		{
			glUseProgram(textShader->getref());
			glUniformMatrix4fv(uniform_matrix_mvp, 1, GL_FALSE, glm::value_ptr(MVP));
			glUniform1f(uniform_deltatime, SDL_GetTicks());
			
			
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, text_glref);
			glUniform1i(glGetUniformLocation(textShader->getref(), "colorMap"), 0);
			glUniform4f(glGetUniformLocation(textShader->getref(), "tint"), 0.65, 0.65, 0.65, 1);
			
			glBindBuffer(GL_ARRAY_BUFFER, triangleVBO);
			
			glVertexAttribPointer(attrib_vertpos, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), 0);
			glEnableVertexAttribArray(attrib_vertpos);
			glVertexAttribPointer(attrib_texpos, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)(3*sizeof(float)));
			
			glEnableVertexAttribArray(attrib_texpos);
			
			
			glClearColor(0.2,0.2,0.2,1);		  // Draw with OpenGL.
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			
			/* Actually draw the triangle, givingthe number of vertices provided by invoke glDrawArrays
			 while telling that our data is a triangle and we want to draw 0-3 vertices
			 */
			glDrawArrays(GL_TRIANGLES, 0, 6);
			
			
			
			glUseProgram(pShader->getref());
            glUniformMatrix4fv(puniform_matrix_mvp, 1, GL_FALSE, glm::value_ptr(pMVP));
            glUniform1f(puniform_deltatime, SDL_GetTicks());
			
			glBindBuffer(GL_ARRAY_BUFFER, ptriangleVBO);
			glVertexAttribPointer(pshaderAttribute, 3, GL_FLOAT, GL_FALSE, 0, 0);
			glEnableVertexAttribArray(pshaderAttribute);
			
			
            //glClearColor(1,1,0,1);          // Draw with OpenGL.
			// glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            glDrawArrays(GL_TRIANGLES, 0, 3);
		
			
			
			SDL_GL_SwapWindow(window);	// Swap the window/buffer to display the result.
			SDL_Delay(15);				// Pause briefly before moving on to the next cycle.
			
		}
	}
	
	
	
	
	// Once finished with OpenGL functions, the SDL_GLContext can be deleted.
	SDL_GL_DeleteContext(glcontext);
	
	// Done! Close the window, clean-up and exit the program.
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}

void debuginfo() {
	
}
