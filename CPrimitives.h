//
//  CPrimitives.h
//  osxtest
//
//  Created by Alexander on 09/06/14.
//  Copyright (c) 2014 Alexander. All rights reserved.
//

#ifndef __osxtest__CPrimitives__
#define __osxtest__CPrimitives__

#include <iostream>
#include <vector>

#include "CTypes.h"

class CPrimitives {
public:
	static std::vector<float> Cube (int sizex, int sizey, int sizez, CRectangle texa, CRectangle texb, CRectangle texc, CRectangle texd, CRectangle texe, CRectangle texf);
};

#endif /* defined(__osxtest__CPrimitives__) */
