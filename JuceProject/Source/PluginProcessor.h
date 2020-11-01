/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
using namespace juce;


//==============================================================================
/**
*/
class VoltronAudioProcessor  : public juce::AudioProcessor
                             
{
public:
    //==============================================================================
    VoltronAudioProcessor();
    ~VoltronAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;
    public: float rSize=0.5;
    public: float wet=0.5;
    public: float dry=0.5;
    public: float damping=0.5;
    public: float rootFrequencyValue;
    public: ToneGeneratorAudioSource toneGenRoot;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;


private:
    void loadNewSample(std::unique_ptr<InputStream> soundBuffer, const char* format)
    {
        std::unique_ptr<AudioFormatReader> formatReader(formatManager.findFormatForFileExtension(format)->createReaderFor(soundBuffer.release(), true));

        BigInteger midiNotes;
        midiNotes.setRange(0, 126, true);
        SynthesiserSound::Ptr newSound = new SamplerSound("Voice", *formatReader, midiNotes, 0x40, 0.0, 0.0, 10.0);
        sound = newSound;
        synth.addSound(sound);
    }
    //==============================================================================
    static constexpr int maxNumVoices = 3;
    static constexpr float roomSize = 0.5; // TODO: this will eventually need to be set with slider values along with other reverb params
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(VoltronAudioProcessor);
    AudioFormatManager formatManager;
    Reverb reverb;
    Synthesiser synth;
    SynthesiserSound::Ptr sound;
    
};
