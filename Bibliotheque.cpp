//
//  Bibliotheque.cpp
//  ProjectP7
//
//  Created by Alexandre HUMEAU on 21/12/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include <iostream>
#include "Bibliotheque.h"
#include <dirent.h>

/*Bibliotheque::Bibliotheque() : VectorImage()
{}

Bibliotheque::~Bibliotheque()
{}

void Bibliotheque::GetFromFolder(const std::string & cFoldername)
{
    DIR *dp;
	struct dirent *dirp;
    
	if((dp  = opendir(cFoldername.c_str())) == NULL) 
	{
        std::cout << "Error opening " << cFoldername << std::endl;
        return;
	}
    
	while ((dirp = readdir(dp)) != NULL) 
	{
        std::string s(dirp->d_name);
		if(s.length() > 4 && strcmp(".DS_Store", s.c_str()))
		{
            Image sImage(cFoldername + s);
            (*this).add(sImage);
		}		
	}
    
	closedir(dp);
}*/
