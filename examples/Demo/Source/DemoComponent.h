/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 7.0.11

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library.
  Copyright (c) 2020 - Raw Material Software Limited.

  ==============================================================================
*/

#pragma once

//[Headers]     -- You can add your own extra header files here --
#include <JuceHeader.h>
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Projucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class DemoComponent  : public juce::Component,
                       public juce::Button::Listener
{
public:
    //==============================================================================
    DemoComponent ();
    ~DemoComponent() override;

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    void setImage(juce::Image img);
    void setImage(juce::File imgFile);
    void reload(juce::Image img, juce::String metadata, juce::Image thumbnail);
    //[/UserMethods]

    void paint (juce::Graphics& g) override;
    void resized() override;
    void buttonClicked (juce::Button* buttonThatWasClicked) override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    std::unique_ptr<juce::FileChooser> chooser;
    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<juce::ImageComponent> imagePreview;
    std::unique_ptr<juce::TextButton> openImageButton;
    std::unique_ptr<juce::TextButton> copyImageButton;
    std::unique_ptr<juce::TextButton> pasteImageButton;
    std::unique_ptr<juce::ImageComponent> thumbnailPreview;
    std::unique_ptr<juce::TextEditor> metadataText;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DemoComponent)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

