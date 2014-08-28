//
//  CPrimitives.cpp
//  osxtest
//
//  Created by Alexander on 09/06/14.
//  Copyright (c) 2014 Alexander. All rights reserved.
//

#include "CPrimitives.h"

void add(std::vector<float> vf, float f1, float f2, float f3) {
	vf.push_back(f1);
	vf.push_back(f2);
	vf.push_back(f3);
}


//TODO how do i cascade the messy work of adding tex-coords/colors for vertices...
std::vector<float> CPrimitives::Cube(int sizex, int sizey, int sizez, CRectangle texa, CRectangle texb, CRectangle texc, CRectangle texd, CRectangle texe, CRectangle texf) {
	std::vector<float> vb;
	
	add(vb, 1, 0, 0);
	add(vb, 0, 0, 0);
	add(vb, 0, 1, 0);
	
	add(vb, 0, 1, 0);
	add(vb, 1, 1, 0);
	add(vb, 1, 0, 0);
	
	add(vb, 1, 0, 1);
	add(vb, 0, 1, 1);
	add(vb, 0, 0, 1);
	
	add(vb, 1, 0, 1);
	add(vb, 1, 1, 1);
	add(vb, 0, 1, 1);
	
	
	return vb;
}