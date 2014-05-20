//
//  CShader.h
//  osxtest
//
//  Created by Alexander on 26/02/14.
//  Copyright (c) 2014 Alexander. All rights reserved.
//

#ifndef __osxtest__CShader__
#define __osxtest__CShader__

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <OpenGL/gl.h>

class CShader {
private:
	int fragment_ref, vertex_ref;
	int program_ref;
	std::string errormsg;
	
	//initialize class with default values, returns gl assigned int references
	int init();
	//removes any allocated resources
	void deinit();
	
public:
	//constructor, calls init
	CShader();
	
	//compile shader with shader.frag and shader.vert, returns false if that failed
	bool compile(std::string vertpath, std::string fragpath);
	
	//returns the last error if compile returns false
	std::string error();
	
	//returns the gl reference to the shader program
	int getref();
	
	//destructor, calls deinit
	~CShader();
};
#endif /* defined(__osxtest__CShader__) */
