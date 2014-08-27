//
//  CShader.cpp
//  osxtest
//
//  Created by Alexander on 26/02/14.
//  Copyright (c) 2014 Alexander. All rights reserved.
//

#include "CShader.h"


CShader::CShader() {
	fragment_ref = vertex_ref = program_ref = -1;
	init();
	printf("[%i] Created new shader object\n", program_ref);
}

CShader::~CShader() {
	printf("[%i] Deleting shader object\n", program_ref);
	fragment_ref = vertex_ref = program_ref = -1;
	deinit();
}

//TODO actually return false on failed compile/link/error
bool CShader::compile(std::string vertpath, std::string fragpath) {
	printf("[%i] Compiling %s and %s\n", program_ref, vertpath.c_str(), fragpath.c_str());
	
	char * msgbuffer = new char[1024];
	int msglength = 0;
	
	{//vert shader
		std::ifstream file(vertpath, std::ios::binary);
		if(file.good() == false) {
			printf("[%i] Couldn't open file %s, oh well.", program_ref, vertpath.c_str());
			return false;
		}
		file.seekg(0, std::ios::end);
		std::streamsize filesize = file.tellg();
		file.seekg(0, std::ios::beg);
		
		std::vector<char> buffer (filesize);
		if(!file.read((char*)buffer.data(), filesize)) {
			printf("[%i] Read operation failed; Couldn't read file content %s",
				   program_ref, vertpath.c_str());
		}
		buffer.push_back('\0'); //add NULL to end of shader source just to be safe, or else GL might run into trouble while reading the source
		
		const char * c_str = buffer.data();
		glShaderSource(vertex_ref, 1, &c_str, NULL);
		glCompileShader(vertex_ref);
		
		int ecode;
		glGetShaderiv(vertex_ref, GL_COMPILE_STATUS, &ecode);
		if(ecode == GL_FALSE) {
			glGetShaderInfoLog(vertex_ref, 1024, &msglength, msgbuffer);
			printf("[%i] Error while compiling vertex shader %s", program_ref, msgbuffer);
			errormsg = msgbuffer;
		}
	}
	{//frag shader
		std::ifstream file(fragpath, std::ios::binary);
		if(file.good() == false) {
			printf("[%i] Couldn't open file %s, oh well.", program_ref, fragpath.c_str());
			return false;
		}
		file.seekg(0, std::ios::end);
		std::streamsize filesize = file.tellg();
		file.seekg(0, std::ios::beg);
		
		std::vector<char> buffer (filesize);
		if(!file.read((char*)buffer.data(), filesize)) {
			printf("[%i] Read operation failed; Couldn't read file content %s",
				   program_ref, fragpath.c_str());
		}
		buffer.push_back('\0'); //add NULL to end of shader source just to be safe, or else GL might run into trouble while reading the source
		
		const char * c_str = buffer.data();
		glShaderSource(fragment_ref, 1, &c_str, NULL);
		glCompileShader(fragment_ref);
		
		int ecode;
		glGetShaderiv(fragment_ref, GL_COMPILE_STATUS, &ecode);
		if(ecode == GL_FALSE) {
			glGetShaderInfoLog(fragment_ref, 1024, &msglength, msgbuffer);
			printf("[%i] Error while compiling fragment shader %s", program_ref, msgbuffer);
			errormsg = msgbuffer;
		}
	}

	
	//link both fragment and vertex shader into shaderprogram
	{
		glAttachShader(program_ref, vertex_ref);
		glAttachShader(program_ref, fragment_ref);
		
		int ecode;
		
		glLinkProgram(program_ref);
		glGetProgramiv(program_ref, GL_LINK_STATUS, &ecode);
		if(ecode == GL_FALSE) {
			glGetProgramInfoLog(program_ref, 1024, &msglength, msgbuffer);
			printf("[%i] Error while linking shaderprogram: %s", program_ref, msgbuffer);
			
		}
		
		glValidateProgram(program_ref);
		glGetProgramiv(program_ref, GL_VALIDATE_STATUS, &ecode);
		if(ecode == GL_FALSE) {
			glGetProgramInfoLog(program_ref, 1024, &msglength, msgbuffer);
			printf("[%i] Error while validating shaderprogram: %s", program_ref, msgbuffer);
		}
	}
	glUseProgram(0);
	return true;
}

std::string CShader::error() {
	return errormsg;
}

int CShader::getref() {
	return program_ref;
}

int CShader::init() {
	fragment_ref = glCreateShader(GL_FRAGMENT_SHADER);
	vertex_ref = glCreateShader(GL_VERTEX_SHADER);
	program_ref = glCreateProgram();
	
	return program_ref;
}

void CShader::deinit() {
	glDeleteShader(fragment_ref);
	glDeleteShader(vertex_ref);
	glDeleteProgram(program_ref);
}