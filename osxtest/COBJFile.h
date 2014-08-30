//
//  COBJFile.h
//  osxtest
//
//  Created by Alexander on 29/08/14.
//  Copyright (c) 2014 Alexander. All rights reserved.
//

#ifndef __osxtest__COBJFile__
#define __osxtest__COBJFile__

#include <iostream>
#include <map>
#include <fstream>
#include <vector>
#include "glm.hpp"
#include "CShader.h"
#include "CTexture.h"

using namespace std;
using namespace glm;

class COBJFile {
public:
	struct polyface {
		int n1, n2, n3;
	};
	std::map<char, vector<string>> kvmap;
	
	unsigned long vertexcount;
	unsigned long facecount;
	vector<polyface> faces;
	vector<vec3> vertices;
	CShader * shader;
	CTexture * texture;
	
	int parse(std::string path);
};
#endif /* defined(__osxtest__COBJFile__) */
