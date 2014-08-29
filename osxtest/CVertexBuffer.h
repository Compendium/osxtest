//
//  CVertexBuffer.h
//  osxtest
//
//  Created by Alexander on 05/06/14.
//  Copyright (c) 2014 Alexander. All rights reserved.
//

#ifndef __osxtest__CVertexBuffer__
#define __osxtest__CVertexBuffer__

#include <iostream>
#include <vector>
#include <OpenGL/gl.h>
#include "glm.hpp"

using namespace glm;

class CVertexBuffer {
private:
	struct SVertexAttrib {
		int attrib_id;
		int elements;
		int stride;
		int offset;
	};
	std::vector<SVertexAttrib> vertex_attribs;
	std::vector<float> fv;
	unsigned int buffer_id;
	bool replacemode;
	int replaceoffset, replacecount;
public:
	CVertexBuffer();
	void add (std::vector<float> fp);
	void add (vec2 f);
	void add (vec3 f);
	void add (vec4 f);
	void add (float f);
	void add (float f1, float f2);
	void add (float f1, float f2, float f3);
	void upload();
	void reserve (int elemcount);
	void replace(int offset);
	void addAttribPointer(int ap_gl_id, int size, int stride, int offset);
	
	unsigned int getCount();
	
	void enable ();
	void disable ();
};

#endif /* defined(__osxtest__CVertexBuffer__) */
