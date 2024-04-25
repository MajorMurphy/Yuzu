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

    thumbnailPreview.reset (new juce::ImageComponent());
    addAndMakeVisible (thumbnailPreview.get());
    thumbnailPreview->setName ("thumbnail");

    metadataText.reset (new juce::TextEditor ("metadata"));
    addAndMakeVisible (metadataText.get());
    metadataText->setMultiLine (true);
    metadataText->setReturnKeyStartsNewLine (false);
    metadataText->setReadOnly (true);
    metadataText->setScrollbarsShown (true);
    metadataText->setCaretVisible (false);
    metadataText->setPopupMenuEnabled (true);
    metadataText->setText (juce::String());

    imageResolution.reset (new juce::Label ("image resolution",
                                            TRANS ("Primary Image: 1000 x 1000")));
    addAndMakeVisible (imageResolution.get());
    imageResolution->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    imageResolution->setJustificationType (juce::Justification::centredLeft);
    imageResolution->setEditable (false, false, false);
    imageResolution->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    imageResolution->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));

    thumbnailResolution.reset (new juce::Label ("thumbnail resolution",
                                                TRANS ("Thumbnail: 100 x 100")));
    addAndMakeVisible (thumbnailResolution.get());
    thumbnailResolution->setFont (juce::Font (15.00f, juce::Font::plain).withTypefaceStyle ("Regular"));
    thumbnailResolution->setJustificationType (juce::Justification::centredLeft);
    thumbnailResolution->setEditable (false, false, false);
    thumbnailResolution->setColour (juce::TextEditor::textColourId, juce::Colours::black);
    thumbnailResolution->setColour (juce::TextEditor::backgroundColourId, juce::Colour (0x00000000));


    //[UserPreSize]
    //[/UserPreSize]

    setSize (600, 400);


    //[Constructor] You can add your own custom stuff here..
    reload(juce::Image(), juce::String(), juce::Image());
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
    thumbnailPreview = nullptr;
    metadataText = nullptr;
    imageResolution = nullptr;
    thumbnailResolution = nullptr;


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

    imagePreview->setBounds (0, 160, getWidth() - 0, getHeight() - 164);
    thumbnailPreview->setBounds (getWidth() - 8 - 206, 32, 206, 120 - 0);
    metadataText->setBounds (120, 32, getWidth() - 343, 120);
    imageResolution->setBounds (120 + 0, 32 + -7 - 24, (getWidth() - 343) - 0, 24);
    thumbnailResolution->setBounds ((getWidth() - 8 - 206) + 0, 32 + -7 - 24, 206 - 0, 24);
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
                    String extentions("*.jpg;*.jpeg;*.png;*.gif;*.webp;*.bmp");
#if YUZU_LINK_LIBHEIF || JUCE_USING_COREIMAGE_LOADER
                    extentions += ";*.heic;*.heif";
#endif
                    ptr->chooser = std::make_unique<juce::FileChooser>("Open Image",
                        File::getSpecialLocation(File::SpecialLocationType::userPicturesDirectory),
                        extentions, true);
                    auto folderChooserFlags = FileBrowserComponent::openMode | FileBrowserComponent::FileChooserFlags::canSelectFiles;

                    ptr->chooser->launchAsync(folderChooserFlags, [ptr](const FileChooser& chooser)
                    {
                            if (chooser.getResult().existsAsFile())
                            {
                                ptr->setImage(chooser.getResult());
                            }
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
        setImage(yuzu::SystemClipboard::getImageFromClipboard());
        //[/UserButtonCode_pasteImageButton]
    }

    //[UserbuttonClicked_Post]
    //[/UserbuttonClicked_Post]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
void DemoComponent::setImage(juce::Image img)
{
    reload(img, juce::String(), juce::Image());
}
void DemoComponent::setImage(juce::File imgFile)
{
    auto fmt = ExtendedImageFileFormat::findImageFormatForFile(imgFile);
    if (fmt)
    {
        juce::OwnedArray<gin::ImageMetadata> md;
        fmt->loadMetadataFromImage(md);
        reload(fmt->decodeImage(), ImageMetadata::getAsString(md), fmt->decodeThumbnail());
    }
    else
    {
        jassertfalse;
    }
}
void DemoComponent::reload(juce::Image img, juce::String metadata, juce::Image thumbnail)
{
    imagePreview->setImage(img);
    metadataText->setText(metadata);
    thumbnailPreview->setImage(thumbnail);
    imageResolution->setText("Primary: " + String(img.getWidth()) + " x " + String(img.getHeight()), dontSendNotification);
    thumbnailResolution->setText("Thumbnail: " + String(thumbnail.getWidth()) + " x " + String(thumbnail.getHeight()), dontSendNotification);
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
                    virtualName="" explicitFocusOrder="0" pos="0 160 0M 164M" class="juce::ImageComponent"
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
  <GENERICCOMPONENT name="thumbnail" id="29e9dcfea34f1bba" memberName="thumbnailPreview"
                    virtualName="" explicitFocusOrder="0" pos="8Rr 32 206 0M" posRelativeH="5bdef0c0aaac48d6"
                    class="juce::ImageComponent" params=""/>
  <TEXTEDITOR name="metadata" id="5bdef0c0aaac48d6" memberName="metadataText"
              virtualName="" explicitFocusOrder="0" pos="120 32 343M 120" initialText=""
              multiline="1" retKeyStartsLine="0" readonly="1" scrollbars="1"
              caret="0" popupmenu="1"/>
  <LABEL name="image resolution" id="cede6fd8be4655d1" memberName="imageResolution"
         virtualName="" explicitFocusOrder="0" pos="0 -7r 0M 24" posRelativeX="5bdef0c0aaac48d6"
         posRelativeY="5bdef0c0aaac48d6" posRelativeW="5bdef0c0aaac48d6"
         edTextCol="ff000000" edBkgCol="0" labelText="Primary Image: 1000 x 1000"
         editableSingleClick="0" editableDoubleClick="0" focusDiscardsChanges="0"
         fontname="Default font" fontsize="15.0" kerning="0.0" bold="0"
         italic="0" justification="33"/>
  <LABEL name="thumbnail resolution" id="693cdf52278efb48" memberName="thumbnailResolution"
         virtualName="" explicitFocusOrder="0" pos="0 -7r 0M 24" posRelativeX="29e9dcfea34f1bba"
         posRelativeY="29e9dcfea34f1bba" posRelativeW="29e9dcfea34f1bba"
         posRelativeH="29e9dcfea34f1bba" edTextCol="ff000000" edBkgCol="0"
         labelText="Thumbnail: 100 x 100" editableSingleClick="0" editableDoubleClick="0"
         focusDiscardsChanges="0" fontname="Default font" fontsize="15.0"
         kerning="0.0" bold="0" italic="0" justification="33"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]

