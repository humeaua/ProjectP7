#ifndef IMAGE
#define IMAGE

#include <iostream>
#include <cstdio>
#include <vector>

//#include <tr1/memory>

class VectorImage;

class Image
{
private:
	//unsigned char*	cData_;
    std::vector<unsigned char> cData_;
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
    
    virtual void Free();
    virtual void setWidth(const int iWidth);
    virtual void setHeight(const int iHeight);
    
    
	virtual void save(const std::string & filename);
    
    //  return an image with the mean colors of (*this)
    //virtual void MeanColors(char * cResult); 
    
    //virtual Image Resize(const int& iNewWidth);

	virtual void flipHorizontally();
    //virtual void CutImage(const int iHowMuchCuts, VectorImage & sListOfNewImage);
    unsigned char & operator()(int x, int y, int i);
    
};

/*class VectorImage
{
    // In CPP, you have to create a DYNAMIC array of DYNAMIC objects.
    // Otherwise, TA will deduct 35 points.
private:
    std::size_t iCapacity_; //capacity of the current array
    std::size_t iSize_;     // number of current records
    Image* sList_;
    //std::tr1::shared_ptr<Image> sList_;
    
public:
    VectorImage();
    VectorImage(std::size_t iCapacity);
    virtual ~VectorImage();
    //virtual void Alloc();
    virtual std::size_t getCapacity();
    std::size_t getSize();
    Image& get(std::size_t index);
    void print();
    void add(Image & newImage); 
    int remove(int productID);
    
    Image & operator()(std::size_t iIndex);
};*/

#endif
