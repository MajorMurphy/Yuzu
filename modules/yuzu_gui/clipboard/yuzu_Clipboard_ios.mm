#include "yuzu_Clipboard.h"

#if JUCE_IOS
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