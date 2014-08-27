//
//  CVertexBatch.h
//  osxtest
//
//  Created by Alexander on 23/08/14.
//  Copyright (c) 2014 Alexander. All rights reserved.
//

#ifndef __osxtest__CVertexBatch__
#define __osxtest__CVertexBatch__

#include <iostream>
#include <vector>
#include <map>

#define GLM_FORCE_RADIANS
#include "glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "CVertexBuffer.h"
#include "CShader.h"
#include "CTexture.h"

using namespace std;
using namespace glm;

/* TODO TODO TODO
		- add sorting based on texture, since texture state-changes are expensive
		- don't use one CVertexBuffer per object, use one for multiple entries (dpending on vertexcount?)
 */

class CVertexBatch {
private:
	struct SVertexAttrib {
		int attrib_id;
		int elements;
		int stride;
		int offset;
	};
	struct Entry_meta {
		vector<SVertexAttrib> vertex_attribs;
		int vertex_offset;
		int vertex_count;
		
		CShader * shader;
		CVertexBuffer * vertex_buffer;
		CTexture * texture;
		
		mat4 * projection;
		mat4 * view;
		mat4 * model;
		
		Entry_meta () {
			shader = nullptr;
			vertex_buffer = nullptr;
			texture = nullptr;
			projection = view = model = nullptr;
			vertex_buffer = new CVertexBuffer();
		}
	};
	
	CVertexBuffer vertex_buffer;
	vector<Entry_meta> batch_entries;
	
	CShader basic_color_shader;
	CShader basic_texture_shader;
	
public:
	CVertexBatch();
	
	/* 
	 adds a colored cube to the batch, using basic_color_shader
		vec3 pos - world position of the center of the cube
		vec3 dim - dimensions of the cube
		vec4 color[] - a 6 element array that defines colors of the cube faces
	 returns index number
	 */
	int addColoredCube (mat4 * m, mat4 * v, mat4 * p, vec3 pos, vec3 dim, vec4 color[]);
	
	int addTexturedCube (mat4 * m, mat4 * v, mat4 * p, vec3 pos, vec3 dim, CTexture * texture, vec2 texfront[], vec2 texback[], vec2 texleft[], vec2 texright[], vec2 textop[], vec2 texbot[]);
	
	/* 
	 draws all the entries currently in the batch
	 returns the number of glDraw calls
	 */
	int draw();
	
	/*
	 draws a specific element only
	 returns number of glDraw calls
	 */
	int draw(int indexn);
};

#endif /* defined(__osxtest__CVertexBatch__) */
