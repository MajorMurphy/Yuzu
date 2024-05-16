// Copyright (c) 2024 Major Murphy

#include <JuceHeader.h>
#include "yuzu_ImageComponent.h"

void yuzu::ImageComponent::paint(juce::Graphics& g)
{
	if (/*display supports HDR && */ hdr.isValid())
	{
		jassertfalse;
	}
	else
	{
		g.drawImageWithin(sdr, 0,0, getWidth(), getHeight(), juce::RectanglePlacement::centred);
	}
}
void yuzu::ImageComponent::setImage(juce::Image& newSDRImg, yuzu::ExtendedImage& newHDRImg)
{
	hdr = newHDRImg;
	sdr = newSDRImg;
	repaint();
}
