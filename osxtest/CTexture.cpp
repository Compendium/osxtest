//
//  CTexture.cpp
//  osxtest
//
//  Created by Alexander on 08/06/14.
//  Copyright (c) 2014 Alexander. All rights reserved.
//

#include "CTexture.h"

CTexture::CTexture() {
	glGenTextures(1, &glref);
	
}

CTexture::~CTexture() {
	glDeleteTextures(1, &glref);
}

int CTexture::getref() {
	return glref;
}

int CTexture::getWidth() {
	return width;
}

int CTexture::getHeight() {
	return height;
}

void CTexture::set(SDL_Surface * surface) {
	int text_width = 0, text_height = 0;
	GLenum texture_format = -1;
	GLint  nOfColors;
	
	// Check that the image's width is a power of 2
	if ( (surface->w & (surface->w - 1)) != 0  && warned == false) {
		printf("Warning: image width is not a power of 2 (%i)\n", surface->w);
		warned = true;
	}
	
	// Also check if the height is a power of 2
	if ( (surface->h & (surface->h - 1)) != 0  && warned == false) {
		printf("Warning: image height is not a power of 2 (%i)\n", surface->h);
		warned = true;
	}
	
	this->height = surface->h;
	this->width = surface->w;
	
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
	
	glBindTexture(GL_TEXTURE_2D, glref);
	
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, surface->w, surface->h, 0, texture_format, GL_UNSIGNED_BYTE, surface->pixels);
	
	glBindTexture(GL_TEXTURE_2D, 0);
}