#ifndef IMAGE
#define IMAGE

#include <iostream>
#include <cstdio>

class Image
{
private:
	unsigned char*	cData_;
	int	iWidth_;
	int	iHeight_;

	void load(const std::string & cFilename);
public: 
    Image();
	Image(const std::string & cFilename);
	Image(int iWidth, int iHeight);
	Image(Image &img);
	virtual ~Image();
	virtual int getWidth();
	virtual int getHeight();
    
    virtual void ResizeandInitialize(int iWidth, int iHeight);

	virtual void save(const std::string & filename);

	virtual void flipHorizontally();
    virtual void CutImage(const int iHowMuchCuts, Image sListOfNewImage[]);
	unsigned char & operator()(int x, int y, int i);
};

#endif
