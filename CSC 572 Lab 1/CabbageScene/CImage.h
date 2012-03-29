#ifndef _CABBAGE_SCENE_CIMAGE_H_INCLUDED_
#define _CABBAGE_SCENE_CIMAGE_H_INCLUDED_

#include <string>

#ifdef __unix__
#include<GL/gl.h>
#endif

#ifdef _WIN32
#include <GL\glew.h>
#endif

#include "../CabbageCore/SVector2.h"

class CImage
{

    friend class CImageLoader;

    char * ImageData;
    int Width;
    int Height;

	bool Alpha;

    CImage(char * imageData, int width, int height, bool const alpha = false);

public:

    ~CImage();

    int const getWidth() const;
    int const getHeight() const;
    char const * const getImageData() const;
	bool const hasAlpha() const;

};

#endif
