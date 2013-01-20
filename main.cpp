#include <iostream>
#include <cstdio>
#include "image.h"
#include <dirent.h>
#include <sstream>
#include <cmath>
#include <time.h>

void rotateAllImagesInFolder(const std::string&, const std::string&);
void mosaiqueAllImagesInFolder(std::string&, std::string&, std::string&);
void mosaiqueAllImagesInFolder(std::string&, std::string&, std::vector<Image> &);

int main(int argc, char *argv[])
{
	clock_t start = clock();
    std::string cLibraryInputName, cInputFolderName, cOutputFolderName;
    
    std::cout << "Enter the Library folder name  :" << std::endl;
    std::cin >> cLibraryInputName;
    
    std::cout << "Enter the Input Image folder name : " << std::endl;
    std::cin >> cInputFolderName;
    
    std::cout << "Enter the Output Image folder name : " << std::endl;
    std::cin >> cOutputFolderName;
    
    std::vector<Image> sLibrary;
    std::cout << "Loading Library" << std::endl;
    Image::GetFromFolder(cLibraryInputName, sLibrary);
    std::cout << "End of Loading Library" << std::endl;
    std::cout << "Time elapsed : " << (double)(clock() - start) / CLOCKS_PER_SEC << " seconds." << std::endl;
    
    mosaiqueAllImagesInFolder(cInputFolderName, cOutputFolderName, sLibrary);
    std::cout << "Total time elapsed : " << (double)(clock() - start) / CLOCKS_PER_SEC << " seconds." << std::endl;
    sLibrary.clear();
    return 0;
}

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

void mosaiqueAllImagesInFolder(std::string& inputfolder, std::string& outputfolder, std::string& libraryfolder)
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
			Image j((i.getWidth() / 24) * 24, (i.getHeight() / 24) * 24);
            std::vector<std::vector<Image> > sVectofImage = i.cutImage();
			j = i.mergeImage(sVectofImage, libraryfolder);
			j.save(outputfolder + s);
			std::cout << "done." << std::endl;
		}		
	}

	closedir(dp);

}

void mosaiqueAllImagesInFolder(std::string& inputfolder, std::string& outputfolder, std::vector<Image> & sLibrary)
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
			Image j((i.getWidth() / 24) * 24, (i.getHeight() / 24) * 24);
            std::vector<std::vector<Image> > sVectofImage = i.cutImage();
			j = i.mergeImage(sVectofImage, sLibrary);
			j.save(outputfolder + s);
			std::cout << "done." << std::endl;
		}		
	}
    
	closedir(dp);
    
}
