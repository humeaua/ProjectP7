#include "image.h"

#include "jpeglib.h"
#include <cstdlib>


Image::Image(int width, int height)
{
	this->iWidth_ = width;
	this->iHeight_ = height;
	cData_ = new unsigned char[3 * iWidth_ * iHeight_];
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
	delete cData_;
}

Image::Image(Image &img)
{
	this->iWidth_ = img.iWidth_;
	this->iHeight_ = img.iHeight_;
	this->cData_ = new unsigned char[3*iWidth_*iHeight_];
	for(int i=0; i < 3*iWidth_*iHeight_; i ++)
    {
		this->cData_[i] = img.cData_[i];
    }
}

int Image::getWidth()
{
	return iWidth_;
}

int Image::getHeight()
{
	return iHeight_;
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

	cData_ = new unsigned char[3*iWidth_*iHeight_];

	if(cData_ == NULL)
	{
        std::cout << "Error in Memory allocation. " << std::endl;
		exit(EXIT_FAILURE);
	}

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
