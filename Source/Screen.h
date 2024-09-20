/*
  ==============================================================================

    Screen.h
    Created: 17 Sep 2024 4:10:23pm
    Author:  Chukwuemeka L. Nkama

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class Screen  : public juce::Component//, private juce::Timer
{
public:
    Screen();
    ~Screen() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    //void timerCallback() override;
    void setTempo(juce::String val);

private:
    // Screen is composed of units at RH of screen and current tempo value
    juce::String units{ "BPM" };
    juce::String tempo;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Screen)
};
