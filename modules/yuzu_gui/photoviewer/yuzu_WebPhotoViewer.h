
#pragma once

#include <JuceHeader.h>

namespace yuzu
{
    class WebPhotoViewer :
        public juce::WebBrowserComponent
    {
    public:


        WebPhotoViewer();
        ~WebPhotoViewer() override;

        void loadImage(juce::File imageFile);

    private:


        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(WebPhotoViewer)
    };
}