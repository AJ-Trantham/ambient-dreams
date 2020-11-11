
#include "VoltronToneGenerator.h"

/*
* A Tone generator will generate a sine wave.
* Make sure set the sample rate, frequency and call updateAngleDelta()
* before calling fillBufferWithTone()
*/
VoltronToneGenerator::VoltronToneGenerator() {
    this->level = 0.125f;
    this->currentAngle = 0.0;
    this->frequency = 261.6256; // C4
}

VoltronToneGenerator::~VoltronToneGenerator() {

}

void VoltronToneGenerator::updateAngleDelta() {
    auto cyclesPerSample = this->frequency / this->sampleRate;                              
    this->angleDelta = cyclesPerSample * 2.0 * juce::MathConstants<double>::pi;     
}

void VoltronToneGenerator::setFrequency(double frequency) {
    this->frequency = frequency;
}

void VoltronToneGenerator::setSampleRate(double sampleRate) {
    this->sampleRate = sampleRate;
}

void VoltronToneGenerator::fillBufferWithTone(juce::AudioBuffer<float>& buffer) {
    
    AudioSourceChannelInfo bufferToFill = AudioSourceChannelInfo(buffer);

    auto* leftBuffer = bufferToFill.buffer->getWritePointer(0, bufferToFill.startSample);
    auto* rightBuffer = bufferToFill.buffer->getWritePointer(1, bufferToFill.startSample);

    for (auto sample = 0; sample < bufferToFill.numSamples; ++sample)
    {
        auto currentSample = (float)std::sin(currentAngle);
        currentAngle += this->angleDelta;
        leftBuffer[sample] = currentSample * this->level;
        rightBuffer[sample] = currentSample * this->level;
    }
}



