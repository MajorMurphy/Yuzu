// Copyright (c) 2024 Major Murphy

#include "yuzu_Image.h"

bool yuzu::Image::isHDR()
{
    return gainMap.isValid();
}

bool yuzu::Image::isValid() 
{ 
    if (!primary.isValid())
        return false;
    if (gainMap.isValid())
    {
        if (gainMap.getHeight() != primary.getHeight())
            return false;
        if (gainMap.getWidth() != primary.getWidth())
            return false;
    }
    return true;
}
