//
//  CVertexBuffer.cpp
//  osxtest
//
//  Created by Alexander on 05/06/14.
//  Copyright (c) 2014 Alexander. All rights reserved.
//
//TODO add safety guards, prevent array overflows

#include "CVertexBuffer.h"

CVertexBuffer::CVertexBuffer () {
	glGenBuffers(1, &buffer_id);
	replacemode = false;
	replaceoffset = replacecount = 0;
}

void CVertexBuffer::add (std::vector<float> fp) {
	
}

void CVertexBuffer::add (float f) {
	if(replacemode) {
		fv[replaceoffset + replacecount++] = f;
	} else {
		fv.push_back(f);
	}
}

void CVertexBuffer::add(float f1, float f2) {
	if(replacemode) {
		fv[replaceoffset + replacecount++] = f1;
		fv[replaceoffset + replacecount++] = f2;
	} else {
		fv.push_back(f1);
		fv.push_back(f2);
	}
}

void CVertexBuffer::add(float f1, float f2, float f3) {
	if(replacemode) {
		fv[replaceoffset + replacecount++] = f1;
		fv[replaceoffset + replacecount++] = f2;
		fv[replaceoffset + replacecount++] = f3;
	} else {
		fv.push_back(f1);
		fv.push_back(f2);
		fv.push_back(f3);
	}
}

void CVertexBuffer::upload() {
	glBindBuffer(GL_ARRAY_BUFFER, buffer_id);
	if(replacemode) {
		glBufferSubData(GL_ARRAY_BUFFER, replaceoffset * sizeof(float), replacecount * sizeof(float), &fv[replaceoffset]);
		replacemode = false;
		replaceoffset = replacecount = 0;
	} else {
			
		//TODO make the usage a parameter?
		glBufferData(GL_ARRAY_BUFFER, fv.size() * sizeof(float), &fv[0], GL_STATIC_DRAW);
	}
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void CVertexBuffer::replace(int offset) {
	replacemode = true;
	replaceoffset = offset;
}

void CVertexBuffer::addAttribPointer(int ap_gl_id, int size, int stride, int offset){
	SVertexAttrib sva = SVertexAttrib();
	sva.attrib_id = ap_gl_id;
	sva.elements = size;
	sva.stride = stride;
	sva.offset = offset;
	
	vertex_attribs.push_back(sva);
}

void CVertexBuffer::enable (){
	glBindBuffer(GL_ARRAY_BUFFER, buffer_id);
	for(SVertexAttrib sva : vertex_attribs) {
		glEnableVertexAttribArray(sva.attrib_id);
		glVertexAttribPointer(sva.attrib_id, sva.elements, GL_FLOAT, GL_FALSE,
							  sva.stride*sizeof(float), (void*)(sva.offset*sizeof(float)));
	}
}

void CVertexBuffer::disable (){
	for(SVertexAttrib sva : vertex_attribs) {
		glDisableVertexAttribArray(sva.attrib_id);
	}
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}