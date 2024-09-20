/*
  ==============================================================================

    LookAndFeel.cpp
    Created: 17 Sep 2024 3:57:51pm
    Author:  Chukwuemeka L. Nkama

  ==============================================================================
*/

#include "LookAndFeel.h"

LookAndFeel::LookAndFeel()
{
    // Set font to Roboto
    juce::Typeface::Ptr typeface = juce::Typeface::createSystemTypefaceFor(
        BinaryData::RubikMonoOneRegular_ttf, BinaryData::RubikMonoOneRegular_ttfSize
    );
    setDefaultSansSerifTypeface(typeface);
    setColour(juce::ResizableWindow::backgroundColourId, juce::Colour(0xffc9cbce));
    setColour(juce::TextButton::ColourIds::buttonColourId, juce::Colour(0xff101621));
}

void LookAndFeel::drawLinearSlider(juce::Graphics& g,
    int x, int y, int width, int height,
    float sliderPos, float minSliderPos, float maxSliderPos,
    const juce::Slider::SliderStyle, juce::Slider& slider)
{
    // Get toggle state
    bool state = slider.getValue() >= 0.5;

    // Define background colour
    juce::Colour bgc = state ? juce::Colour(0xff005d5d) : juce::Colour(0xff101621);

    // Create a rounded Rectangle Background
    auto backRect = juce::Rectangle<float>(
        static_cast<float> (x),
        static_cast<float> (y),
        static_cast<float> (width),
        static_cast<float> (height)
    );

    // Set colour of rounded rectangle
    g.setColour(bgc);
    g.fillRoundedRectangle(backRect, height/2);

    // Thumb circle for linear slider
    float rad = height * 0.5f;

    // limit sliderPos so thumb fits in it
    float limitPos = juce::jlimit(x + rad, x + width - rad, sliderPos);

    g.setColour(juce::Colours::white);
    auto thumbBounds = juce::Rectangle<float>{
        rad, rad
    }.withCentre({ limitPos, static_cast<float>(y) + height * 0.5f });
    g.fillEllipse(thumbBounds);
}