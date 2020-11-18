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
#include "Note.h"
#include <vector>
using namespace juce;

class VoltronToneGenerator {

public:
    VoltronToneGenerator();
    ~VoltronToneGenerator();

    void addNote(Note* note);
    void removeNote(double hz);
    void fillBufferWithTone(juce::AudioBuffer<float>& buffer);
    void clearTones();

private:
    
    AudioSourceChannelInfo bufferToFill;
    std::vector<Note> notes;
};