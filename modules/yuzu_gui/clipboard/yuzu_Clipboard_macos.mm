#include "yuzu_Clipboard.h"

#if JUCE_MAC
//==============================================================================
using namespace yuzu;
using namespace juce;

bool yuzu::SystemClipboard::checkForImage()
{
    // not yet implemented
    return false;
}

Image yuzu::SystemClipboard::pasteImage()
{
	// not yet implemented
	jassertfalse;
	return Image();
}

void yuzu::SystemClipboard::copyImage(Image)
{
	// not yet implemented
	jassertfalse;
}
#endif
