#include "Image.h"

#include "jpeglib.h"
#include <cstdlib>
#include <dirent.h>

unsigned char myMean2(std::vector<unsigned char> & cVect)
{
    long long lResult = 0;
    for (std::vector<unsigned char>::iterator it = cVect.begin() ; it != cVect.end() ; ++it)
    {
        lResult += *it;
    }
    lResult /= cVect.size();
    return (char)lResult;
}

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

const unsigned char & Image::operator()(int x, int y, int i) const
{
	return cData_[y*3*iWidth_ + x*3 + i];
}

std::vector<std::vector<Image> > Image::cutImage()
{
    Image element(24,24);
    std::vector<Image> lign(iHeight_ / 24, element);
    std::vector<std::vector<Image> > result(iWidth_ / 24, lign);
    
    for (int y = 0 ; y < iHeight_ / 24 ; ++y)
        for (int x = 0 ; x < iWidth_ / 24 ; ++x)
            for(int a = 0; a < 24; a ++)
                for(int b = 0; b < 24; b ++)
                    for(int c = 0; c < 3; c++)
                    {
                        result[x][y](a,b, c)=(*this)(24 * x + a, 24 * y + b, c);
                    }
    
    return result;
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
            if (sImage.getWidth() == 24 && sImage.getHeight() == 24)
            {
                sResult.push_back(sImage);
            }
            else
            {
                Image sImage24 = sImage.Resize24();
                sResult.push_back(sImage24);
            }
		}		
	}
    
	closedir(dp);
    return sResult;
}

void Image::GetFromFolder(const std::string & cFoldername, std::vector<Image> & cLibrary)
{
    cLibrary = GetFromFolder(cFoldername);
}

double Image::Diff(Image &img)
{
    double MSE = 0;
    
	try
	{
		if (img.iWidth_ != iWidth_ || img.iHeight_ != iHeight_)
		{
			char err=1;
			throw(err);
		}
		else
		{
			for (int i = 0; i < iWidth_; i ++)
            {
				for (int j = 0; j < iHeight_; j ++)
                {
					for (int c = 0; c < 3; c ++)
                    {
                        unsigned char cPixelthis = (*this)(i,j,c), cPixelImg = img(i,j,c);
						MSE += (cPixelthis - cPixelImg) * (cPixelthis - cPixelImg);
                    }
                }
            }
		}
	}
	catch(char err)
	{
		std::cout<<"Error, images don't have the same size"<<std::endl;
	}
    
	return MSE;
}

double Image::Diff(const Image &img) const
{
    double MSE = 0;
    
	try
	{
		if (img.iWidth_ != iWidth_ || img.iHeight_ != iHeight_)
		{
			char err=1;
			throw(err);
		}
		else
		{
			for (int i = 0; i < iWidth_; i ++)
            {
				for (int j = 0; j < iHeight_; j ++)
                {
					for (int c = 0; c < 3; c ++)
                    {
                        unsigned char cPixelthis = (*this)(i,j,c), cPixelImg = img(i,j,c);
						MSE += (cPixelthis - cPixelImg) * (cPixelthis - cPixelImg);
                    }
                }
            }
		}
	}
	catch(char err)
	{
		std::cout<<"Error, images don't have the same size"<<std::endl;
	}
    
	return MSE;
}

Image Image::ChooseImage(const std::string &cFolderName) const
{
    std::vector<Image> sLibrary = GetFromFolder(cFolderName);
    return ChooseImage(sLibrary);
}

Image Image::ChooseImage(std::vector<Image> &sLibrary) const
{
    std::vector<Image>::iterator iter = sLibrary.begin();
    Image sResultImage = *sLibrary.begin();
    double dDiff = Diff(sResultImage);
    iter++;
    while (iter != sLibrary.end()) 
    {
        double dDiffNew = Diff(*iter);
        if (dDiffNew < dDiff)
        {
            dDiff = dDiffNew;
            sResultImage = *iter;
        }
        //  Stopping condition : in case of the image is already a mosaic
        if (dDiffNew < 1)
        {
            break;
        }
        iter++;
    }
    return sResultImage;
}

Image Image::ChooseImage(const std::vector<Image> &sLibrary) const
{
    std::vector<Image>::const_iterator iter = sLibrary.begin();
    Image sResultImage = *sLibrary.begin();
    double dDiff = Diff(sResultImage);
    iter++;
    while (iter != sLibrary.end()) 
    {
        double dDiffNew = Diff(*iter);
        if (dDiffNew < dDiff)
        {
            dDiff = dDiffNew;
            sResultImage = *iter;
        }
        //  Stopping condition : in case of the image is already a mosaic
        if (dDiffNew < 1)
        {
            break;
        }
        iter++;
    }
    return sResultImage;
}

Image Image::mergeImage(std::vector<std::vector<Image> > & elements, const std::string &cFolderName)
{
	Image result ((int)elements[0].size() * 24, (int)elements.size() * 24);
	Image square(24,24);

	for(unsigned int x = 0; x < elements.size(); x ++)
		for(unsigned int y = 0; y < elements[0].size(); y ++)
		{
			square = elements[x][y].ChooseImage(cFolderName);
			for(int a = 0; a < 24; a ++)
				for(int b = 0; b < 24; b ++)
					for(int c = 0; c < 3; c++)
					{
						result(24 * x + a, 24 * y + b, c) = square(a,b,c);
					}
		}
	return result;
}

Image Image::mergeImage(const std::vector<std::vector<Image> > & elements, const std::string &cFolderName)
{
	Image result ((int)elements[0].size() * 24, (int)elements.size() * 24);
	Image square(24,24);
    
	for(unsigned int x = 0; x < elements.size(); x ++)
		for(unsigned int y = 0; y < elements[0].size(); y ++)
		{
			square = elements[x][y].ChooseImage(cFolderName);
			for(int a = 0; a < 24; a ++)
				for(int b = 0; b < 24; b ++)
					for(int c = 0; c < 3; c++)
					{
						result(24 * x + a, 24 * y + b, c) = square(a,b,c);
					}
		}
	return result;
}

Image Image::mergeImage(const std::vector<std::vector<Image> > & elements, std::vector<Image> & sLibrary)
{
    int iSizex = (int)elements.size() * 24, iSizey = (int)elements[0].size() * 24;
	Image result (iSizex, iSizey);
	Image square(24,24);
    
	for(unsigned int x = 0; x < elements.size(); x ++)
		for(unsigned int y = 0; y < elements[0].size(); y ++)
		{
			square = elements[x][y].ChooseImage(sLibrary);
			for(int a = 0; a < 24; a ++)
				for(int b = 0; b < 24; b ++)
					for(int c = 0; c < 3; c++)
					{
						result(24 * x + a, 24 * y + b, c) = square(a,b,c);
					}
		}
	return result;
}

Image Image::mergeImage(std::vector<std::vector<Image> > & elements, std::vector<Image> & sLibrary)
{
    int iSizex = (int)elements.size() * 24, iSizey = (int)elements[0].size() * 24;
	Image result (iSizex, iSizey);
	Image square(24,24);
    
	for(unsigned int x = 0; x < elements.size(); x ++)
		for(unsigned int y = 0; y < elements[0].size(); y ++)
		{
			square = elements[x][y].ChooseImage(sLibrary);
			for(int a = 0; a < 24; a ++)
				for(int b = 0; b < 24; b ++)
					for(int c = 0; c < 3; c++)
					{
						result(24 * x + a, 24 * y + b, c) = square(a,b,c);
					}
		}
	return result;
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

//  Function to resize an image of size iWidth * iHeight into a 24 * 24 one
Image Image::Resize24()
{
    Image sResult(24, 24);
    
    for (int a = 0 ; a < 24 ; ++a)
        for (int b = 0 ; b < 24 ; ++b)
            for (int c = 0; c < 3 ; ++c)
            {
                std::vector<unsigned char> cVect;
                for (int y = 0 ; y < iHeight_ / 24 ; ++y)
                    for (int x = 0 ; x < iWidth_ / 24 ; ++x)
                        cVect.push_back((*this)(a * iWidth_ / 24 + x, b * iHeight_ / 24 + y,c));
                //sResult(a,b,c) = myMean(cVect);
                sResult(a,b,c) = myMean2(cVect);
            }
    return sResult;
}
