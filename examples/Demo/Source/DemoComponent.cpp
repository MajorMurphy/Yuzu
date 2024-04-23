/*
  ==============================================================================

  This is an automatically generated GUI class created by the Projucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Projucer version: 7.0.12

  ------------------------------------------------------------------------------

  The Projucer is part of the JUCE library.
  Copyright (c) 2020 - Raw Material Software Limited.

  ==============================================================================
*/

//[Headers] You can add your own extra header files here...
//[/Headers]

#include "DemoComponent.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
using namespace yuzu;
using namespace juce;
//[/MiscUserDefs]

//==============================================================================
DemoComponent::DemoComponent ()
{
    //[Constructor_pre] You can add your own custom stuff here..
    //[/Constructor_pre]

    imagePreview.reset (new juce::ImageComponent());
    addAndMakeVisible (imagePreview.get());
    imagePreview->setName ("image preview");

    openImageButton.reset (new juce::TextButton ("open image button"));
    addAndMakeVisible (openImageButton.get());
    openImageButton->setButtonText (TRANS ("Open"));
    openImageButton->addListener (this);

    openImageButton->setBounds (8, 8, 94, 24);

    copyImageButton.reset (new juce::TextButton ("copy image button"));
    addAndMakeVisible (copyImageButton.get());
    copyImageButton->setButtonText (TRANS ("Copy"));
    copyImageButton->addListener (this);

    copyImageButton->setBounds (8, 48, 94, 24);

    pasteImageButton.reset (new juce::TextButton ("paste image button"));
    addAndMakeVisible (pasteImageButton.get());
    pasteImageButton->setButtonText (TRANS ("Paste"));
    pasteImageButton->addListener (this);

    pasteImageButton->setBounds (8, 88, 94, 24);


    //[UserPreSize]
    //[/UserPreSize]

    setSize (600, 400);


    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

DemoComponent::~DemoComponent()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    imagePreview = nullptr;
    openImageButton = nullptr;
    copyImageButton = nullptr;
    pasteImageButton = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void DemoComponent::paint (juce::Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (juce::Colour (0xff323e44));

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void DemoComponent::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    imagePreview->setBounds (0, 0, getWidth() - 0, getHeight() - 4);
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void DemoComponent::buttonClicked (juce::Button* buttonThatWasClicked)
{
    //[UserbuttonClicked_Pre]
    //[/UserbuttonClicked_Pre]

    if (buttonThatWasClicked == openImageButton.get())
    {
        //[UserButtonCode_openImageButton] -- add your button handler code here..
            // check permissions
        RuntimePermissions::request(RuntimePermissions::readMediaImages,
            [ptr = DemoComponent::SafePointer(this)](bool granted)
            {
                if (!ptr)
                    return;
                else if (!granted)
                {
                    NativeMessageBox::showMessageBoxAsync(MessageBoxIconType::WarningIcon, "Sorry", "Permission needed to open photos.");
                }
                else
                {
                    ptr->chooser = std::make_unique<juce::FileChooser>("Open Image",
                        File::getSpecialLocation(File::SpecialLocationType::userPicturesDirectory),
                        "*.jpg;*.jpeg;*.png;*.gif", 
                        true);
                    auto folderChooserFlags = FileBrowserComponent::openMode | FileBrowserComponent::FileChooserFlags::canSelectFiles;


                    ptr->chooser->launchAsync(folderChooserFlags, [ptr](const FileChooser& chooser) 
                    {
                        if (chooser.getResult().existsAsFile())
                            ptr->setImage(ImageFileFormat::loadFrom(chooser.getResult()));
                    });
                    
                }
            });
        //[/UserButtonCode_openImageButton]
    }
    else if (buttonThatWasClicked == copyImageButton.get())
    {
        //[UserButtonCode_copyImageButton] -- add your button handler code here..
        yuzu::SystemClipboard::copyImageToClipboard(imagePreview->getImage());
        //[/UserButtonCode_copyImageButton]
    }
    else if (buttonThatWasClicked == pasteImageButton.get())
    {
        //[UserButtonCode_pasteImageButton] -- add your button handler code here..
        imagePreview->setImage(yuzu::SystemClipboard::getImageFromClipboard());
        //[/UserButtonCode_pasteImageButton]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
void DemoComponent::setImage(juce::Image img)
{
    imagePreview->setImage(img);
}
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Projucer information section --

    This is where the Projucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="DemoComponent" componentName=""
                 parentClasses="public juce::Component" constructorParams="" variableInitialisers=""
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330"
                 fixedSize="0" initialWidth="600" initialHeight="400">
  <BACKGROUND backgroundColour="ff323e44"/>
  <GENERICCOMPONENT name="image preview" id="bf43439ea64874c4" memberName="imagePreview"
                    virtualName="" explicitFocusOrder="0" pos="0 0 0M 4M" class="juce::ImageComponent"
                    params=""/>
  <TEXTBUTTON name="open image button" id="e4a534ade34f0d32" memberName="openImageButton"
              virtualName="" explicitFocusOrder="0" pos="8 8 94 24" buttonText="Open"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="copy image button" id="6ceff1450967837d" memberName="copyImageButton"
              virtualName="" explicitFocusOrder="0" pos="8 48 94 24" buttonText="Copy"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
  <TEXTBUTTON name="paste image button" id="4f9326e9c2294d2b" memberName="pasteImageButton"
              virtualName="" explicitFocusOrder="0" pos="8 88 94 24" buttonText="Paste"
              connectedEdges="0" needsCallback="1" radioGroupId="0"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

