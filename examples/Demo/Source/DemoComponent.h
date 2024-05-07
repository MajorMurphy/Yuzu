/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 8.0.0

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library.
  Copyright (c) - Raw Material Software Limited.

  ==============================================================================
*/

#pragma once

//[Headers]     -- You can add your own extra header files here --

// Copyright (c) 2024 Major Murphy

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
                       public juce::ApplicationCommandTarget,
                       public juce::MenuBarModel
{
public:
    //==============================================================================
    DemoComponent ();
    ~DemoComponent() override;

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    void setImage(juce::Image img);
    void setImage(juce::File imgFile);
    void reload(juce::Image img, juce::StringPairArray metadata, juce::Image thumbnail, int motionPhotoSize);

    void browseForImage();
    void exportVideo();
    void copyImageToClipboard();
    void pasteImageFromClipboard();
    void about();


    enum CommandIDs
    {
        openImageFileCmd = 1,
        exportVideoCmd,
        copyImageToClipboardCmd,
        pasteImageFromClipboardCmd,
        aboutCmd
    };
    juce::StringArray getMenuBarNames() override;
    void getAllCommands(juce::Array<juce::CommandID>& c) override;
    juce::PopupMenu getMenuForIndex(int menuIndex, const juce::String& /*menuName*/) override;
    void getCommandInfo(juce::CommandID commandID, juce::ApplicationCommandInfo& result) override;
    bool perform(const InvocationInfo& info) override;
    juce::ApplicationCommandTarget* getNextCommandTarget() override { return nullptr; }
    void menuItemSelected(int /*menuItemID*/, int /*topLevelMenuIndex*/) override {}
    //[/UserMethods]

    void paint (juce::Graphics& g) override;
    void resized() override;



private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    std::unique_ptr<juce::FileChooser> chooser;
    std::unique_ptr<yuzu::ExtendedImageFileFormat> fmt;
    juce::ApplicationCommandManager commandManager;
    std::unique_ptr<juce::MenuBarComponent> menuBar;
    //[/UserVariables]

    //==============================================================================
    std::unique_ptr<juce::ImageComponent> imagePreview;
    std::unique_ptr<juce::ImageComponent> thumbnailPreview;
    std::unique_ptr<juce::TextEditor> metadataText;
    std::unique_ptr<juce::Label> imageResolution;
    std::unique_ptr<juce::Label> thumbnailResolution;
    std::unique_ptr<juce::Label> motionPhotoLabel;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DemoComponent)
};

//[EndFile] You can add extra defines here...
//[/EndFile]

