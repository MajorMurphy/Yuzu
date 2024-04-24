#pragma once

namespace yuzu
{
	class SystemClipboard
	{
	public:
		/** Retrieve binary image from clipboard **/
		static juce::Image getImageFromClipboard();

		/** Place binary image on clipboard **/
		static void copyImageToClipboard(juce::Image image);

	private:
		JUCE_LEAK_DETECTOR(SystemClipboard)
	};
}