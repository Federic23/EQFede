/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

struct LookAndFeel : juce::LookAndFeel_V4
{
    void drawRotarySlider(juce::Graphics&,
        int x, int y, int width, int height,
        float sliderPosProportional,
        float rotaryStartAngle,
        float rotaryEndAngle,
        juce::Slider&) override;
};

struct RotarySliderWithLabels : juce::Slider
{
    RotarySliderWithLabels(juce::RangedAudioParameter& rangedAudioParameter, const juce::String& unitSuffix) :
        juce::Slider(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag,
        juce::Slider::TextEntryBoxPosition::NoTextBox),
    param(&rangedAudioParameter),
    suffix(unitSuffix)
    {
        setLookAndFeel(&lookAndFeel);
    }

    ~RotarySliderWithLabels()
    {
        setLookAndFeel(nullptr);
    }

    void paint(juce::Graphics& g)override;
    juce::Rectangle<int> getSliderBounds() const;
    int getTextHeight() const { return 14; }
    juce::String getDisplayString() const;
private:
    LookAndFeel lookAndFeel;

    juce::RangedAudioParameter* param;
    juce::String suffix;
};

struct ResponseCurveComponent : juce::Component,
juce::AudioProcessorParameter::Listener,
juce::Timer
{
    ResponseCurveComponent(EQFedeAudioProcessor&);
    ~ResponseCurveComponent();

    void parameterValueChanged(int parameterIndex, float newValue) override;

    void parameterGestureChanged(int parameterIndex, bool gestureIsStarting) override { }

    void timerCallback() override;

    void paint(juce::Graphics& g) override;
private:
    EQFedeAudioProcessor& audioProcessor;
    juce::Atomic<bool> parametersChanged{ false };
    MonoChain monoChain;
};

//==============================================================================
/**
*/
class EQFedeAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
    EQFedeAudioProcessorEditor (EQFedeAudioProcessor&);
    ~EQFedeAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;


private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    EQFedeAudioProcessor& audioProcessor;


    RotarySliderWithLabels peakFreqSlider, 
                       peakGainSlider, 
                       peakQualitySlider, 
                       lowCutFreqSlider, 
                       highCutFreqSlider, 
                       lowCutSlopeSlider, 
                       highCutSlopeSlider;

    //Pasar de nombres nemotecnicos a no nemotecnicos no me parece la mejor opcion, pero bueh
    using APVTS = juce::AudioProcessorValueTreeState;
    //attach de paraameter to the slider
    using Attachment = APVTS::SliderAttachment;

    Attachment peakFreqSliderAttachment,
                peakGainSliderAttachment,
                peakQualitySliderAttachment,
                lowCutFreqSliderAttachment,
                highCutFreqSliderAttachment,
                lowCutSlopeSliderAttachment,
                highCutSlopeSliderAttachment;

    ResponseCurveComponent responseCurveComponent;

    std::vector<juce::Component*> getComps();


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EQFedeAudioProcessorEditor)
};
