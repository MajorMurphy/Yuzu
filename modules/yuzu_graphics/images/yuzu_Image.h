// Copyright (c) 2024 Major Murphy

#pragma once

namespace yuzu
{
	class ExtendedImage
	{
	public:
		enum PixelFormat
		{
			UnknownPixelFormat,
			RGBA1010102
		};

		ExtendedImage() {};
		~ExtendedImage() {};

		int getWidth() { return width; }
		int getHeight() { return height; }
		bool isValid();

	private:
		int width = 0;
		int height = 0;
		int lineStride = 0;
		int bpp = 0;
		PixelFormat pixelFmt = UnknownPixelFormat;

		juce::MemoryBlock data;
	};

}