// Copyright (c) 2024 Major Murphy

#include "../images/yuzu_ImageFileFormat.h"


using namespace yuzu;
using namespace juce;



PNGImageExtendedFormat::PNGImageExtendedFormat(juce::InputStream& is) : ExtendedImageFileFormat(is)
{

}

yuzu::PNGImageExtendedFormat::~PNGImageExtendedFormat()
{

}

juce::String yuzu::PNGImageExtendedFormat::getFormatName()
{
	PNGImageFormat fmt;
	return fmt.getFormatName();;
}

bool yuzu::PNGImageExtendedFormat::usesFileExtension(const juce::File& file)
{
	PNGImageFormat fmt;
	return fmt.usesFileExtension(file);
}

bool yuzu::PNGImageExtendedFormat::canUnderstand(juce::InputStream& is)
{
	PNGImageFormat fmt;
	return fmt.canUnderstand(is);
}

juce::Image yuzu::PNGImageExtendedFormat::decodeImage()
{
	MemoryInputStream s(rawFileData, false);
	return fmt.decodeImage(s);
}

