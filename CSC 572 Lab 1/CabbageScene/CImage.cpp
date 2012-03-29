#include "CImage.h"

CImage::CImage(char * const imageData, int const width, int const height, bool const alpha)
    : ImageData(imageData), Width(width), Height(height), Alpha(alpha)
{}

CImage::~CImage()
{
    delete ImageData;
}

int const CImage::getWidth() const
{
    return Width;
}

int const CImage::getHeight() const
{
    return Height;
}

char const * const CImage::getImageData() const
{
    return ImageData;
}

bool const CImage::hasAlpha() const
{
	return Alpha;
}