
#include "VoltronToneGenerator.h"
#include "Note.h"

/*
* A Tone generator will generate a sine wave.
* Make sure set the sample rate, frequency and call updateAngleDelta()
* before calling fillBufferWithTone()
*/
VoltronToneGenerator::VoltronToneGenerator() {
    
}

VoltronToneGenerator::~VoltronToneGenerator() {

}

void VoltronToneGenerator::addNote(Note* notePtr) {
    notes.push_back(*notePtr);
}

void VoltronToneGenerator::removeNote(double hz) {
    for (auto i = 0; i < notes.size(); i++) {
        if (notes[i].getFrequency()) {
            notes.erase (notes.begin()+i);
        }
    }
}

void VoltronToneGenerator::clearTones() {
    notes.clear();
}


void VoltronToneGenerator::fillBufferWithTone(juce::AudioBuffer<float>& buffer) {
    
    AudioSourceChannelInfo bufferToFill = AudioSourceChannelInfo(buffer);

    // right and left channel
    auto* leftBuffer = bufferToFill.buffer->getWritePointer(0, bufferToFill.startSample);
    auto* rightBuffer = bufferToFill.buffer->getWritePointer(1, bufferToFill.startSample);

    for (auto sample = 0; sample < bufferToFill.numSamples; ++sample)
    {

        // sums the sine components of each note in the generator
        auto currentSample = 0.0;
        for (auto i = 0; i < notes.size(); i++) {
            currentSample += notes[i].getSineWave();
        }
        
        leftBuffer[sample] = currentSample;
        rightBuffer[sample] = currentSample;
    }
}



