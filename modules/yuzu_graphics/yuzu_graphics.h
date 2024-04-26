


/*******************************************************************************
 The block below describes the properties of this module, and is read by
 the Projucer to automatically generate project code that uses it.
 For details about the syntax and how to create or use a module, see the
 JUCE Module Format.md file.


 BEGIN_JUCE_MODULE_DECLARATION

  ID:                 yuzu_graphics
  vendor:             Major Murphy
  version:            0.0.1
  name:               Yuzu image classes
  description:        image classes
  website:            http://www.juce.com/juce
  license:            GPL/Commercial
  minimumCppStandard: 17

  dependencies:       juce_graphics gin_graphics gin_metadata gin_webp
  OSXFrameworks:      
  iOSFrameworks:      

 END_JUCE_MODULE_DECLARATION

*******************************************************************************/


#pragma once
#define YUZU_GRAPHICS_INCLUDED

#include <juce_graphics/juce_graphics.h>
#include <gin_graphics/gin_graphics.h>
#include <gin_metadata/gin_metadata.h>
#include <gin_webp/gin_webp.h>

//==============================================================================
/** Config: YUZU_LINK_LIBHEIF

    This will enable support for loading HEIF images by linking to libheif (and libde265 and/or libx265)
*/
#ifndef YUZU_LINK_LIBHEIF
 #define YUZU_LINK_LIBHEIF 0
#endif


//==============================================================================
#include "images/yuzu_Image.h"
#include "images/yuzu_ImageFileFormat.h"
#include "images/yuzu_ImageMetadata.h"