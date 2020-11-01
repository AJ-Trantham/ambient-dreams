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
    addAndMakeVisible(frequencySlider);
    frequencySlider.setRange(50, 5000.0);         
    frequencySlider.setTextValueSuffix(" Hz");     
    frequencySlider.setValue(500.0);
    frequencySlider.addListener(this); 
    //frequencySlider.addListener(p); we need to add
    
    addAndMakeVisible(roomSizeSlider);
    roomSizeSlider.setRange(0,1.0);
    roomSizeSlider.setTextValueSuffix(" %");
    roomSizeSlider.setValue(.5);
    roomSizeSlider.addListener(this);
    addAndMakeVisible(roomSizeLabel);
    roomSizeLabel.setText("Room Size", juce::dontSendNotification);
    roomSizeLabel.attachToComponent(&roomSizeSlider, true); // [4
    
    addAndMakeVisible(wetLevelSlider);
    wetLevelSlider.setRange(0,1.0);
    wetLevelSlider.setTextValueSuffix(" %");
    wetLevelSlider.setValue(.5);
    wetLevelSlider.addListener(this);
    addAndMakeVisible(wetLevelLabel);
    wetLevelLabel.setText("Wet Level", juce::dontSendNotification);
    wetLevelLabel.attachToComponent(&wetLevelSlider, true);
    
    addAndMakeVisible(dryLevelSlider);
    dryLevelSlider.setRange(0,1.0);
    dryLevelSlider.setTextValueSuffix(" %");
    dryLevelSlider.setValue(.5);
    dryLevelSlider.addListener(this);
    addAndMakeVisible(dryLevelLabel);
    dryLevelLabel.setText("Dry Level", juce::dontSendNotification);
    dryLevelLabel.attachToComponent(&dryLevelSlider, true);
    
    addAndMakeVisible(dampingSlider);
    dampingSlider.setRange(0,1.0);
    dampingSlider.setTextValueSuffix(" %");
    dampingSlider.setValue(.5);
    dampingSlider.addListener(this);
    addAndMakeVisible(dampingLabel);
    dampingLabel.setText("Damping ", juce::dontSendNotification);
    dampingLabel.attachToComponent(&dampingSlider, true);
    
    addAndMakeVisible(frequencyLabel);
    frequencyLabel.setText("Frequency", juce::dontSendNotification);
    frequencyLabel.attachToComponent(&frequencySlider, true); // [4]
    
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
    
//juce::Image bgImage;
    bgImage = juce::ImageCache::getFromMemory(BinaryData::nightsky_jpg, BinaryData::nightsky_jpgSize);
    g.drawImage(bgImage, 0, 0, getWidth(), getHeight(), 0, 0, bgImage.getWidth(), bgImage.getHeight());
    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
    g.drawText("Noisier", 425, 500, 75, 20, juce::Justification::centredRight);
    //g.drawFittedText ("Noisier", getLocalBounds(), juce::Justification::centred, 1);
    g.setColour (juce::Colours::white);
    // | (60) 150 (60) 150 (60) 150 (60) 150 (60) |
    g.fillRect(60, 150, 150, 300);
    g.fillRect(270, 150, 150, 300);
    g.fillRect(490, 150, 150, 300);
    g.fillRect(700, 150, 150, 300);
}

void NewProjectAudioProcessorEditor::resized()
{
    juce::Rectangle<int> area = getLocalBounds().reduced(40);
    keyMenu.setBounds(area.removeFromTop(20));
    
    auto sliderLeft = 120;
    frequencySlider.setBounds(sliderLeft, 20, getWidth() - sliderLeft - 10, 20);
    durationSlider.setBounds(sliderLeft, 50, getWidth() - sliderLeft - 10, 20);
    roomSizeSlider.setBounds(sliderLeft, 80, getWidth() - sliderLeft - 10, 20);
    wetLevelSlider.setBounds(sliderLeft, 120, getWidth() - sliderLeft - 300, 20);
    dryLevelSlider.setBounds(sliderLeft, 150, getWidth() - sliderLeft - 300, 20);
    dampingSlider.setBounds(sliderLeft, 180, getWidth() - sliderLeft - 300, 20);
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}

