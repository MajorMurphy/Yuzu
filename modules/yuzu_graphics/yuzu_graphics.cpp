// Copyright (c) 2024 Major Murphy

#ifdef YUZU_GRAPHICS_INCLUDED
 /* When you add this cpp file to your project, you mustn't include it in a file where you've
    already included any other headers - just put it inside a file on its own, possibly with your config
    flags preceding it, but don't include anything else. That also includes avoiding any automatic prefix
    header files that the compiler may be using.
 */
 #error "Incorrect use of Yuzu cpp file"
#endif

#include "yuzu_graphics.h"



//==============================================================================
#include "images/yuzu_Image.cpp"
#include "images/yuzu_ImageFileFormat.cpp"
#include "images/yuzu_ImageMetadata.cpp"
#include "image_formats/yuzu_HEIFLoader.cpp"
#include "image_formats/yuzu_JPEGLoader.cpp"
#include "image_formats/yuzu_PNGLoader.cpp"
#include "image_formats/yuzu_GIFLoader.cpp"
#include "image_formats/yuzu_BMPLoader.cpp"
#include "image_formats/yuzu_WEBPLoader.cpp"