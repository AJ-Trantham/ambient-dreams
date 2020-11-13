/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
NewProjectAudioProcessorEditor::NewProjectAudioProcessorEditor (VoltronAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{ 
    addAndMakeVisible(roomSizeSlider);
    roomSizeSlider.setSliderStyle(Slider::SliderStyle::Rotary);
    roomSizeSlider.setRange(0,1.0);
    roomSizeSlider.setTextValueSuffix(" %");
    roomSizeSlider.setValue(.5);
    roomSizeSlider.addListener(this);
    roomSizeSlider.setTextBoxStyle(Slider::NoTextBox,true, 60, 15);
    roomSizeSlider.setPopupDisplayEnabled( true,false,NULL,2000);
    roomSizeSlider.setNumDecimalPlacesToDisplay(3);
    roomSizeLabel.setJustificationType(Justification::centredBottom);
    roomSizeLabel.setText("Room Size", juce::dontSendNotification);
    roomSizeLabel.attachToComponent(&roomSizeSlider, false);
    
     addAndMakeVisible(wetLevelSlider);
     wetLevelSlider.setRange(0,1.0);
     wetLevelSlider.setTextValueSuffix(" %");
     wetLevelSlider.setSliderStyle(Slider::SliderStyle::Rotary);
     wetLevelSlider.setValue(.5);
     wetLevelSlider.setTextBoxStyle(Slider::NoTextBox,true, 60, 15);
     wetLevelSlider.setPopupDisplayEnabled( true,false,NULL,2000);
     wetLevelSlider.setNumDecimalPlacesToDisplay(3);
     wetLevelSlider.addListener(this);
    
    
     //wetLevelSlider.setColour(c,juce::Colours::white );
     wetLevelLabel.setJustificationType(Justification::centredBottom);
     wetLevelLabel.setText("Wet Level", juce::dontSendNotification);
     wetLevelLabel.attachToComponent(&wetLevelSlider, false);
    
     addAndMakeVisible(dryLevelSlider);
     dryLevelSlider.setSliderStyle(Slider::SliderStyle::Rotary);
     dryLevelSlider.setRange(0,1.0);
     dryLevelSlider.setTextValueSuffix(" %");
     dryLevelSlider.setValue(.5);
     dryLevelSlider.setTextBoxStyle(Slider::NoTextBox,true, 60, 15);
     dryLevelSlider.setPopupDisplayEnabled( true,false,NULL,2000);
     dryLevelSlider.setNumDecimalPlacesToDisplay(3);
     dryLevelSlider.addListener(this);
     dryLevelLabel.setJustificationType(Justification::centredBottom);
     dryLevelLabel.setText("Dry Level", juce::dontSendNotification);
     dryLevelLabel.attachToComponent(&dryLevelSlider, false);
    
    addAndMakeVisible(dampingSlider);
    dampingSlider.setSliderStyle(Slider::SliderStyle::Rotary);
    dampingSlider.setRange(0,1.0);
    dampingSlider.setTextValueSuffix(" %");
    dampingSlider.setValue(.5);
    dampingSlider.setTextBoxStyle(Slider::NoTextBox,true, 60, 15);
    dampingSlider.setPopupDisplayEnabled( true,false,NULL,2000);
    dampingSlider.setNumDecimalPlacesToDisplay(3);
    dampingSlider.addListener(this);
    addAndMakeVisible(dampingLabel);
    dampingLabel.setJustificationType(Justification::centredBottom);
    dampingLabel.setText("Damping", juce::dontSendNotification);
    dampingLabel.attachToComponent(&dampingSlider, false);
    
    keyMenu.addItem ("A", 'A');
    keyMenu.addItem ("A#", 'a');
    keyMenu.addItem ("B", 'B');
    keyMenu.addItem ("C", 'C');
    keyMenu.addItem ("C#", 'c');
    keyMenu.addItem ("D", 'D');
    keyMenu.addItem ("D#", 'd');
    keyMenu.addItem ("E", 'E');
    keyMenu.addItem ("F", 'F');
    keyMenu.addItem ("F#", 'f');
    keyMenu.addItem ("G", 'G');
    keyMenu.addItem ("G#", 'g');
    
    keyMenu.onChange = [this] { styleMenuChanged(); };
    keyMenu.setSelectedId (1);
    keyMenu.setJustificationType(Justification::centred);
    
    addAndMakeVisible(&keyMenu);



    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (900, 600);
}

NewProjectAudioProcessorEditor::~NewProjectAudioProcessorEditor()
{
}

//==============================================================================
void NewProjectAudioProcessorEditor::paint (juce::Graphics& g)
{
   
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
    
    juce::Image bgImage;
    bgImage = juce::ImageCache::getFromMemory(BinaryData::nightsky_jpg, BinaryData::nightsky_jpgSize);
    g.drawImage(bgImage, 0, 0, getWidth(), getHeight(), 0, 0, bgImage.getWidth(), bgImage.getHeight());
    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
    g.drawText("Noisier", 425, 500, 75, 20, juce::Justification::centredRight);
    //g.drawFittedText ("Noisier", getLocalBounds(), juce::Justification::centred, 1);
    g.setColour (juce::Colours::black);
    g.setOpacity(.5);
    // | (60) 150 (60) 150 (60) 150 (60) 150 (60) |
    g.fillRect(60, 150, 150, 300);
    g.fillRect(270, 150, 150, 300);
    g.fillRect(490, 150, 150, 300);
    g.fillRect(700, 150, 150, 300);
    g.setColour (juce::Colours::white);
    g.drawRect(60, 150, 150, 300);
    g.drawRect(270, 150, 150, 300);
    g.drawRect(490, 150, 150, 300);
    g.drawRect(700, 150, 150, 300);
    g.setFont (25.0f);
    g.drawText("Reverb", 660, 55, 150,150, juce::Justification::centredRight);
}

void NewProjectAudioProcessorEditor::resized()
{
    juce::Rectangle<int> area = getLocalBounds().reduced(40);
    keyMenu.setBounds(area.removeFromTop(20));
    auto sliderLeft = 120;
    durationSlider.setBounds(sliderLeft, 50, getWidth() - sliderLeft - 10, 20);
    dryLevelSlider.setBoundsRelative(0.81, 0.285, 0.1, 0.1);
    wetLevelSlider.setBoundsRelative(0.81, 0.405, 0.1, 0.1);
    dampingSlider.setBoundsRelative(0.81, 0.525, 0.1, 0.1);
    roomSizeSlider.setBoundsRelative(0.81, 0.645, 0.1, 0.1);
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}

