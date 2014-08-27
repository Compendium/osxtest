//
//  CVertexBatch.cpp
//
//  Created by Alexander on 23/08/14.
//  Copyright (c) 2014 Alexander. All rights reserved.
//

#include "CVertexBatch.h"

CVertexBatch::CVertexBatch (){
	basic_color_shader.compile("resources/basic_color_shader.vert", "resources/basic_color_shader.frag");
	basic_texture_shader.compile("resources/basic_texture_shader.vert", "resources/basic_texture_shader.frag");
}

int CVertexBatch::addColoredCube(mat4 * m, mat4 * v, mat4 * p, vec3 pos, vec3 dim, vec4 *color){
	int vertexcount = 36;
	
	vec3 c1 = vec3(color[0]);
	vec3 c2 = vec3(color[1]);
	vec3 c3 = vec3(color[2]);
	vec3 c4 = vec3(color[3]);
	vec3 c5 = vec3(color[4]);
	vec3 c6 = vec3(color[5]);
	vec3 c7 = vec3(color[6]);
	vec3 c8 = vec3(color[7]);
	
	Entry_meta meta;
	meta.vertex_count = 36;
	int offset = 0;
	for(Entry_meta em : batch_entries)
		offset += em.vertex_count;
	meta.vertex_offset = offset;
	meta.shader = &basic_color_shader;
	meta.vertex_buffer = &vertex_buffer;//------
	meta.model = m;
	meta.view = v;
	meta.projection = p;
	
	
	CVertexBuffer * vb = meta.vertex_buffer;
	int vboffset = vb->getCount();
	//front a
	vb->add(-0.5 * dim.x + pos.x, -0.5 * dim.y + pos.y, +0.5 * dim.z + pos.z); vb->add(c1);
	vb->add(+0.5 * dim.x + pos.x, +0.5 * dim.y + pos.y, +0.5 * dim.z + pos.z); vb->add(c4);
	vb->add(-0.5 * dim.x + pos.x, +0.5 * dim.y + pos.y, +0.5 * dim.z + pos.z); vb->add(c3);
	//front b
	vb->add(-0.5 * dim.x + pos.x, -0.5 * dim.y + pos.y, +0.5 * dim.z + pos.z); vb->add(c1);
	vb->add(+0.5 * dim.x + pos.x, -0.5 * dim.y + pos.y, +0.5 * dim.z + pos.z); vb->add(c2);
	vb->add(+0.5 * dim.x + pos.x, +0.5 * dim.y + pos.y, +0.5 * dim.z + pos.z); vb->add(c4);
	//back a
	vb->add(-0.5 * dim.x + pos.x, +0.5 * dim.y + pos.y, -0.5 * dim.z + pos.z); vb->add(c5);
	vb->add(+0.5 * dim.x + pos.x, +0.5 * dim.y + pos.y, -0.5 * dim.z + pos.z); vb->add(c6);
	vb->add(-0.5 * dim.x + pos.x, -0.5 * dim.y + pos.y, -0.5 * dim.z + pos.z); vb->add(c8);
	//back b
	vb->add(+0.5 * dim.x + pos.x, +0.5 * dim.y + pos.y, -0.5 * dim.z + pos.z); vb->add(c6);
	vb->add(+0.5 * dim.x + pos.x, -0.5 * dim.y + pos.y, -0.5 * dim.z + pos.z); vb->add(c7);
	vb->add(-0.5 * dim.x + pos.x, -0.5 * dim.y + pos.y, -0.5 * dim.z + pos.z); vb->add(c8);
	//left a
	vb->add(-0.5 * dim.x + pos.x, +0.5 * dim.y + pos.y, +0.5 * dim.z + pos.z); vb->add(c3);
	vb->add(-0.5 * dim.x + pos.x, +0.5 * dim.y + pos.y, -0.5 * dim.z + pos.z); vb->add(c5);
	vb->add(-0.5 * dim.x + pos.x, -0.5 * dim.y + pos.y, +0.5 * dim.z + pos.z); vb->add(c1);
	//left b
	vb->add(-0.5 * dim.x + pos.x, -0.5 * dim.y + pos.y, -0.5 * dim.z + pos.z); vb->add(c8);
	vb->add(-0.5 * dim.x + pos.x, -0.5 * dim.y + pos.y, +0.5 * dim.z + pos.z); vb->add(c1);
	vb->add(-0.5 * dim.x + pos.x, +0.5 * dim.y + pos.y, -0.5 * dim.z + pos.z); vb->add(c5);
	//right a
	vb->add(+0.5 * dim.x + pos.x, -0.5 * dim.y + pos.y, +0.5 * dim.z + pos.z); vb->add(c2);
	vb->add(+0.5 * dim.x + pos.x, +0.5 * dim.y + pos.y, -0.5 * dim.z + pos.z); vb->add(c6);
	vb->add(+0.5 * dim.x + pos.x, +0.5 * dim.y + pos.y, +0.5 * dim.z + pos.z); vb->add(c4);
	//right b
	vb->add(+0.5 * dim.x + pos.x, +0.5 * dim.y + pos.y, -0.5 * dim.z + pos.z); vb->add(c6);
	vb->add(+0.5 * dim.x + pos.x, -0.5 * dim.y + pos.y, +0.5 * dim.z + pos.z); vb->add(c2);
	vb->add(+0.5 * dim.x + pos.x, -0.5 * dim.y + pos.y, -0.5 * dim.z + pos.z); vb->add(c7);
	//bottom a
	vb->add(-0.5 * dim.x + pos.x, -0.5 * dim.y + pos.y, -0.5 * dim.z + pos.z); vb->add(c8);//1
	vb->add(+0.5 * dim.x + pos.x, -0.5 * dim.y + pos.y, +0.5 * dim.z + pos.z); vb->add(c2);//2
	vb->add(-0.5 * dim.x + pos.x, -0.5 * dim.y + pos.y, +0.5 * dim.z + pos.z); vb->add(c1);//3
	//bottom b
	vb->add(+0.5 * dim.x + pos.x, -0.5 * dim.y + pos.y, -0.5 * dim.z + pos.z); vb->add(c7);//3
	vb->add(+0.5 * dim.x + pos.x, -0.5 * dim.y + pos.y, +0.5 * dim.z + pos.z); vb->add(c2);//2
	vb->add(-0.5 * dim.x + pos.x, -0.5 * dim.y + pos.y, -0.5 * dim.z + pos.z); vb->add(c8);//1
	//top a
	vb->add(-0.5 * dim.x + pos.x, +0.5 * dim.y + pos.y, +0.5 * dim.z + pos.z); vb->add(c3);//3
	vb->add(+0.5 * dim.x + pos.x, +0.5 * dim.y + pos.y, +0.5 * dim.z + pos.z); vb->add(c4);//2
	vb->add(-0.5 * dim.x + pos.x, +0.5 * dim.y + pos.y, -0.5 * dim.z + pos.z); vb->add(c5);//1
	//top b
	vb->add(-0.5 * dim.x + pos.x, +0.5 * dim.y + pos.y, -0.5 * dim.z + pos.z); vb->add(c5);//1
	vb->add(+0.5 * dim.x + pos.x, +0.5 * dim.y + pos.y, +0.5 * dim.z + pos.z); vb->add(c4);//2
	vb->add(+0.5 * dim.x + pos.x, +0.5 * dim.y + pos.y, -0.5 * dim.z + pos.z); vb->add(c6);//3
	
	vb->upload();
	
	unsigned int pshaderAttributeVertPos = glGetAttribLocation(meta.shader->getref(), "attrib_vertexpos");
	SVertexAttrib positionattrib;
	positionattrib.attrib_id = pshaderAttributeVertPos;
	positionattrib.elements = 3; //xyz
	positionattrib.stride = 6;//xyzrgb
	positionattrib.offset = vboffset;
	
	unsigned int pshaderAttributeVertCol = glGetAttribLocation(meta.shader->getref(), "attrib_vertexcolor");
	SVertexAttrib colorattrib;
	colorattrib.attrib_id = pshaderAttributeVertCol;
	colorattrib.elements = 3;//rgb
	colorattrib.stride = 6;//xyzrgb
	colorattrib.offset = vboffset + 3; //xyz
	
	meta.vertex_attribs.push_back(positionattrib);
	meta.vertex_attribs.push_back(colorattrib);
	
	batch_entries.push_back(meta);
	return vertexcount;
}

int CVertexBatch::addTexturedCube(mat4 *m, mat4 *v, mat4 *p, vec3 pos, vec3 dim, CTexture * texture, vec2 *texfront, vec2 *texback, vec2 *texleft, vec2 *texright, vec2 *textop, vec2 *texbot){
	int vertexcount = 36;
	
	Entry_meta meta;
	meta.vertex_buffer = &vertex_buffer;
	meta.shader = &basic_texture_shader;
	
	meta.vertex_count = 36;
	int offset = 0;
	for(int i = 0; i < batch_entries.size(); i++)
		offset += batch_entries[i].vertex_count;
	meta.vertex_offset = offset;
	meta.shader = &basic_texture_shader;
	meta.vertex_buffer = &vertex_buffer;//-----
	meta.model = m;
	meta.view = v;
	meta.projection = p;
	meta.texture = texture;
	
	CVertexBuffer * vb = meta.vertex_buffer;
	int vboffset = vb->getCount();
	//front a
	vb->add(-0.5 * dim.x + pos.x, -0.5 * dim.y + pos.y, +0.5 * dim.z + pos.z); vb->add(0,1);
	vb->add(+0.5 * dim.x + pos.x, +0.5 * dim.y + pos.y, +0.5 * dim.z + pos.z); vb->add(1,0);
	vb->add(-0.5 * dim.x + pos.x, +0.5 * dim.y + pos.y, +0.5 * dim.z + pos.z); vb->add(0,0);
	//front b
	vb->add(-0.5 * dim.x + pos.x, -0.5 * dim.y + pos.y, +0.5 * dim.z + pos.z); vb->add(0,1);
	vb->add(+0.5 * dim.x + pos.x, -0.5 * dim.y + pos.y, +0.5 * dim.z + pos.z); vb->add(1,1);
	vb->add(+0.5 * dim.x + pos.x, +0.5 * dim.y + pos.y, +0.5 * dim.z + pos.z); vb->add(1,0);
	//back a
	vb->add(-0.5 * dim.x + pos.x, +0.5 * dim.y + pos.y, -0.5 * dim.z + pos.z); vb->add(1,0);
	vb->add(+0.5 * dim.x + pos.x, +0.5 * dim.y + pos.y, -0.5 * dim.z + pos.z); vb->add(0,0);
	vb->add(-0.5 * dim.x + pos.x, -0.5 * dim.y + pos.y, -0.5 * dim.z + pos.z); vb->add(1,1);
	//back b
	vb->add(+0.5 * dim.x + pos.x, +0.5 * dim.y + pos.y, -0.5 * dim.z + pos.z); vb->add(0,0);
	vb->add(+0.5 * dim.x + pos.x, -0.5 * dim.y + pos.y, -0.5 * dim.z + pos.z); vb->add(0,1);
	vb->add(-0.5 * dim.x + pos.x, -0.5 * dim.y + pos.y, -0.5 * dim.z + pos.z); vb->add(1,1);
	//left a
	vb->add(-0.5 * dim.x + pos.x, +0.5 * dim.y + pos.y, +0.5 * dim.z + pos.z); vb->add(1,0);
	vb->add(-0.5 * dim.x + pos.x, +0.5 * dim.y + pos.y, -0.5 * dim.z + pos.z); vb->add(0,0);
	vb->add(-0.5 * dim.x + pos.x, -0.5 * dim.y + pos.y, +0.5 * dim.z + pos.z); vb->add(1,1);
	//left b
	vb->add(-0.5 * dim.x + pos.x, -0.5 * dim.y + pos.y, -0.5 * dim.z + pos.z); vb->add(0,1);
	vb->add(-0.5 * dim.x + pos.x, -0.5 * dim.y + pos.y, +0.5 * dim.z + pos.z); vb->add(1,1);
	vb->add(-0.5 * dim.x + pos.x, +0.5 * dim.y + pos.y, -0.5 * dim.z + pos.z); vb->add(0,0);
	//right a
	vb->add(+0.5 * dim.x + pos.x, -0.5 * dim.y + pos.y, +0.5 * dim.z + pos.z); vb->add(0,1);
	vb->add(+0.5 * dim.x + pos.x, +0.5 * dim.y + pos.y, -0.5 * dim.z + pos.z); vb->add(1,0);
	vb->add(+0.5 * dim.x + pos.x, +0.5 * dim.y + pos.y, +0.5 * dim.z + pos.z); vb->add(0,0);
	//right b
	vb->add(+0.5 * dim.x + pos.x, +0.5 * dim.y + pos.y, -0.5 * dim.z + pos.z); vb->add(1,0);
	vb->add(+0.5 * dim.x + pos.x, -0.5 * dim.y + pos.y, +0.5 * dim.z + pos.z); vb->add(0,1);
	vb->add(+0.5 * dim.x + pos.x, -0.5 * dim.y + pos.y, -0.5 * dim.z + pos.z); vb->add(1,1);
	//bottom a
	vb->add(-0.5 * dim.x + pos.x, -0.5 * dim.y + pos.y, -0.5 * dim.z + pos.z); vb->add(1,1);//1
	vb->add(+0.5 * dim.x + pos.x, -0.5 * dim.y + pos.y, +0.5 * dim.z + pos.z); vb->add(1,1);//2
	vb->add(-0.5 * dim.x + pos.x, -0.5 * dim.y + pos.y, +0.5 * dim.z + pos.z); vb->add(1,1);//3
	//bottom b
	vb->add(+0.5 * dim.x + pos.x, -0.5 * dim.y + pos.y, -0.5 * dim.z + pos.z); vb->add(1,1);//3
	vb->add(+0.5 * dim.x + pos.x, -0.5 * dim.y + pos.y, +0.5 * dim.z + pos.z); vb->add(1,1);//2
	vb->add(-0.5 * dim.x + pos.x, -0.5 * dim.y + pos.y, -0.5 * dim.z + pos.z); vb->add(1,1);//1
	//top a
	vb->add(-0.5 * dim.x + pos.x, +0.5 * dim.y + pos.y, +0.5 * dim.z + pos.z); vb->add(1,1);//3
	vb->add(+0.5 * dim.x + pos.x, +0.5 * dim.y + pos.y, +0.5 * dim.z + pos.z); vb->add(1,1);//2
	vb->add(-0.5 * dim.x + pos.x, +0.5 * dim.y + pos.y, -0.5 * dim.z + pos.z); vb->add(1,1);//1
	//top b
	vb->add(-0.5 * dim.x + pos.x, +0.5 * dim.y + pos.y, -0.5 * dim.z + pos.z); vb->add(1,1);//1
	vb->add(+0.5 * dim.x + pos.x, +0.5 * dim.y + pos.y, +0.5 * dim.z + pos.z); vb->add(1,1);//2
	vb->add(+0.5 * dim.x + pos.x, +0.5 * dim.y + pos.y, -0.5 * dim.z + pos.z); vb->add(1,1);//3
	
	
	vb->upload();

	unsigned int pshaderAttributeVertPos = glGetAttribLocation(meta.shader->getref(), "attrib_vertexpos");
	SVertexAttrib positionattrib;
	positionattrib.attrib_id = pshaderAttributeVertPos;
	positionattrib.elements = 3; //xyz
	positionattrib.stride = 5;//xyzuv
	positionattrib.offset = vboffset + 0;
	
	unsigned int pshaderAttributeVertCol = glGetAttribLocation(meta.shader->getref(), "attrib_vertexuv");
	SVertexAttrib colorattrib;
	colorattrib.attrib_id = pshaderAttributeVertCol;
	colorattrib.elements = 2;//uv
	colorattrib.stride = 5;//xyzuv
	colorattrib.offset = vboffset + 3; //xyz
	
	meta.vertex_attribs.push_back(positionattrib);
	meta.vertex_attribs.push_back(colorattrib);
	
	batch_entries.push_back(meta);
	
	return vertexcount;
}

int CVertexBatch::draw(){
	int glcalls = 0;
	vertex_buffer.upload();
	for(int i = 0; i < batch_entries.size(); i++)
		glcalls += draw(i);
	
	return glcalls;
}

int CVertexBatch::draw(int index) {
	int glcalls = 0;
	Entry_meta * m = &batch_entries[index];
	//if(m->texture != nullptr)
	//	return 0;
	
	glUseProgram(m->shader->getref());
	
	glm::mat4 MVP = *m->projection * *m->view * *m->model;
	
	glUniformMatrix4fv(glGetUniformLocation(m->shader->getref(), "umvp"), 1, GL_FALSE, glm::value_ptr(MVP));
	
	if(m->texture != nullptr) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m->texture->getref());
		glUniform1i(glGetUniformLocation(m->shader->getref(), "tex_0"), 0);
	}
	
	
	m->vertex_buffer->enable();
	
	for(SVertexAttrib sva : m->vertex_attribs) {
		glEnableVertexAttribArray(sva.attrib_id);
		glVertexAttribPointer(sva.attrib_id, sva.elements, GL_FLOAT, GL_FALSE, sva.stride*sizeof(float), (void*)(sva.offset*sizeof(float)));
		//printf("stride %i floats, offset %i floats\n", sva.stride, sva.offset);
	}
	//printf("\n");
	//assert(index*36 == m->vertex_offset);
	
	glDrawArrays(GL_TRIANGLES, 0, m->vertex_count);
	glcalls++;
	
	for(SVertexAttrib sva : m->vertex_attribs)
		glDisableVertexAttribArray(sva.attrib_id);
	
	m->vertex_buffer->disable();
	glUseProgram(0);
	return glcalls;
}