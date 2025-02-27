#pragma once
#include <JuceHeader.h>
#include "PluginProcessor.h"

class DynamicMultiEffectAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
    explicit DynamicMultiEffectAudioProcessorEditor(DynamicMultiEffectAudioProcessor&);
    ~DynamicMultiEffectAudioProcessorEditor() override;

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    DynamicMultiEffectAudioProcessor& audioProcessor;
    juce::Slider gainSlider;
    juce::Slider levelSlider;
    std::unique_ptr<juce::AudioProcessorParameter::Listener> gainAttachment;
    std::unique_ptr<juce::AudioProcessorParameter::Listener> levelAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DynamicMultiEffectAudioProcessorEditor)
};