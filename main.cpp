#include <iostream>
#include <cstdio>
#include "image.h"
#include <dirent.h>
#include <sstream>
#include <cmath>
#include <time.h>

void rotateAllImagesInFolder(const std::string & inputfolder, const std::string & outputfolder);
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

void mosaiqueAllImagesInFolder(std::string& inputfolder, std::string& outputfolder, std::string& libraryfolder);
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
		if(s.length() > 4)
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

void mosaiqueAllImagesInFolder(std::string& inputfolder, std::string& outputfolder, std::vector<Image> & sLibrary);
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
		if(s.length() > 4)
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


int main(int argc, char *argv[])
{
	//rotateAllImagesInFolder("/Users/alexhum49/Documents/Workspace/ProjectP7/inputImages/", "/Users/alexhum49/Documents/Workspace/ProjectP7/outputImages/");
    //Image sImage("/Users/alexhum49/Documents/Workspace/ProjectP7/inputImages/2012.jpg");
    clock_t start = clock();
    std::string cLibraryInputName = "/Users/alexhum49/Documents/Workspace/ProjectP7/LibraryinputImages/",
                cLibraryOutputName = "/Users/alexhum49/Documents/Workspace/ProjectP7/LibraryImages/",
                cInputFolderName = "/Users/alexhum49/Documents/Workspace/ProjectP7/inputImages/",
                cOutputFolderName = "/Users/alexhum49/Documents/Workspace/ProjectP7/outputImages/";
    //std::vector<Image> sVectImage = Image::GetFromFolder(cFolderName);
    std::vector<Image> sLibrary;
    std::cout << "Loading Library" << std::endl;
    Image::GetFromFolder(cLibraryInputName, sLibrary);
    std::cout << "End of Loading Library" << std::endl;
    std::cout << "Time elapsed : " << (double)(clock() - start) / CLOCKS_PER_SEC << std::endl;
    
    /*std::cout << "Saving Library" << std::endl;
    for (std::size_t i = 0 ; i < sLibrary.size() ; ++i)
    {
        std::stringstream ss;
        ss << i;
        std::string cOut = ss.str();
        
        sLibrary[i].save(cLibraryOutputName + cOut + ".jpg");
    }
    std::cout << "Library saved" << std::endl;*/
    mosaiqueAllImagesInFolder(cInputFolderName, cOutputFolderName, sLibrary);
    //int iNbCuts = 9;
    //VectorImage sImageArray(iNbCuts);
    //sImage.CutImage(iNbCuts, sImageArray);
    
    //for (std::size_t i = 0 ; i < iNbCuts ; ++i)
    //{
    //    std::stringstream out;
    //    out << i;
    //    sImageArray(i).save("/Users/alexhum49/Documents/Workspace/ProjectP7/outputImages/2012_" + out.str() + ".jpg");
    //}
    //Image sResizedImage(200,285);
    //sResizedImage = sImage.Resize(285);
    
    //sResizedImage.save("/Users/alexhum49/Documents/Workspace/ProjectP7/outputImages/2012_Resized.jpg");
    std::cout << "Total time elapsed : " << (double)(clock() - start) / CLOCKS_PER_SEC << " seconds."<< std::endl;
    sLibrary.clear();
	return 0;
}
