#ifndef IMAGE
#define IMAGE

#include <iostream>
#include <cstdio>
#include <vector>
#include <string>
#include <algorithm>

//  Function to compute the mean of an unsigned char vector
unsigned char myMean(std::vector<unsigned char> & cVect);

class Image
{
private:
	std::vector<unsigned char> cData_;
	int	iWidth_;
	int	iHeight_;

    //  Method to load a an image from its name
	void load(const std::string & cFilename);
public: 
    
    //  Constructors of Image
    Image();
	Image(const std::string & cFilename);
	Image(int iWidth, int iHeight);
	Image(const Image &img);
    //  Destructor of Image
	virtual ~Image();
    
    //  Methods to get the elements of the class
	virtual int getWidth();
	virtual int getHeight();
    
    virtual void Free();
    
    //  Methods to set elements of the class without construcing it
    virtual void setWidth(const int iWidth);
    virtual void setHeight(const int iHeight);
    
    //  Methods to load a library (two signatures depending on what we want to use)
    static std::vector<Image> GetFromFolder(const std::string & cFoldername);
    static void GetFromFolder(const std::string & cFoldername, std::vector<Image> & cLibrary);
    
    //  Method to compute the Mean Square Error between *this and sImage
    virtual double Diff(const Image & sImage) const;
    
    //  Methods to choose an Image in the library to minimize the mean square error
    //  Library can be already loaded (vector<Image>) or may need to be loaded (string)
    virtual Image ChooseImage(const std::vector<Image> & sLibrary) const;
    virtual Image ChooseImage(const std::string & cFolderName) const;
    
    //  Method to cut an image in square of 24 * 24
    virtual std::vector<std::vector<Image> > cutImage();
    
    //  Methods to merge the images in elements with the library (uses Choose image)
    //  This function has to be called after cutImage()
    virtual Image mergeImage(const std::vector<std::vector<Image> >& elements, const std::string& cFolderName);
    virtual Image mergeImage(const std::vector<std::vector<Image> >& elements, std::vector<Image> & sLibrary);
    
    //  Method to resize an image of size Height * Width into a 24 * 24 one
    virtual Image Resize24();
    
    //  Method to save the image in the file
	virtual void save(const std::string & filename);
    
    //  Method to flip an image
	virtual void flipHorizontally();
    
    //  Operators to access an element of the image
    unsigned char & operator()(int x, int y, int i);
    const unsigned char & operator()(int x, int y, int i) const;
};

#endif
