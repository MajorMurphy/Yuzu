// Copyright (c) 2024 Major Murphy

#include "yuzu_Clipboard.h"

#if JUCE_ANDROID
//==============================================================================
using namespace yuzu;
using namespace juce;

Image yuzu::SystemClipboard::getImageFromClipboard()
{
	// not yet implemented
	jassertfalse;
	return Image();
}

void yuzu::SystemClipboard::copyImageToClipboard(Image)
{
	// not yet implemented
	jassertfalse;
}
#endif