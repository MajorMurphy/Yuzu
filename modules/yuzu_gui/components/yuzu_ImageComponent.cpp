// Copyright (c) 2024 Major Murphy

#include <JuceHeader.h>
#include "yuzu_ImageComponent.h"

void yuzu::ImageComponent::paint(juce::Graphics& g)
{
	if (img.isHDR())
	{
		jassertfalse;
	}
	else
	{
		g.drawImageWithin(img.getSDR(), 0,0, getWidth(), getHeight(), juce::RectanglePlacement::centred);
	}
}

void yuzu::ImageComponent::setImage(yuzu::Image& newImg) 
{ 
	img = newImg; 
	repaint();
}
