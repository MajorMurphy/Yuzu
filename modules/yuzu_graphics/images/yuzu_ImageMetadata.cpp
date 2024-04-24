#include "yuzu_ImageMetadata.h"

#if YUZU_LINK_LIBHEIF
#include <libheif/heif.h>
#endif

using namespace juce;
namespace yuzu
{

	bool yuzu::ImageMetadata::getFromImage(juce::InputStream& is, juce::OwnedArray<gin::ImageMetadata>& metadata)
	{
        if (gin::ImageMetadata::getFromImage(is, metadata))
            return true;


        yuzu::HEIFImageFormat heif;

        is.setPosition(0);
        if (heif.canUnderstand(is))
        {
            is.setPosition(0);
            
            return heif.loadMetadataFromImage(is, metadata);
        }

        return false;
    }

}