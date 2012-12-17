#include <iostream>
#include <cstdio>
#include "image.h"
#include <dirent.h>

void rotateAllImagesInFolder(std::string inputfolder, std::string outputfolder);
void rotateAllImagesInFolder(std::string inputfolder, std::string outputfolder)
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
	rotateAllImagesInFolder("/Users/alexhum49/Documents/Workspace/ProjectP7/inputImages/", "/Users/alexhum49/Documents/Workspace/ProjectP7/outputImages/");
	return 0;
}
