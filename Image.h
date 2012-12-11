#ifndef IMAGE
#define IMAGE

#include <iostream>
#include <cstdio>

using namespace std;

class Image
{
private:
	unsigned char*	data;
	int	width;
	int	height;

	void load(string filename);
public: 
	Image(string filename);
	Image(int width, int height);
	Image(Image &img);
	~Image();
	int getWidth();
	int getHeight();

	void save(string filename);

	void flipHorizontally();

	unsigned char & operator()(int x, int y, int i);
};

#endif
