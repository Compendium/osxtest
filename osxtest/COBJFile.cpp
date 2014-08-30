//
//  COBJFile.cpp
//  osxtest
//
//  Created by Alexander on 29/08/14.
//  Copyright (c) 2014 Alexander. All rights reserved.
//

#include "COBJFile.h"

int COBJFile::parse(std::string path){
	
	
	std::ifstream file(path, std::ios::binary);
	if(file.good() == false) {
		printf("[%s] Couldn't open file %s, oh well.", "OBJFileImport", path.c_str());
		return false;
	}
	file.seekg(0, std::ios::end);
	std::streamsize filesize = file.tellg();
	file.seekg(0, std::ios::beg);
	
	for( std::string line; getline(file, line);)
	{
		char k = line[0];
		if(k == '#')
			continue; //skip comments
		string v = line.substr(2);
		kvmap[k].push_back(v);
	}
	
	for(auto& kv : kvmap) {
		for(string s : kv.second)
			printf("k=%c, v=%s\n", kv.first, s.c_str());
	}
	
	//poulate vertices
	for(auto& a : kvmap['v']) {
		vec3 vertex;
		string s = a;
		string sx = s.substr(0, s.find(' '));
		string sy = s.substr(s.find(' ')+1, s.find(' ', s.find(' '))-1);
		string sz = s.substr(s.rfind(' ')+1);
		string ss = sx + " " + sy + " " + sz;
		vertex = vec3(stof(sx), stof(sy), stof(sz));
		vertices.push_back(vertex);
		
		//printf("read\n%s\n\"%s\" \"%s\" \"%s\"\n%f %f %f\n\n", s.c_str(), sx.c_str(), sy.c_str(), sz.c_str(), vertex.x, vertex.y, vertex.z);
	}
	vertexcount = vertices.size();
	
	//poulate faces
	for(auto& a : kvmap['f']) {
		polyface face;
		string s = a;
		string sx = s.substr(0, s.find(' '));
		string sy = s.substr(s.find(' '));
		sy = sy.substr(1, sy.rfind(' ')-1);
		string sz = s.substr(s.rfind(' ')+1);
		string ss = sx + " " + sy + " " + sz;
		face.n1 = stoi(sx) - 1;
		face.n2 = stoi(sy) - 1;
		face.n3 = stoi(sz) - 1;
		faces.push_back(face);
		
		//printf("read\n%s\n\"%s\" \"%s\" \"%s\"\n%i %i %i\n\n", s.c_str(), sx.c_str(), sy.c_str(), sz.c_str(), face.n1 + 1, face.n2 + 1, face.n3 + 1);
	}
	facecount = faces.size();
	
	
	
	printf("[OBJFileImport] parsing %s (%.3f kb) obj name is: %s\n", path.c_str(), (float)(filesize / 1024.0f), kvmap['o'][0].c_str());
	
	return 0;
}