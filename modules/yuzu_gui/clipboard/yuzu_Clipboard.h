// Copyright (c) 2024 Major Murphy

#pragma once

namespace yuzu
{
	class SystemClipboard
	{
	public:
		/** Checks for binary image on clipboard **/
		static bool checkForImage();

		/** Retrieve binary image from clipboard **/
		static juce::Image pasteImage();

		/** Place binary image on clipboard **/
		static void copyImage(juce::Image image);

	private:
		JUCE_LEAK_DETECTOR(SystemClipboard)
	};
}