// Copyright (c) 2024 Major Murphy

#include "../images/yuzu_ImageFileFormat.h"


using namespace yuzu;
using namespace juce;


GIFImageExtendedFormat::GIFImageExtendedFormat(juce::InputStream& is) : ExtendedImageFileFormat(is)
{

}

yuzu::GIFImageExtendedFormat::~GIFImageExtendedFormat()
{

}

juce::String yuzu::GIFImageExtendedFormat::getFormatName()
{
	GIFImageFormat fmt;
	return fmt.getFormatName();;
}

bool yuzu::GIFImageExtendedFormat::usesFileExtension(const juce::File& file)
{
	GIFImageFormat fmt;
	return fmt.usesFileExtension(file);
}

bool yuzu::GIFImageExtendedFormat::canUnderstand(juce::InputStream& is)
{
	GIFImageFormat fmt;
	return fmt.canUnderstand(is);
}

juce::Image yuzu::GIFImageExtendedFormat::decodeImage()
{
	MemoryInputStream s(rawFileData, false);
	return fmt.decodeImage(s);
}

bool yuzu::GIFImageExtendedFormat::loadMetadataFromImage(juce::OwnedArray<gin::ImageMetadata>& metadata)
{
	MemoryInputStream s(rawFileData, false);
	return gin::ImageMetadata::getFromImage(s, metadata);
}
