/*
==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
TAPAudioProcessorEditor::TAPAudioProcessorEditor (TAPAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p), tempo(std::to_string(audioProcessor.getTempo()))
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.

    // Set custom LookAndFeel
    juce::LookAndFeel::setDefaultLookAndFeel(&LNF);

    // set current tempo
    screen.setTempo(tempo);
    startTimerHz(60);

    // Setup label
    label.setColour(juce::Label::textColourId, juce::Colours::black);
    label.setText("TRIGGER", juce::dontSendNotification);
    label.setFont(juce::Font(10.0f, juce::Font::bold));

    // Setup toggle label
    toggleLabel.setColour(juce::Label::textColourId, juce::Colours::black);
    toggleLabel.setText("MIDI MODE", juce::dontSendNotification);
    toggleLabel.setFont(juce::Font(10.0f, juce::Font::bold));

    // Setup Tap Button
    tap.setButtonText("TAP");
    tap.setClickingTogglesState(true);
    tap.addListener(this);


    // Setup Toggle Switch
    toggleSwitch.setSliderStyle(juce::Slider::LinearHorizontal);
    toggleSwitch.setRange(0, 1, 0.1);
    toggleSwitch.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    toggleSwitch.onValueChange = [&]()
        {
            // Make switch behave like a switch
            if (toggleSwitch.getValue() < 0.5) {
                toggleSwitch.setValue(0.0, juce::dontSendNotification);
                audioProcessor.mode = false;
            }
            else {
                toggleSwitch.setValue(1.0, juce::dontSendNotification);
                audioProcessor.mode = true;
            }
        };
    

    // Add Components
    addAndMakeVisible(&screen);
    addAndMakeVisible(&led);
    addAndMakeVisible(label);
    addAndMakeVisible(toggleLabel);
    addAndMakeVisible(tap);
    addAndMakeVisible(toggleSwitch);

    setSize (550, 300);
}

TAPAudioProcessorEditor::~TAPAudioProcessorEditor()
{
    stopTimer();
    tap.removeListener(this);
}

//==============================================================================
void TAPAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    //g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
    g.fillAll(juce::Colour(0xffc9cbce));
}

void TAPAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    screen.setBounds(75, 20, 400, 150);

    // Get screen's bounds and set that of tap button
    auto screenBounds = screen.getBounds();
    auto tapWidth = 80;
    auto shift = 50;
    tap.setBounds(screenBounds.getCentre().getX() - tapWidth / 2 + shift, 180, tapWidth, 50);

    // Set position of LED and Label
    led.setBounds(tap.getBounds().getCentre().getX() - 30, 
        tap.getBounds().getY() + tap.getBounds().getHeight() + 10, 10, 10);
    label.setBounds(led.getBounds().getX() + led.getBounds().getWidth(),
        led.getBounds().getY(), 70, 10);

    // Set position of toggle Switch and its label
    auto switchWidth = 75;
    auto switchHeight = 30;
    toggleSwitch.setBounds(tap.getBounds().getX() - switchWidth - 20, 
        tap.getBounds().getCentre().getY() - switchHeight / 2, switchWidth, switchHeight);
    toggleLabel.setBounds(toggleSwitch.getBounds().getCentre().getX() - 28, led.getBounds().getY(),
        70, 10);
}

void TAPAudioProcessorEditor::timerCallback()
{

    // Turn on LED once button is clicked
    if (audioProcessor.trigger) led.setLED(true);
    audioProcessor.trigger = false; // Reset Trigger

    // set current tempo
    tempo = std::to_string(audioProcessor.getTempo());
    screen.setTempo(tempo);
    screen.repaint();
}

void TAPAudioProcessorEditor::sendMessage()
{
    // Send a MIDI NoteOn message to the MIDI Buffer at channel 1, key 60, vel 127
    auto message = juce::MidiMessage::noteOn(1, 69, (juce::uint8)127);
    audioProcessor.tapBuffer.addEvent(message, 0); // samplePosition is irrelevant to our algorithm
}

void TAPAudioProcessorEditor::buttonClicked(juce::Button* button)
{
    audioProcessor.tap = button->getToggleState();
    button->setToggleState(false, juce::dontSendNotification);
}

