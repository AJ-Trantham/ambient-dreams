/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <stdint.h>

//==============================================================================
VoltronAudioProcessor::VoltronAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
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
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool VoltronAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool VoltronAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
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
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void VoltronAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool VoltronAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void VoltronAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    buffer.clear();

    juce::MidiBuffer processedMidi;
    int time;
    juce::MidiMessage m;

    for (juce::MidiBuffer::Iterator i(midiMessages); i.getNextEvent(m, time);)
    {
        if (m.isNoteOn())
        {
            uint8_t newVel = (uint8_t)noteOnVel;
            m = juce::MidiMessage::noteOn(m.getChannel(), m.getNoteNumber(), newVel);
        }
        else if (m.isNoteOff())
        {
        }
        else if (m.isAftertouch())
        {
        }
        else if (m.isPitchWheel())
        {
        }

        processedMidi.addEvent(m, time);
    }

    midiMessages.swapWith(processedMidi);
}

//==============================================================================
bool VoltronAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* VoltronAudioProcessor::createEditor()
{
    return new VoltronAudioProcessorEditor (*this);
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
