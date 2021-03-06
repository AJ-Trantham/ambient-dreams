/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"



//==============================================================================

VoltronAudioProcessor::VoltronAudioProcessor() 
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor(BusesProperties().withOutput("Output", juce::AudioChannelSet::stereo(), true)) // sets output but doesnot set input
#endif
{
    for (auto i = 0; i < maxNumVoices; ++i) {
        synth.addVoice(new SamplerVoice());
    }
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
    return true;
}

bool VoltronAudioProcessor::producesMidi() const
{
    return true;
}

bool VoltronAudioProcessor::isMidiEffect() const
{
    return true;
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
    /*// set up the tone generator
    toneGen.setAmplitude(1.0);
    toneGen.setFrequency(440.00);
    toneGen.prepareToPlay(sampleRate, samplesPerBlock); */
    
    int lastSampleRate = sampleRate;

    synth.setCurrentPlaybackSampleRate(sampleRate);
    reverb.setSampleRate(sampleRate);
}

void VoltronAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
    toneGen.releaseResources();
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
    // I will want to wrap this in some synth object rather than using the stright up ToneGeneratorAudioSource class
    //toneGen.getNextAudioBlock(juce::AudioSourceChannelInfo(buffer));

    //loadNewSample(&buffer, "ogg");

    Reverb::Parameters reverbParameters;
    reverbParameters.roomSize = roomSize;

    /*SynthesiserSound::Ptr newSound = new SamplerSound();
    sound = newSound;
    synth.addSound(sound);*/

    MidiMessage message = MidiMessage::noteOn(1, 5, (uint8)100);
    midiMessages.addEvent(message, 0);

    reverb.setParameters(reverbParameters);
    synth.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());

    if (getMainBusNumOutputChannels() == 1)
        reverb.processMono(buffer.getWritePointer(0), buffer.getNumSamples());
    else if (getMainBusNumOutputChannels() == 2)
        reverb.processStereo(buffer.getWritePointer(0), buffer.getWritePointer(1), buffer.getNumSamples());

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

