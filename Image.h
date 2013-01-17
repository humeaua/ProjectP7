#ifndef IMAGE
#define IMAGE

#include <iostream>
#include <cstdio>
#include <vector>
#include <string>
#include <algorithm>

unsigned char myMean(std::vector<unsigned char> & cVect);
unsigned char myMean2(std::vector<unsigned char> & cVect);

class Image
{
private:
	std::vector<unsigned char> cData_;
	int	iWidth_;
	int	iHeight_;

	void load(const std::string & cFilename);
public: 
    Image();
	Image(const std::string & cFilename);
	Image(int iWidth, int iHeight);
	Image(const Image &img);
	virtual ~Image();
	virtual int getWidth();
	virtual int getHeight();
    
    virtual void Free();
    virtual void setWidth(const int iWidth);
    virtual void setHeight(const int iHeight);
    
    static std::vector<Image> GetFromFolder(const std::string & cFoldername);
    static void GetFromFolder(const std::string & cFoldername, std::vector<Image> & cLibrary);
    
    //virtual double Diff(Image & sImage);
    virtual double Diff(const Image & sImage) const;
    
    //virtual Image ChooseImage(std::vector<Image> & sLibrary) const;
    virtual Image ChooseImage(const std::vector<Image> & sLibrary) const;
    virtual Image ChooseImage(const std::string & cFolderName) const;
    
    virtual std::vector<std::vector<Image> > cutImage();
    
    //Image mergeImage(std::vector<std::vector<Image> >& elements, const std::string& cFolderName);
    Image mergeImage(const std::vector<std::vector<Image> >& elements, const std::string& cFolderName);
    //Image mergeImage(std::vector<std::vector<Image> >& elements, std::vector<Image> & sLibrary);
    Image mergeImage(const std::vector<std::vector<Image> >& elements, std::vector<Image> & sLibrary);
    
    Image Resize24();
    
	virtual void save(const std::string & filename);
    
	virtual void flipHorizontally();
    
    unsigned char & operator()(int x, int y, int i);
    const unsigned char & operator()(int x, int y, int i) const;
};

#endif
