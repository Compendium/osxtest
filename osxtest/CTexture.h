//
//  CTexture.h
//  osxtest
//
//  Created by Alexander on 08/06/14.
//  Copyright (c) 2014 Alexander. All rights reserved.
//

#ifndef __osxtest__CTexture__
#define __osxtest__CTexture__

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <OpenGL/gl.h>
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>

class CTexture {
private:
	int width, height;
	int bpp;
	unsigned int glref;
	bool warned;
public:
	CTexture();
	~CTexture();
	void load(SDL_Surface * surface);
	void load(std::string path);
	
	int getref();
	int getWidth();
	int getHeight();
};
#endif /* defined(__osxtest__CTexture__) */
