// Copyright (c) 2024 Major Murphy

#include <JuceHeader.h>
#include "yuzu_ImageComponent.h"

#if JUCE_WINDOWS
#include <d2d1_1.h>
#include <Windows.Graphics.Display.Interop.h>
#include <winrt/Windows.Graphics.Display.h>
using namespace winrt::Windows::Graphics::Display;
#endif

void yuzu::ImageComponent::paint(juce::Graphics& g)
{
	if (hdr.isValid() 
		/* && displaySupportsHDR() */ 
		&& g.getInternalContext().supportsHDR()
		)
	{
		g.getInternalContext().drawImageHDR(hdr.getWidth(), hdr.getHeight(), hdr.getStride(), hdr.getData(), juce::AffineTransform());
	}
	
	g.drawImageWithin(sdr, 0,0, getWidth(), getHeight(), juce::RectanglePlacement::centred);
	
}
void yuzu::ImageComponent::setImage(juce::Image& newSDRImg, yuzu::ExtendedImage& newHDRImg)
{
	hdr = newHDRImg;
	sdr = newSDRImg;
	repaint();
}

bool yuzu::ImageComponent::displaySupportsHDR()
{
#if JUCE_WINDOWS
	try
	{
		auto factory{ winrt::get_activation_factory<DisplayInformation,	IDisplayInformationStaticsInterop>() };

		DisplayInformation displayInfo{ nullptr };

		auto window = dynamic_cast<juce::TopLevelWindow*>(getTopLevelComponent());
		if (!window)
			return false;
		auto hwnd = (HWND)window->getWindowHandle();

		winrt::check_hresult(
			factory->GetForWindow(
				hwnd,
				winrt::guid_of<DisplayInformation>(),
				winrt::put_abi(displayInfo)
			)
		);

		auto colorInfo{ displayInfo.GetAdvancedColorInfo() };

		/*
		displayInfo.AdvancedColorInfoChanged(
			[&](auto sender, auto args)
			{
				// Todo: handle change
			}
		*/

		return colorInfo.CurrentAdvancedColorKind() == winrt::Windows::Graphics::Display::AdvancedColorKind::HighDynamicRange;
	}
	catch (...)
	{
		jassertfalse;
		return false;
	}
#else
	return false;
#endif
}
