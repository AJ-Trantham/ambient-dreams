/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "OnOffButton.h"

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

    juce::Slider roomSizeSlider;
    juce::Label  roomSizeLabel;
    juce::Slider wetLevelSlider;
    juce::Label  wetLevelLabel;
    juce::Slider dampingSlider;
    juce::Label  dampingLabel;
    juce::Slider dryLevelSlider;
    juce::Label  dryLevelLabel;
    juce::Slider durationSlider;
    juce::Label  durationLabel;
    
    juce::Slider delayLengthSlider;
    juce::Label  delayLengthLabel;
    juce::Slider gainStartSlider;
    juce::Label  gainStarthLabel;
    
    void sliderValueChanged(juce::Slider* slider) override
    {
        audioProcessor.rSize = roomSizeSlider.getValue();
        audioProcessor.wet = wetLevelSlider.getValue();
        audioProcessor.dry = dryLevelSlider.getValue();
        audioProcessor.damping = dampingSlider.getValue();
        audioProcessor.delayLength = delayLengthSlider.getValue();
        audioProcessor.startGain= gainStartSlider.getValue();
     }
    
    void styleMenuChanged()
    {
        float freq;
        switch (keyMenu.getSelectedId())
        {
            case 'A':
                freq = 440.0000;
                break;
            case 'a':
                freq = 466.1638;
                break;
            case 'B':
                freq = 493.8833;
                break;
            case 'C':
                freq = 261.6256;
                break;
            case 'c':
                freq = 277.1826;
                break;
            case 'D':
                freq = 293.6648;
                break;
            case 'd':
                freq = 311.1270;
                break;
            case 'E':
                freq = 329.6276;
                break;
            case 'F':
                freq = 349.2282;
                break;
            case 'f':
                freq = 369.9944;
                break;
            case 'G':
                freq = 391.9954;
                break;
            case 'g':
                freq = 415.3047;
                break;
            default:
                break;
        }
        audioProcessor.rootFrequencyValue = freq;
    }

    void pluginOnOffSwitchToggled()
    {
        audioProcessor.pluginOnOffState = pluginOnOff.getToggleState();
    }

    void reverbOnOffSwitchToggled()
    {
        audioProcessor.reverbOnOffState = reverbOnOff.getToggleState();
    }
    void delayOnOffSwitchToggled()
    {
        audioProcessor.delayOnOffState = delayOnOff.getToggleState();
    }
    
    

private:
    //void roomSizeSliderValueChanged(juce::Slider* slider) override; // [3]
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    VoltronAudioProcessor &audioProcessor;


    ComboBox keyMenu;
    OnOffButton pluginOnOff;
    OnOffButton reverbOnOff;
    OnOffButton delayOnOff;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (NewProjectAudioProcessorEditor)
};
