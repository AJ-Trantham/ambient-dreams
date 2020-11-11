/*
  ==============================================================================

    This file contains the basic framework code for a VotronToneGenerator.

    The VoltronToneGenerator is able to be set to a frequency and play a sine 
    wave of that frequency.

    Future idea would be to have this objcet be able to play other waves
    beside a sine wave.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
using namespace juce;

class VoltronToneGenerator {

public:
    VoltronToneGenerator();
    ~VoltronToneGenerator();

    void setFrequency(double frequency);
    void setSampleRate(double sampleRate);
    void updateAngleDelta();
    void prepareToPlay(double frequency, double sampleRate);
    void fillBufferWithTone(juce::AudioBuffer<float>& buffer);

private:
    double sampleRate;
    double currentAngle;
    double angleDelta;
    double frequency;
    double level;
    AudioSourceChannelInfo bufferToFill;
};