/*
  ==============================================================================

    LookAndFeel.h
    Created: 17 Sep 2024 3:58:05pm
    Author:  Chukwuemeka L. Nkama

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class LookAndFeel : public juce::LookAndFeel_V4 {
public:
    LookAndFeel();
    void drawLinearSlider(juce::Graphics& g,
        int x, int y, int width, int height,
        float sliderPos, float minSliderPos, float maxSliderPos,
        const juce::Slider::SliderStyle, juce::Slider& slider) override;
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LookAndFeel);
};
