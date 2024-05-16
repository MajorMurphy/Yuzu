// Copyright (c) 2024 Major Murphy


/*******************************************************************************
 The block below describes the properties of this module, and is read by
 the Projucer to automatically generate project code that uses it.
 For details about the syntax and how to create or use a module, see the
 JUCE Module Format.md file.


 BEGIN_JUCE_MODULE_DECLARATION

  ID:                 yuzu_gui
  vendor:             Major Murphy
  version:            0.0.2
  name:               Yuzu GUI classes
  description:        gui classes
  website:            https://github.com/majorMurphy/Yuzu
  license:            BSD
  minimumCppStandard: 17

  dependencies:       juce_gui_basics yuzu_graphics
  OSXFrameworks:      
  iOSFrameworks:      

 END_JUCE_MODULE_DECLARATION

*******************************************************************************/


#pragma once
#define YUZU_GUI_INCLUDED

#include <juce_gui_basics/juce_gui_basics.h>
#include <yuzu_graphics/yuzu_graphics.h>

//==============================================================================
#include "clipboard/yuzu_Clipboard.h"
#include "components/yuzu_ImageComponent.h"