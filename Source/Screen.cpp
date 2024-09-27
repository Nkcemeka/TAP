/*
  ==============================================================================

    Screen.cpp
    Created: 17 Sep 2024 4:10:23pm
    Author:  Chukwuemeka L. Nkama

  ==============================================================================
*/

#include <JuceHeader.h>
#include "Screen.h"

//==============================================================================
Screen::Screen()
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
}

Screen::~Screen()
{
}

void Screen::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    // Get component's bounds
    auto bounds = getLocalBounds().toFloat();
    juce::Point<float> topMid {bounds.getCentreX(), bounds.getY()};

    // Define Colour Gradient for screen
    juce::ColourGradient cg {juce::Colour(0xff101821), bounds.getBottomRight(),
        juce::Colour(0xff68717c), topMid, false};
    g.setGradientFill(cg);

    // Radius for corner curves
    float rad = 10.0f;

    g.drawRoundedRectangle(bounds, rad, 1);
    g.fillRoundedRectangle(bounds, rad);
    g.setColour(juce::Colours::white);
    g.setFont (30.0f);
    g.drawText (tempo, bounds,
                juce::Justification::centred, true);   // draw Tempo to Screen


    g.setFont(16.0f);
    juce::Rectangle<float> paddedArea = bounds.withTrimmedRight(10).withTrimmedBottom(10);
    g.drawText(units, paddedArea, juce::Justification::bottomRight, true);
}

void Screen::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
}

void Screen::setTempo(juce::String val)
{
    tempo = val;
}