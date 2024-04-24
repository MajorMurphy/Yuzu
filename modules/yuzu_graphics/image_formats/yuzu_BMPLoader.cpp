#include "../images/yuzu_ImageFileFormat.h"


using namespace yuzu;
using namespace juce;

BMPImageExtendedFormat::BMPImageExtendedFormat(juce::InputStream& is) : ExtendedImageFileFormat(is)
{

}

yuzu::BMPImageExtendedFormat::~BMPImageExtendedFormat()
{

}

juce::String yuzu::BMPImageExtendedFormat::getFormatName()
{
	gin::BMPImageFormat fmt;
	return fmt.getFormatName();;
}

bool yuzu::BMPImageExtendedFormat::usesFileExtension(const juce::File& file)
{
	gin::BMPImageFormat fmt;
	return fmt.usesFileExtension(file);
}

bool yuzu::BMPImageExtendedFormat::canUnderstand(juce::InputStream& is)
{
	gin::BMPImageFormat fmt;
	return fmt.canUnderstand(is);
}

juce::Image yuzu::BMPImageExtendedFormat::decodeImage()
{
	MemoryInputStream s(rawFileData, false);
	return fmt.decodeImage(s);
}

bool yuzu::BMPImageExtendedFormat::loadMetadataFromImage(juce::OwnedArray<gin::ImageMetadata>&)
{
	return false;
}
