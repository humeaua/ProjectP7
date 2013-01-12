#include "Image.h"

#include "jpeglib.h"
#include <cstdlib>
#include <cmath>
#include <stdexcept>
#include <dirent.h>

Image::Image() : iWidth_(0), iHeight_(0)
{}

Image::Image(int width, int height)
{
	this->iWidth_ = width;
	this->iHeight_ = height;
	//cData_ = new unsigned char[3 * iWidth_ * iHeight_];
    cData_.reserve(3 * iWidth_ * iHeight_);
    cData_.resize(3 * iWidth_ * iHeight_);
	for(int i=0; i < 3*width*height; i ++)
    {
		cData_[i] = 0;
    }
}

Image::Image(const std::string& cFilename)
{
	load(cFilename);
}

Image::~Image()
{
	//delete cData_;
    cData_.clear();
}

Image::Image(const Image &img)
{
	this->iWidth_ = img.iWidth_;
	this->iHeight_ = img.iHeight_;
	//this->cData_ = new unsigned char[3*iWidth_*iHeight_];
    this->cData_ = img.cData_;
	//for(int i=0; i < 3*iWidth_*iHeight_; i ++)
    //{
	//	this->cData_[i] = img.cData_[i];
    //}
}

void Image::Free()
{
    cData_.clear();
}

int Image::getWidth()
{
	return iWidth_;
}

int Image::getHeight()
{
	return iHeight_;
}

void Image::setHeight(const int iHeight)
{
    iHeight_ = iHeight;
}

void Image::setWidth(const int iWidth)
{
    iWidth_ = iWidth;
}

void Image::load(const std::string & cFilename)
{
	FILE *infile;
    jpeg_decompress_struct cinfo;
    jpeg_error_mgr jerr;
	JSAMPARRAY buffer;

    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_decompress(&cinfo);

    if ((infile = fopen(cFilename.c_str(), "rb")) == NULL) 
	{
        std::cout << "Can't open " << cFilename << std::endl;
        exit(EXIT_FAILURE);
    }

	jpeg_stdio_src(&cinfo, infile);
	jpeg_read_header(&cinfo, TRUE);

	iWidth_ = cinfo.image_width;
	iHeight_ = cinfo.image_height;

	//cData_ = new unsigned char[3*iWidth_*iHeight_];
    cData_.resize(3 * iWidth_ * iHeight_);

	//if(cData_ == NULL)
	//{
    //    std::cout << "Error in Memory allocation. " << std::endl;
	//	exit(EXIT_FAILURE);
	//}

	jpeg_start_decompress(&cinfo);

    buffer = (*cinfo.mem->alloc_sarray) ((j_common_ptr) &cinfo, JPOOL_IMAGE, iWidth_*3, 1);

	while (cinfo.output_scanline < cinfo.output_height) 
	{
		jpeg_read_scanlines(&cinfo, buffer, 1);

		for(int j=0; j < 3*iWidth_; j++)
        {
			cData_[(cinfo.output_scanline-1)*3*iWidth_ + j] = (*buffer)[j];
        }
	}
	
	jpeg_finish_decompress(&cinfo);
	jpeg_destroy_decompress(&cinfo);

	fclose(infile);
}

void Image::save(const std::string& cFilename)
{
	FILE *outfile;
    jpeg_compress_struct cinfo;
    jpeg_error_mgr jerr;
	JSAMPROW row_pointer;          

	if((outfile = fopen(cFilename.c_str(), "wb")) == NULL)
	{
        std::cout << "Can't open " << cFilename << " for saving image." << std::endl;
		exit(EXIT_FAILURE);
    }

 
	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_compress(&cinfo);
	jpeg_stdio_dest(&cinfo, outfile);
  
	cinfo.image_width      = iWidth_;
	cinfo.image_height     = iHeight_;
	cinfo.input_components = 3;
	cinfo.in_color_space   = JCS_RGB;

	jpeg_set_defaults(&cinfo);
	jpeg_set_quality (&cinfo, 96, true);	  /*set the quality [0..100]  */
	jpeg_start_compress(&cinfo, true);

  
	while (cinfo.next_scanline < cinfo.image_height) 
	{
		row_pointer = (JSAMPROW) &cData_[cinfo.next_scanline*iWidth_*3];
		jpeg_write_scanlines(&cinfo, &row_pointer, 1);
	}

	jpeg_finish_compress(&cinfo);
	fclose(outfile);
}

unsigned char & Image::operator()(int x, int y, int i)
{
	return cData_[y*3*iWidth_ + x*3 + i];
}

std::vector<Image> Image::GetFromFolder(const std::string & cFoldername)
{
    std::vector<Image> sResult;
    DIR *dp;
	struct dirent *dirp;
    
	if((dp  = opendir(cFoldername.c_str())) == NULL) 
	{
        throw "Error opening " + cFoldername;
	}
    
	while ((dirp = readdir(dp)) != NULL) 
	{
        std::string s(dirp->d_name);
		if(s.length() > 4 && strcmp(".DS_Store", s.c_str()))
		{
            Image sImage(cFoldername + s);
            sResult.push_back(sImage);
		}		
	}
    
	closedir(dp);
    return sResult;
}

void Image::GetFromFolder(const std::string & cFoldername, std::vector<Image> & cLibrary)
{
    cLibrary = GetFromFolder(cFoldername);
}

void Image::flipHorizontally()
{
	for(int y=0; y < iHeight_ / 2; y ++)
    {
		for(int x = 0; x < iWidth_; x ++)
        {
			for(int c =0; c < 3; c++)
			{
				int d = (*this)(x,y, c);
				(*this)(x,y, c) = (*this)(x,iHeight_-y-1, c);
				(*this)(x,iHeight_-y-1, c) = d;
			}
        }
    }
}

/*void Image::CutImage(const int iHowMuchCuts, VectorImage & sListOfNewImage)
{
    //  Let us test if iHowMuchCuts is a square
    double dSqrtCuts = sqrt(iHowMuchCuts);
    if (dSqrtCuts - (int)floor(dSqrtCuts) != 0)
    {
        std::cout << "Number of cuts is not a square" << std::endl;
        std::cout << "Not yet implemented" << std::endl;
    }
    else
    {
        int iCut = dSqrtCuts;
        int iNewHeight = iHeight_ / iCut, iNewWidth = iWidth_ / iCut;
        
        for (int i = 0 ; i < iCut ; ++i)
        {
            for (int j = 0 ; j < iCut ; ++j)
            {
                Image * pImage = new Image(iNewWidth, iNewHeight);
                
                for (int iPixel = 0 ; iPixel < iNewHeight ; ++iPixel)
                {
                    for (int jPixel = 0 ; jPixel < iNewWidth ; ++jPixel)
                    {
                        for (int c = 0 ; c < 3 ; ++c)
                        {
                            (*pImage)(jPixel, iPixel, c) = (*this)(jPixel + j * iNewWidth,iPixel + i * iNewHeight, c);
                        }
                    }
                }
                sListOfNewImage.add(*pImage);
                //  Free memory
                delete pImage;
            }
        }
    }
}

void Image::MeanColors(char* cResult)
{
    Image sResult(1,1);
    long lResult[3] = {0, 0, 0};
    for (int iPixel = 0 ; iPixel < iWidth_ ; ++iPixel)
    {
        for (int jPixel = 0 ; jPixel < iHeight_ ; ++jPixel)
        {
            for (int c = 0 ; c < 3 ; ++c)
            {
                lResult[c] += (*this)(iPixel, jPixel, c);
            }
        }
    }
    for (int c = 0 ; c < 3 ; ++c)
    {
        lResult[c] /= (iWidth_ * iHeight_);
    }
    for (int c = 0 ; c < 3 ; ++c)
    {
        cResult[c] = (char)lResult[c];
    }
}

Image Image::Resize(const int& iNewWidth)
{
    if (iNewWidth > (*this).iWidth_)
    {
        std::cout << "Not yet implemented" << std::endl;
        throw std::runtime_error::runtime_error("Height and Width have to be below Width and Height for now");
    }
    else if ((*this).iWidth_ % iNewWidth != 0)
    {
        std::cout << "Not yet implemented" << std::endl;
        throw std::runtime_error::runtime_error("New Width and New Height of image have to be divisors of Height and Width");
    }
    else
    {
        int iCut = iWidth_ / iNewWidth, iNbCuts = iCut * iCut;
        Image sResultImage(iNewWidth, iHeight_ / iCut);
        
        //  only case implemented for now
        VectorImage sListOfImage(iNbCuts), sListOfImage2(iNbCuts);
        this->CutImage(iNbCuts, sListOfImage);
        for (int i = 0 ; i < iNbCuts ; ++i)
        {
            Image sImage = sListOfImage(i);
            char cResult[3];
            sImage.MeanColors(cResult); // cResult is an array of size 3
            int iRemainder = i % iCut, iQuotient = i % iCut;
            
            for (int c = 0 ; c < 3 ; ++c)
            {
                sResultImage(iQuotient, iRemainder, c) = cResult[c];
            }
            
            //delete cResult;
        }
        return sResultImage;
    }
    Image sImage;
    return sImage;
}

VectorImage::VectorImage() : iCapacity_(1), iSize_(1)
{}

VectorImage::VectorImage(std::size_t iCapacity) : iCapacity_(iCapacity), iSize_(0)
{}

VectorImage::~VectorImage()
{
    if (sList_)
    {
        for( std::size_t iSize = 0 ; iSize < iSize_ ; ++iSize)
        {
            sList_[iSize].Free();
        }
    }
}

std::size_t VectorImage::getCapacity()
{
    return iCapacity_;
}

std::size_t VectorImage::getSize()
{
    return iSize_;
}

Image& VectorImage::get(std::size_t iIndex)
{
    if (iIndex <  iSize_)
    {
        return sList_[iIndex];
    }
    else
    {
        throw std::runtime_error::runtime_error("Cannot access to the desired element");
    }
}

void VectorImage::add(Image & newImage)
{
    if (iSize_ < iCapacity_)
    {
        Image* sTmpList = new Image[iSize_ + 1];
        for (int i = 0 ; i < iSize_ ; i++)
        {
            sTmpList[i] = sList_[i];
        }
        sList_ = sTmpList;
        sList_[iSize_] = newImage;
    }
    else
    {
        iCapacity_ *= 2;
        Image* tmpList = new Image[iCapacity_];
        for (int i = 0 ; i < iSize_ ; ++i)
        {
            tmpList[i] = sList_[i];
        }
        tmpList[iSize_] = newImage;
        sList_ = tmpList;
    }
    iSize_++;
}

int VectorImage::remove(int iElmt)
{
    if (iElmt > iSize_ - 1)
    {
        throw std::runtime_error::runtime_error("Try to delete a non-existing element");
    }
    
    for(int i=iElmt; i<iSize_-1; i++)
    {
        sList_[i] = sList_[i+1];
    }
    iSize_--;
    // SHRINK
    if( iSize_*2 == iCapacity_ )
    {
        iCapacity_ = iCapacity_/2;
        Image* tmpList = new Image[iCapacity_];
        for(int i=0; i<iSize_; i++)
        {
            tmpList[i] =  sList_[i];
        }
        sList_ = tmpList;
        iSize_ = iCapacity_;
    }
    return 0;
}

Image & VectorImage::operator()(std::size_t iIndex)
{
    return get(iIndex);
}*/