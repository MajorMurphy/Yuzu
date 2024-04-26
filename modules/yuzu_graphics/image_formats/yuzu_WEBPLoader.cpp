// Copyright (c) 2024 Major Murphy

#include "../images/yuzu_ImageFileFormat.h"


using namespace yuzu;
using namespace juce;


WEBPImageExtendedFormat::WEBPImageExtendedFormat(juce::InputStream& is) : ExtendedImageFileFormat(is)
{

}

yuzu::WEBPImageExtendedFormat::~WEBPImageExtendedFormat()
{

}

juce::String yuzu::WEBPImageExtendedFormat::getFormatName()
{
	gin::WEBPImageFormat fmt;
	return fmt.getFormatName();;
}

bool yuzu::WEBPImageExtendedFormat::usesFileExtension(const juce::File& file)
{
	gin::WEBPImageFormat fmt;
	return fmt.usesFileExtension(file);
}

bool yuzu::WEBPImageExtendedFormat::canUnderstand(juce::InputStream& is)
{
	gin::WEBPImageFormat fmt;
	return fmt.canUnderstand(is);
}

juce::Image yuzu::WEBPImageExtendedFormat::decodeImage()
{
	MemoryInputStream s(rawFileData, false);
	return fmt.decodeImage(s);
}

bool yuzu::WEBPImageExtendedFormat::loadMetadataFromImage(juce::OwnedArray<gin::ImageMetadata>&)
{
	return false;
}
