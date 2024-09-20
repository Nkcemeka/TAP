/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "LookAndFeel.h"
#include "Screen.h"
#include "LED.h"

//==============================================================================
/**
*/
class TAPAudioProcessorEditor  : public juce::AudioProcessorEditor, private juce::Timer,
    private juce::Button::Listener
{
public:
    TAPAudioProcessorEditor (TAPAudioProcessor&);
    ~TAPAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    void timerCallback() override;
    void buttonClicked(juce::Button* button) override;
    void sendMessage();

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    TAPAudioProcessor& audioProcessor;
    LookAndFeel LNF;
    Screen screen;
    LED led;
    juce::TextButton tap;
    juce::Label label;
    juce::Label toggleLabel;
    juce::String tempo;
    juce::Slider toggleSwitch;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TAPAudioProcessorEditor)
};
