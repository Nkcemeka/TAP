/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
TAPAudioProcessor::TAPAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ), size{6}, tempo(0), timeValues(size, 0), pos(0), trigger(0), mode(0),
    tap(0)
#endif
{
}

TAPAudioProcessor::~TAPAudioProcessor()
{
}

//==============================================================================
const juce::String TAPAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool TAPAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool TAPAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool TAPAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double TAPAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int TAPAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int TAPAudioProcessor::getCurrentProgram()
{
    return 0;
}

void TAPAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String TAPAudioProcessor::getProgramName (int index)
{
    return {};
}

void TAPAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void TAPAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void TAPAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool TAPAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
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

void TAPAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    // Clear audio buffer
    buffer.clear();

    // Process MIDI from MIDI Input in MIDI mode
    if (mode) {
        handleMIDI(midiMessages);
    }
    else {
        // UI mode
        handleUIMode();
    }
}

//==============================================================================
bool TAPAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* TAPAudioProcessor::createEditor()
{
    return new TAPAudioProcessorEditor(*this);
}

//==============================================================================
void TAPAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void TAPAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new TAPAudioProcessor();
}

double TAPAudioProcessor::getTime(int sample, double sampleRate)
{
    int interval = sample;
    return interval / sampleRate;
}

int TAPAudioProcessor::calcTempo(const std::vector<double>& vals)
{
    double interval{ 0 }; // interval in seconds
    double num = vals[vals.size() - 1] - vals[0];
    double dem = size - 1;

    if (num == 0) {
        return interval;
    }

    interval = num/dem;
   
    return static_cast<int>(std::round(60 / interval)); // time in BPM
}

int TAPAudioProcessor::getTempo()
{
    return tempo;
}

void TAPAudioProcessor::handleUIMode()
{
    if (tap) {
        timeValues[pos] = juce::Time::getMillisecondCounterHiRes() / 1000;
        pos = (pos + 1) % size;
        trigger = true;

        // Calculate the Tempo after buffer has been updated
        if (pos == 0) {
            tempo = calcTempo(timeValues);
        }
        tap = false; // we are done processing
    }
}

void TAPAudioProcessor::handleMIDI(juce::MidiBuffer& midiMessages)
{
    // Go through Midi Messages and store in a fixed length vector of size n
    for (const auto metadata : midiMessages)
    {
        auto message = metadata.getMessage();

        if (message.isNoteOn())
        {
            // Store current time
            timeValues[pos] = juce::Time::getMillisecondCounterHiRes() / 1000;
            pos = (pos + 1) % size;
            trigger = true;
        }

        // Calculate the Tempo after buffer has been updated
        if (pos == 0) {
            tempo = calcTempo(timeValues);
        }
    }
    midiMessages.clear();
}