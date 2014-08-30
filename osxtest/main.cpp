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
#include <ctime>

#include <SDL2/SDL.h>
#include <SDL2_ttf/SDL_ttf.h>
#include <OpenGL/gl.h>
#define GLM_FORCE_RADIANS
#include "glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "CShader.h"
#include "CVertexBuffer.h"
#include "CTexture.h"
#include "CPrimitives.h"
#include "CVertexBatch.h"
#include "COBJFile.h"

using namespace std;

float itof (int i) {
	return ((float)i/(float)0xff);
}

int main(int argc, const char * argv[])
{
	SDL_Init(SDL_INIT_VIDEO); // Init SDL2
	
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 16);
	
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
	
	std::printf("Starting engines... \"%s\" with glsl version \"%s\" rendering on \"%s\"\n",
				glGetString(GL_VERSION),
				glGetString(GL_SHADING_LANGUAGE_VERSION),
				glGetString(GL_RENDERER));
	
	int w, h, pw, ph;
	SDL_GetWindowSize(window, &w, &h);
	SDL_GL_GetDrawableSize(window, &pw, &ph);
	printf("winodwsize: %ix%i (physical: %ix%i)\n", w, h, pw, ph);
	
	if(TTF_Init()==-1) {
		printf("TTF_Init: %s\n", TTF_GetError());
		exit(2);
	}
	if(!TTF_WasInit() && TTF_Init()==-1) {
		printf("TTF_Init: %s\n", TTF_GetError());
		exit(1);
	}
	
	TTF_Font *font;
	int font_height = ph*0.02; //size in pixels, make text appear the same size
	//on all resolutions
	std::printf("font_height: %i\n", font_height);
	font=TTF_OpenFont("resources/hiragino.otf", font_height);
	if(!font) {
		printf("TTF_OpenFont: %s\n", TTF_GetError());
		// handle error
	}
	
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glEnable(GL_SMOOTH);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_MULTISAMPLE);
	glDisable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);
	glEnable(GL_DEPTH_TEST);
	
	
	// Render some UTF8 text in solid white to a new surface
	// then blit to the upper left of the screen
	// then free the text surface
	SDL_Color color={255,255,255};
	SDL_Surface *surface;
	
	CVertexBatch cvb;

	string text = "こんにちは！";
	string renderer =
		string(reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
	
	text += " (rendering on:";
	text += renderer;
	text += ")";
	text += "font size: ";
	text += std::to_string(font_height);
	text += "px";
	
	int mw, mh;
	//int TTF_SizeUTF8(TTF_Font *font, const char *text, int *w, int *h)
	TTF_SizeUTF8(font, text.c_str(), &mw, &mh);
	printf("calculated size w x h = %i x %i \n", mw, mh);
	surface=TTF_RenderUTF8_Blended_Wrapped(font, text.c_str(), color, pw);
	
	CTexture * texttex = new CTexture();
	
	if(!surface) {
		std::printf("TTF_RenderUTF8_Blended_Wrapped error; %s\n",
					TTF_GetError());
	} else {
		texttex->load(surface);
		SDL_FreeSurface(surface);
	}
	

	CShader * textShader = new CShader();
	textShader->compile("resources/text.vert", "resources/text.frag");
	
	//glm::mat4 Projection = glm::perspectiveFov(90.0f, (float)pw, (float)ph, 1.0f, 1000.0f);
	//glm::mat4 Projection = glm::ortho(-(pw/2.f), +(pw/2.f), -(ph/2.f), +(ph/2.f));
	glm::mat4 Projection = glm::ortho(0.0f, (float)pw, 0.0f, (float)ph);
	glm::mat4 ViewTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	glm::mat4 View = glm::mat4(1.0f) * ViewTranslate;
	glm::mat4 Model = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f));
	glm::mat4 MVP = Projection * View * Model;
	
	int uniform_matrix_mvp = glGetUniformLocation(textShader->getref(), "umvp");
	int uniform_deltatime = glGetUniformLocation(textShader->getref(), "udt");
	
	const unsigned int attrib_vertpos = glGetAttribLocation(textShader->getref(), "attrib_vertexpos");
	const unsigned int attrib_texpos = glGetAttribLocation(textShader->getref(), "attrib_texpos");
	
	CVertexBuffer * interfaceBuffer = new CVertexBuffer();
	interfaceBuffer->reserve(15*2);
	interfaceBuffer->addAttribPointer(attrib_vertpos, 3, 5, 0);
	interfaceBuffer->addAttribPointer(attrib_texpos, 2, 5, 3);
	
	glm::mat4 pProjection = glm::perspectiveFov(90.0f, (float)pw, (float)ph, 1.0f, 1000.0f);
    glm::mat4 pViewTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -6.0f));
    glm::mat4 pView = pViewTranslate;
	pView = glm::scale(pView, glm::vec3(4.5f));
    glm::mat4 pModel = glm::scale(glm::mat4(1.0f), glm::vec3(1.5f));
	//pModel = glm::translate(pModel, glm::vec3(0.5f, 0.5f, 0.5f));
    glm::mat4 pMVP = pProjection * pView * pModel;
    
	SDL_Event e;
	bool keeprunning = true;
	int framecount = 0;
	int drawcalls = 0;

	clock_t t = clock();
	float timediff;
	
	CTexture cubetex = CTexture();
	cubetex.load("resources/logo.png");
	
	vec4 colors [] = {vec4(1, 0, 0, 1), vec4(0, 1, 0, 1), vec4(0, 0, 1, 1), vec4(1, 1, 1, 1),
	vec4(1, 1, 0, 1), vec4(1, 0, 1, 1), vec4(0, 1, 1, 1), vec4(0, 0, 0, 1)};
	
	//cvb.addColoredCube(&pModel, &pView, &pProjection, vec3(0,0.5,0), vec3(1,1,1), colors);
	//cvb.addColoredCube(&pModel, &pView, &pProjection, vec3(5,0,0), vec3(1,1,1), colors);
	
	vec2 texfront [] = {vec2(0, 0), vec2(0, 0)};
	vec2 texback [] = {vec2(0, 0), vec2(0, 0)};
	vec2 texleft [] = {vec2(0, 0), vec2(0, 0)};
	vec2 texright [] = {vec2(0, 0), vec2(0, 0)};
	vec2 texbot [] = {vec2(0, 0), vec2(0, 0)};
	vec2 textop [] = {vec2(0, 0), vec2(0, 0)};
	
	//cvb.addTexturedCube(&pModel, &pView, &pProjection, vec3(0, 0.2, 0), vec3(1,1,1), &cubetex, texfront, texback, texleft, texright, textop, texbot);
	//cvb.addTexturedCube(&pModel, &pView, &pProjection, vec3(0, 2, 0), vec3(1,1,1), &cubetex, texfront, texback, texleft, texright, textop, texbot);
	//cvb.addTexturedCube(&pModel, &pView, &pProjection, vec3(0, -2, 0), vec3(1,1,1), &cubetex, texfront, texback, texleft, texright, textop, texbot);
	//cvb.addColoredCube(&pModel, &pView, &pProjection, vec3(2.0,0,0), vec3(1,1,1), colors);
	//cvb.addTexturedCube(&pModel, &pView, &pProjection, vec3(-1, -0.2, 0), vec3(1,1,1), &cubetex, texfront, texback, texleft, texright, textop, texbot);


	//cvb.addColoredCube(&pModel, &pView, &pProjection, vec3(1.0,0,0), vec3(1,1,1), colors);
	//cvb.addColoredCube(&pModel, &pView, &pProjection, vec3(3.0,0,0), vec3(1,1,1), colors);
	//cvb.addColoredCube(&pModel, &pView, &pProjection, vec3(4.0,0,0), vec3(1,1,1), colors);
	
	COBJFile cobj;
	//cobj.parse("resources/untitled.obj");
	
	cvb.addObject(&pModel, &pView, &pProjection, &cubetex, "resources/untitled.obj", "", "", "");
	
	while (keeprunning) {
		t = clock();
		
		// Render some UTF8 text in solid white to a new surface
		// then blit to the upper left of the screen
		// then free the text surface
		SDL_Color color={255,255,255};
		SDL_Surface *surface;
		
		string text = "こんにちは！";
		string renderer =
		string(reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
		
		text += " (rendering on:";
		text += renderer;
		text += ")";
		text += "font size: ";
		text += std::to_string(font_height);
		text += "px ";
		text += "ft: ";
		text += std::to_string(timediff);
		text += " fc: ";
		text += std::to_string(framecount);
		text += " drawcalls: ";
		text += std::to_string(drawcalls);
		drawcalls = 0;
		
		//int TTF_SizeUTF8(TTF_Font *font, const char *text, int *w, int *h)
		surface=TTF_RenderUTF8_Blended_Wrapped(font, text.c_str(), color, pw);

		if(!surface) {
			std::printf("TTF_RenderUTF8_Blended_Wrapped error; %s\n",
						TTF_GetError());
		} else {
			texttex->load(surface);
			SDL_FreeSurface(surface);
		}
		
		interfaceBuffer->replace(0);
		interfaceBuffer->add(0, 0, 0);
		interfaceBuffer->add(0, 1);
		interfaceBuffer->add(texttex->getWidth(), 0, 0);
		interfaceBuffer->add(1, 1);
		interfaceBuffer->add(0, texttex->getHeight(), 0);
		interfaceBuffer->add(0, 0);
		interfaceBuffer->add(texttex->getWidth(), texttex->getHeight(), 0);
		interfaceBuffer->add(1, 0);
		interfaceBuffer->add(0, texttex->getHeight(), 0);
		interfaceBuffer->add(0, 0);
		interfaceBuffer->add(texttex->getWidth(), 0, 0);
		interfaceBuffer->add(1, 1);
		interfaceBuffer->upload();
		
		int mousex, mousey;
		SDL_GetMouseState(&mousex, &mousey);
		//update loop -- first work all the events
		while (SDL_PollEvent(&e) != 0) {
			if(e.type == SDL_KEYDOWN || e.type == SDL_QUIT)
				keeprunning = false;
		}//then do all the not-event stuff
		{
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
		
		pModel = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f));
		
		//pModel = glm::rotate(pModel, 0.050f + framecount/35.0f, glm::vec3(.25f, 1.f, 0.f));
		pModel = glm::rotate(pModel, 0.050f + mousex/35.0f, glm::vec3(0.f, 1.f, 0.f));
		//pModel = glm::rotate(pModel, 0.5f, glm::vec3(1.f, 0.f, 0.f));
		//pModel = glm::rotate(pModel, 0.005f, glm::vec3(0.f, 0.f, 1.f));
		//pModel = glm::translate(pModel, glm::vec3(-0.5f, -0.5f, -0.5f));

		pMVP = pProjection * pView * pModel;
		
		//render loop
		{
			glClearColor(itof(0x40),itof(0x40),itof(0x40),1);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			
			glUseProgram(textShader->getref());
			glUniformMatrix4fv(uniform_matrix_mvp, 1, GL_FALSE, glm::value_ptr(MVP));
			glUniform1f(uniform_deltatime, SDL_GetTicks());
			
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texttex->getref());
			glUniform1i(glGetUniformLocation(textShader->getref(), "colorMap"), 0);
			glUniform4f(glGetUniformLocation(textShader->getref(), "tint"), itof(0xF2), itof(0xEF), itof(0xDC), 1);

			interfaceBuffer->enable();
			glDrawArrays(GL_TRIANGLES, 0, 6);
			interfaceBuffer->disable();
			
			
			drawcalls += cvb.draw();

			
			SDL_GL_SwapWindow(window);
			SDL_Delay(15);
		}
		
		clock_t now = clock();
		float tickdiff = ((float)(now - t));
		timediff = tickdiff /CLOCKS_PER_SEC*1000.0f;
		framecount++;
	}
	
	// Once finished with OpenGL functions, the SDL_GLContext can be deleted.
	SDL_GL_DeleteContext(glcontext);
	
	// Done! Close the window, clean-up and exit the program.
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}
