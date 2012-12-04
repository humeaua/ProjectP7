//
//  Image.cpp
//  ProjectP7
//
//  Created by Alexandre HUMEAU on 03/12/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include <iostream>
#include "Image.h"

Pixel::Pixel() : iRed_(0), iBlue_(0), iGreen_(0)
{}

Pixel::Pixel(const int iRed, const int iBlue, const int iGreen) : iRed_(iRed), iBlue_(iBlue), iGreen_(iGreen)
{}

Pixel::~Pixel()
{}

Image::Image()
{}

Image::~Image()
{
    sImage_.clear();
}