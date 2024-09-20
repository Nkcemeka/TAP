/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/**
*/
class TAPAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    TAPAudioProcessor();
    ~TAPAudioProcessor() override;

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

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    double getTime(int sample, double sampleRate);
    int calcTempo(const std::vector<double>&);
    int getTempo();
    void handleMIDI(juce::MidiBuffer&);
    void handleUIMode();
    juce::MidiBuffer tapBuffer; // Create a midi tap buffer
    std::atomic<bool> trigger;
    std::atomic<bool> mode; // checks if we are in UI or MIDI mode
    std::atomic<bool> tap; // checks if there is a tap or not

private:
    //==============================================================================
    int size; // size of samples we will consider for tempo calculation
    int pos;
    std::vector<double> timeValues;
    std::atomic<int> tempo;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TAPAudioProcessor)
};
