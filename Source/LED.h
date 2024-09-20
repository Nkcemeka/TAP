/*
  ==============================================================================

    LED.h
    Created: 18 Sep 2024 10:56:46am
    Author:  Chukwuemeka L. Nkama

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class LED  : public juce::Component, private juce::Timer
{
public:
    LED();
    ~LED() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    void setLED(bool);
    void timerCallback() override;

private:
    bool trigger; // Boolean to store if the LED has been triggered 
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LED)
};
