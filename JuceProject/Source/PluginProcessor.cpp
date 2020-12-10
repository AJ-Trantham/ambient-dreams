/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "VoltronToneGenerator.h"
#include <time.h>
#include <stdlib.h>


//==============================================================================

VoltronAudioProcessor::VoltronAudioProcessor() 
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor(BusesProperties().withOutput("Output", juce::AudioChannelSet::stereo(), true)) // sets output but doesnot set input
#endif
    {
        srand( static_cast<unsigned int>(time(nullptr)));
}

VoltronAudioProcessor::~VoltronAudioProcessor()
{
}

//==============================================================================
const juce::String VoltronAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool VoltronAudioProcessor::acceptsMidi() const
{
    return false;
}

bool VoltronAudioProcessor::producesMidi() const
{
    return false;
}

bool VoltronAudioProcessor::isMidiEffect() const
{
    return false;
}

double VoltronAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int VoltronAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int VoltronAudioProcessor::getCurrentProgram()
{
    return 0;
}

void VoltronAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String VoltronAudioProcessor::getProgramName (int index)
{
    return {};
}

void VoltronAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void VoltronAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    this->sampleRate = sampleRate; // this is essential as we need the sample rate when adding notes in the processBlock()
    reverb.setSampleRate(sampleRate);

    // set up delay size, samples per block is bonus time
    int numSeconds = 2;
    int delayBufferSize = numSeconds * (sampleRate + samplesPerBlock);

    // 1 channel which is our generated audio
    delayBuffer.setSize(1, delayBufferSize); 
    delayBuffer.clear();

    expectedReadPosition = -1; // ??? might need to be 0
    writePosition = 0;
    readPosition = 0;
}

void VoltronAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
    // TODO: we might need to call the destructors of the tone generators here
    toneGenRoot.clearTones();
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool VoltronAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
    // sends outpout to both right and left
    return (layouts.getMainOutputChannels() <= 2);
}
#endif

void VoltronAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    Reverb::Parameters reverbParameters;
    
    if (pluginOnOffState)
    {
        float r = rSize;
        // we only want to change notes when a new frequency is slected
        double freq = this->rootFrequencyValue;
        
        double root = freq;
        double octave = root * 2.0;
        double third = root * (5.0 / 4.0);
        double fifth = root * (6.0 / 4.0);
        
        if (freq != prevRootFreq) {
            toneGenRoot.clearTones(); // delete the old chord
            toneGenRoot.addNote(new Note(root, .5, this->sampleRate));
            toneGenRoot.addNote(new Note(octave, .25, this->sampleRate));
            toneGenRoot.addNote(new Note(third, .5, this->sampleRate));
            toneGenRoot.addNote(new Note(fifth, .5, this->sampleRate));
            prevRootFreq = freq;
        }
                
        if (rand() % 200 == 0) { // about every 2 second
            
            int n = rand() % 6 + 1; // random 1 to 6
            toneGenRoot.clearTones(); // delete the old chord
            toneGenRoot.addNote(new Note(root, .5, this->sampleRate));
            toneGenRoot.addNote(new Note(octave, .25, this->sampleRate));
            toneGenRoot.addNote(new Note(third, .5, this->sampleRate));
            toneGenRoot.addNote(new Note(fifth, .5, this->sampleRate));

            if (n == 1) { // take out octave
                toneGenRoot.removeNote(octave);
            }
            else if (n == 2) { // take out 3
                toneGenRoot.removeNote(third);

            }
            else if (n == 3) { // take out 5
                toneGenRoot.removeNote(fifth);

            }
            else if (n == 4) { // take out 3 and 5
                toneGenRoot.removeNote(third);
                toneGenRoot.removeNote(fifth);
            }
            else if (n == 5) { // take out 3 and 5
                toneGenRoot.removeNote(octave);
                toneGenRoot.removeNote(fifth);
            }
            else if (n == 6) { // take out 3 and 5
                toneGenRoot.removeNote(octave);
                toneGenRoot.removeNote(third);
            }
            printf("!!!");
        }
        
        // get root tone into buffer
        toneGenRoot.fillBufferWithTone(buffer);



        // apply effects here

        if(delayOnOffState){
        printf(" Check if entered if statment for delay");
        // do delay
        const int bufferLength = buffer.getNumSamples();
        const int delayBufferLength = delayBuffer.getNumSamples();

        const float* bufferData = buffer.getReadPointer(0);
        const float* delayBufferData = delayBuffer.getReadPointer(0);
        float gainStart =startGain;
        float gainEnd= .01;
        printf(" Gain Start %f\n ", gainStart);
        writeToDelayBuffer(buffer, 0, writePosition, gainStart, gainEnd, bufferData, delayBufferData, bufferLength, delayBufferLength);
        readFromDelayBuffer(buffer, 0, writePosition, gainStart, gainEnd, bufferData, delayBufferData, bufferLength, delayBufferLength);

        // update delay buffer positions
        writePosition += bufferLength;
        writePosition %= delayBufferLength;
        // end delay
        }
   if (reverbOnOffState)
        {
            reverbParameters.roomSize = r;
            reverbParameters.wetLevel = wet;
            reverbParameters.dryLevel = dry;
            reverbParameters.damping = damping;
            printf("Room Size value %f ", reverbParameters.roomSize);
            printf("wet level %f\n ", reverbParameters.wetLevel);
            printf("damping %f \n", reverbParameters.damping);
            printf("dry level %f \n", reverbParameters.dryLevel);

            reverb.setParameters(reverbParameters);

        
        }
        if (getMainBusNumOutputChannels() == 1)
            reverb.processMono(buffer.getWritePointer(0), buffer.getNumSamples());
        else if (getMainBusNumOutputChannels() == 2)
            reverb.processStereo(buffer.getWritePointer(0), buffer.getWritePointer(1), buffer.getNumSamples());
       
            
        
    }
}

void VoltronAudioProcessor::readFromDelayBuffer(AudioSampleBuffer& buffer, const int channelIn, const int writePos, float startGain, float endGain,
    const float* bufferData, const float* delayBufferData, const int bufferLength, const int delayBufferLength) 
{
    int delayTime = delayLength; //  This is in ms
    printf("delay time  %d \n", delayTime);
    //  TODO: change later John says its not rocket science
        
    const int readPosition = static_cast<int> (delayBufferLength + writePosition - (sampleRate * delayTime / 1000)) % delayBufferLength; // this lets us access the buffer backwards past 0

    if (delayBufferLength > bufferLength + readPosition) {
        buffer.addFrom(channelIn, 0, delayBufferData + readPosition, bufferLength);
    }
    else {
        const int bufferRemaining = delayBufferLength - readPosition;
        buffer.addFrom(channelIn, 0, delayBufferData + readPosition, bufferRemaining);
        buffer.addFrom(channelIn, 0, delayBufferData + readPosition, bufferLength-bufferRemaining);
    }

}

void VoltronAudioProcessor::writeToDelayBuffer(AudioSampleBuffer& buffer, const int channelIn, const int writePos, float startGain, float endGain,
    const float* bufferData, const float* delayBufferData, const int bufferLength, const int delayBufferLength)
{
    // copy data from main buffer delay buffer
    if (delayBufferLength > bufferLength + writePosition) {
        delayBuffer.copyFromWithRamp(channelIn, writePosition, bufferData, bufferLength, startGain, endGain);
    }
    else {
        const int bufferRemaining = delayBufferLength - writePosition; // the space left in our buffer
        delayBuffer.copyFromWithRamp(channelIn, writePosition, bufferData, bufferRemaining, startGain, endGain);
        delayBuffer.copyFromWithRamp(channelIn, 0, bufferData + bufferRemaining, bufferLength - bufferRemaining, startGain, endGain);
    }
}

//==============================================================================
bool VoltronAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* VoltronAudioProcessor::createEditor()
{
    return new NewProjectAudioProcessorEditor (*this);
}

//==============================================================================
void VoltronAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void VoltronAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}


//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new VoltronAudioProcessor();
} 

