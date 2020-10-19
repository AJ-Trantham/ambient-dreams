/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "../JuceLibraryCode/JuceHeader.h"


//==============================================================================
HelloWorldAudioProcessorEditor::HelloWorldAudioProcessorEditor (HelloWorldAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (900, 600);
}

HelloWorldAudioProcessorEditor::~HelloWorldAudioProcessorEditor()
{
}

//==============================================================================
void HelloWorldAudioProcessorEditor::paint (juce::Graphics& g)
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
    g.setColour (juce::Colours::white);
    // | (60) 150 (60) 150 (60) 150 (60) 150 (60) |
    g.fillRect(60, 150, 150, 300);
    g.fillRect(270, 150, 150, 300);
    g.fillRect(490, 150, 150, 300);
    g.fillRect(700, 150, 150, 300);
}

void HelloWorldAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}
