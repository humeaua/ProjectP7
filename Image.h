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
	Image(const std::string & cFilename);
	Image(int iWidth, int iHeight);
	Image(Image &img);
	virtual ~Image();
	int getWidth();
	int getHeight();

	void save(const std::string & filename);

	void flipHorizontally();

	unsigned char & operator()(int x, int y, int i);
};

#endif
