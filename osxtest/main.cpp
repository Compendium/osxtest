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
	int font_height = 256;
	font=TTF_OpenFont("resources/hiragino.otf", font_height);
	if(!font) {
		printf("TTF_OpenFont: %s\n", TTF_GetError());
		// handle error
	}
	
	// Create an OpenGL context associated with the window.
	SDL_GLContext glcontext = SDL_GL_CreateContext(window);
	
	//std::printf("\"%s\" with glsl version \"%s\" rendering on \"%s\"\n", glGetString(GL_VERSION), glGetString(GL_SHADING_LANGUAGE_VERSION), glGetString(GL_RENDERER));
	
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	
	
	// Render some UTF8 text in solid white to a new surface
	// then blit to the upper left of the screen
	// then free the text surface
	SDL_Color color={255,255,255};
	SDL_Surface *text_surface;
	int text_width = 0, text_height = 0;
	unsigned int text_glref;
	if(!(text_surface=TTF_RenderText_Blended(font, "Hello World!", color))) {
		//handle error here, perhaps print TTF_GetError at least
	} else {
		SDL_SaveBMP(text_surface, "render.bmp");
		GLuint texture;			// This is a handle to our texture object
		SDL_Surface *surface;	// This surface will tell us the details of the image
		GLenum texture_format = -1;
		GLint  nOfColors;
		
		if ( (surface = text_surface/*SDL_CreateRGBSurface(0, 128, 128, 32, 0, 0, 0, 0)*/) ) {
			/*for (int x = 0; x < 128; x++) {
			 for (int y = 0; y < 128; y++) {
			 ((unsigned int*)surface->pixels)[x + y * 128] = x+y + 0x00ff0000;
			 }
			 }*/
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
				printf("warning: the image is not truecolor..  this will probably break\n");
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
		}
		else {
			printf("SDL could not load image.bmp: %s\n", SDL_GetError());
			SDL_Quit();
			return 1;
		}
		
		// Free the SDL_Surface only if it was successfully created
		if ( surface ) {
			SDL_FreeSurface( surface );
		}
		text_glref = texture;
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
	
	
	
	//glm::mat4 Projection = glm::perspectiveFov(90.0f, (float)pw, (float)ph, 1.0f, 1000.0f);
	glm::mat4 Projection = glm::ortho(-(pw/2.f), +(pw/2.f), -(ph/2.f), +(ph/2.f));
	glm::mat4 ViewTranslate = glm::mat4();//glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -10.0f));
	glm::mat4 View = ViewTranslate;
	glm::mat4 Model = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f));
	glm::mat4 MVP = Projection * View * Model;
	
	int uniform_matrix_mvp = glGetUniformLocation(shaderGLPtr, "umvp");
	int uniform_deltatime = glGetUniformLocation(shaderGLPtr, "udt");
	
	/* Create a variable to hold the VBO identifier */
	GLuint triangleVBO;
	
	const unsigned int attrib_vertpos = glGetAttribLocation(shaderGLPtr, "attrib_vertexpos");
	const unsigned int attrib_texpos = glGetAttribLocation(shaderGLPtr, "attrib_texpos");
	
	//ph, pw, text_height, text_width
	
	/* Vertices of two triangles forming a quad (counter-clockwise winding) */
	float data[] = {
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
			
			glClearColor(0.2,0.2,0.2,1);		  // Draw with OpenGL.
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			
			/* Actually draw the triangle, givingthe number of vertices provided by invoke glDrawArrays
			 while telling that our data is a triangle and we want to draw 0-3 vertices
			 */
			glDrawArrays(GL_TRIANGLES, 0, 6);
			
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

