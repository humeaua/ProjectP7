//
//  Image.h
//  ProjectP7
//
//  Created by Alexandre HUMEAU on 03/12/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef ProjectP7_Image_h
#define ProjectP7_Image_h

#include <vector>

class Pixel {
protected:
    int iRed_, iBlue_, iGreen_;
    
public:
    Pixel();
    Pixel(const int iRed, const int iBlue, const int iGreen);
    
    virtual ~Pixel();
    
};

class Image {
protected:
    std::vector<Pixel> sImage_;
    
    
public:
    Image();
    virtual ~Image();
    
};

#endif
