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
			RGBA1010102,
			RGBAHalfFloat64
		};

		ExtendedImage() {};
		ExtendedImage(
			PixelFormat fmt,
			unsigned int width,
			unsigned int height,
			unsigned int bpp,
			unsigned int lineStride,
			juce::MemoryBlock data
		) : pixelFmt(fmt),
			width(width),
			height(height),
			bpp(bpp),
			lineStride(lineStride)
		{};
		~ExtendedImage() {};

		int getWidth() { return width; }
		int getHeight() { return height; }
		int getStride() { return lineStride; }
		void* getData() { return data.getData(); }

		PixelFormat getFormat() { return pixelFmt; }

		bool isValid();

	private:
		unsigned int width = 0;
		unsigned int height = 0;
		unsigned int lineStride = 0;
		unsigned int bpp = 0;
		PixelFormat pixelFmt = UnknownPixelFormat;

		juce::MemoryBlock data;
	};

}