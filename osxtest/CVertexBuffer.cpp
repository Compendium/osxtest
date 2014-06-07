//
//  CVertexBuffer.cpp
//  osxtest
//
//  Created by Alexander on 05/06/14.
//  Copyright (c) 2014 Alexander. All rights reserved.
//

#include "CVertexBuffer.h"

CVertexBuffer::CVertexBuffer () {
	glGenBuffers(1, &buffer_id);
}

void CVertexBuffer::add (float* fp) {
	
}

void CVertexBuffer::add (float f) {
	fv.push_back(f);
}

void CVertexBuffer::add(float f1, float f2) {
	fv.push_back(f1);
	fv.push_back(f2);
}

void CVertexBuffer::add(float f1, float f2, float f3) {
	fv.push_back(f1);
	fv.push_back(f2);
	fv.push_back(f3);
}

void CVertexBuffer::upload() {
	glBindBuffer(GL_ARRAY_BUFFER, buffer_id);
	
	//TODO make the usage a parameter?
	glBufferData(GL_ARRAY_BUFFER, fv.size() * sizeof(float), &fv[0], GL_STATIC_DRAW);
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);
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

/*
 glGenBuffers(1, &ptriangleVBO);
 
 glBindBuffer(GL_ARRAY_BUFFER, ptriangleVBO);
 
 glBufferData(GL_ARRAY_BUFFER, NUM_OF_VERTICES_IN_DATA * 3 * sizeof(float), pdata, GL_STATIC_DRAW);
 
 glVertexAttribPointer(pshaderAttribute, 3, GL_FLOAT, GL_FALSE, 0, 0);
 
 glEnableVertexAttribArray(pshaderAttribute);
 
 glBindBuffer(GL_ARRAY_BUFFER, 0);
 
 
 *****
 
 
 glBindBuffer(GL_ARRAY_BUFFER, triangleVBO);
 glEnableVertexAttribArray(attrib_vertpos);
 glEnableVertexAttribArray(attrib_texpos);
 glVertexAttribPointer(attrib_vertpos, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), 0);
 glVertexAttribPointer(attrib_texpos, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)(3*sizeof(float)));
 
 
 glClearColor(itof(0x40),itof(0x40),itof(0x40),1);		  // Draw with OpenGL.
 glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
 
 glDrawArrays(GL_TRIANGLES, 0, 6);
 
 glDisableVertexAttribArray(attrib_vertpos);
 glDisableVertexAttribArray(attrib_texpos);

*/