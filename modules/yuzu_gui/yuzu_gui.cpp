// Copyright (c) 2024 Major Murphy

#ifdef YUZU_GUI_INCLUDED
 /* When you add this cpp file to your project, you mustn't include it in a file where you've
    already included any other headers - just put it inside a file on its own, possibly with your config
    flags preceding it, but don't include anything else. That also includes avoiding any automatic prefix
    header files that the compiler may be using.
 */
 #error "Incorrect use of Yuzu cpp file"
#endif

#include "yuzu_gui.h"



//==============================================================================
#include "clipboard/yuzu_Clipboard_windows.cpp"
#include "clipboard/yuzu_Clipboard_ios.mm"
#include "clipboard/yuzu_Clipboard_macos.mm"
#include "clipboard/yuzu_Clipboard_linux.cpp"
#include "clipboard/yuzu_Clipboard_android.cpp"
#include "photoviewer/yuzu_WebPhotoViewer.cpp"
#include "misc/yuzu_FileHelpers.cpp"

