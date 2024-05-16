// Copyright (c) 2024 Major Murphy

#pragma once
#include <yuzu_graphics/images/yuzu_Image.h>

namespace yuzu
{
	class ImageComponent : public juce::Component
	{
	public:
		ImageComponent() {};
		ImageComponent(juce::Image sdr, yuzu::ExtendedImage hdr = yuzu::ExtendedImage()) : sdr(sdr), hdr(hdr) {}
		~ImageComponent()=default;

		void paint(juce::Graphics& g) override;

		void setImage(juce::Image& newSDRImg, yuzu::ExtendedImage& newHDRImg);
		juce::Image getImage() { return sdr; }

		bool displaySupportsHDR();

	private:
		yuzu::ExtendedImage hdr;
		juce::Image sdr;
	};
}