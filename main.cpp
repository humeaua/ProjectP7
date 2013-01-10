#include <iostream>
#include <cstdio>
#include "image.h"
#include <dirent.h>
#include <sstream>
#include <cmath>

void rotateAllImagesInFolder(const std::string & inputfolder, const std::string  & outputfolder);
void rotateAllImagesInFolder(const std::string & inputfolder, const std::string & outputfolder)
{
	DIR *dp;
	struct dirent *dirp;

	if((dp  = opendir(inputfolder.c_str())) == NULL) 
	{
        std::cout << "Error opening " << inputfolder << std::endl;
        	return;
	}

	while ((dirp = readdir(dp)) != NULL) 
	{
        std::string s(dirp->d_name);
		if(s.length() > 4 && strcmp(".DS_Store", s.c_str()))
		{
            std::cout << inputfolder + s << "... ";
            std::cout.flush();
			Image i(inputfolder + s);
			i.flipHorizontally();
			i.save(outputfolder + s);
            std::cout << "done." << std::endl;
		}		
	}
    
	closedir(dp);
}


int main(int argc, char *argv[])
{
	//rotateAllImagesInFolder("/Users/alexhum49/Documents/Workspace/ProjectP7/inputImages/", "/Users/alexhum49/Documents/Workspace/ProjectP7/outputImages/");
    Image sImage("/Users/alexhum49/Documents/Workspace/ProjectP7/inputImages/2012.jpg");
    
    //int iNbCuts = 9;
    //VectorImage sImageArray(iNbCuts);
    //sImage.CutImage(iNbCuts, sImageArray);
    
    //for (std::size_t i = 0 ; i < iNbCuts ; ++i)
    //{
    //    std::stringstream out;
    //    out << i;
    //    sImageArray(i).save("/Users/alexhum49/Documents/Workspace/ProjectP7/outputImages/2012_" + out.str() + ".jpg");
    //}
    Image sResizedImage(200,285);
    sResizedImage = sImage.Resize(285);
    
    sResizedImage.save("/Users/alexhum49/Documents/Workspace/ProjectP7/outputImages/2012_Resized.jpg");
    
	return 0;
}
