// Copyright (c) 2024 Major Murphy

#include "yuzu_Image.h"


bool yuzu::ExtendedImage::isValid()
{ 
    if (width<=0)
        return false;
    if (height <= 0)
        return false;
    if (lineStride <= 0)
        return false;
    if (bpp <= 0)
        return false;
    if (pixelFmt == UnknownPixelFormat)
        return false;

    return true;
}
