/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "VoltronToneGenerator.h"




//==============================================================================

VoltronAudioProcessor::VoltronAudioProcessor() 
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor(BusesProperties().withOutput("Output", juce::AudioChannelSet::stereo(), true)) // sets output but doesnot set input
#endif
    {
        
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
   // stk::Delay(0,4095);
   // this->sampleRate = stk::Delay::tick (this->sampleRate );
    
    reverb.setSampleRate(sampleRate);
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
    
    if (onOffState)
    {
        float r = rSize;
        // we only want to change notes when a new frequency is slected
        double freq = this->rootFrequencyValue;
        if (freq != prevRootFreq) {
            double root = freq;
            double octave = root * 2.0;
            double third = root * (5.0 / 4.0);
            double fifth = root * (6.0 / 4.0);
            toneGenRoot.clearTones(); // delete the old chord
            toneGenRoot.addNote(new Note(root, .25, this->sampleRate));
            toneGenRoot.addNote(new Note(octave, .25, this->sampleRate));
            toneGenRoot.addNote(new Note(third, .5, this->sampleRate));
            toneGenRoot.addNote(new Note(fifth, .5, this->sampleRate));
            prevRootFreq = freq;
        }

        // fill buffer with base chordal tone - this needs to happen before we try to put our buffer through ANY effect
        toneGenRoot.fillBufferWithTone(buffer);

        // apply chorus effect
        StkFrames frames = StkFrames(buffer.getNumSamples(),2);
        int size=buffer.getNumSamples();
                
        unsigned long v = frames.size();
        printf("size of array %ul ", v);
        Chorus chor = Chorus(6000);
        chor.setModDepth(.8);
        chor.setModFrequency(20.0);
        chor.setEffectMix(.8); // this seems to affect the overall sound
        PitShift pitchShift = PitShift(); // I tried this pitch effect as well but it doesn't seem to be working
        pitchShift.setShift(.7);
        pitchShift.setEffectMix(.8);
        for(int i=0;i<size;i++){
            StkFloat bufferSample = buffer.getSample(0, i);
            StkFloat audioData = chor.tick(bufferSample);
            //StkFloat audioData = pitchShift.tick(bufferSample);
            buffer.setSample(0, i, (float)audioData);
            buffer.addSample(1, i, (float)audioData);
        }

       //frames.~StkFrames();
       //buff.~StkFrames();
       //chor.clear();
        

        reverbParameters.roomSize = r;
        reverbParameters.wetLevel = wet;
        reverbParameters.dryLevel = dry;
        reverbParameters.damping = damping;
        printf("Room Size value %f ", reverbParameters.roomSize);
        printf("wet level %f\n ", reverbParameters.wetLevel);
        printf("damping %f \n", reverbParameters.damping);
        printf("dry level %f \n", reverbParameters.dryLevel);
        

        reverb.setParameters(reverbParameters);

        if (getMainBusNumOutputChannels() == 1)
            reverb.processMono(buffer.getWritePointer(0), buffer.getNumSamples());
        else if (getMainBusNumOutputChannels() == 2)
            reverb.processStereo(buffer.getWritePointer(0), buffer.getWritePointer(1), buffer.getNumSamples());
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

