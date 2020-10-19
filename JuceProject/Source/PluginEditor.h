/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class NewProjectAudioProcessorEditor  : public juce::AudioProcessorEditor, public juce::Slider::Listener
{
public:
    NewProjectAudioProcessorEditor (VoltronAudioProcessor&);
    ~NewProjectAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

    juce::Slider frequencySlider;
    juce::Label  frequencyLabel;
    juce::Slider durationSlider;
    juce::Label  durationLabel;

    void sliderValueChanged(juce::Slider* slider) override //this can't access toneGenRoot which is an issue. not sure how to actually pass the slider value back and forth
    {
        frequencySlider.setValue(frequencySlider.getValue(), juce::dontSendNotification);
    }


private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    VoltronAudioProcessor &audioProcessor;



    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (NewProjectAudioProcessorEditor)
};
