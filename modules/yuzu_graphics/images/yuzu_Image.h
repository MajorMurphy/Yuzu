// Copyright (c) 2024 Major Murphy

#pragma once

namespace yuzu
{
	class Image
	{
	public:
		Image() {};
		Image(juce::Image img) : primary(img) {};
		juce::Image getSDR() { return primary; }

		bool isHDR();

		int getWidth() { return primary.getWidth(); }
		int getHeight() { return primary.getHeight(); }
		bool isValid();

	private:
		juce::Image primary;
		juce::Image gainMap;
	};

}