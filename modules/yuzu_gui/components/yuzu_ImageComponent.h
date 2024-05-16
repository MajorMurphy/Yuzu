// Copyright (c) 2024 Major Murphy

#pragma once
#include <yuzu_graphics/images/yuzu_Image.h>

namespace yuzu
{
	class ImageComponent : public juce::Component
	{
	public:
		ImageComponent() {};
		ImageComponent(yuzu::Image img) : img(img) {}
		~ImageComponent()=default;

		void paint(juce::Graphics& g) override;

		void setImage(yuzu::Image& newImg);
		yuzu::Image getImage() { return img; }

	private:
		yuzu::Image img;
	};
}