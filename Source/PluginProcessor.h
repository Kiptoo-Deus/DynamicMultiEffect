#pragma once
#include <JuceHeader.h>

class DynamicMultiEffectAudioProcessor : public juce::AudioProcessor {
public:
    DynamicMultiEffectAudioProcessor();
    ~DynamicMultiEffectAudioProcessor() override;

    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;
    bool isBusesLayoutSupported(const BusesLayout& layouts) const override;
    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override { return true; }

    const juce::String getName() const override { return "DynamicMultiEffect"; }
    bool acceptsMidi() const override { return false; }
    bool producesMidi() const override { return false; }
    bool isMidiEffect() const override { return false; }
    double getTailLengthSeconds() const override { return 0.0; }

    int getNumPrograms() override { return 1; }
    int getCurrentProgram() override { return 0; }
    void setCurrentProgram(int) override {}
    const juce::String getProgramName(int) override { return {}; }
    void changeProgramName(int, const juce::String&) override {}

    void getStateInformation(juce::MemoryBlock&) override;
    void setStateInformation(const void*, int) override;

    // Parameters
    juce::AudioParameterFloat* distortionGain;
    juce::AudioParameterFloat* delayTime;
    juce::AudioParameterFloat* reverbSize;

    // Get audio data for visualization
    void getAudioDataForVisualization(juce::AudioBuffer<float>& destBuffer);

    // DSP Components
    juce::dsp::ProcessorChain<
        juce::dsp::Gain<float>,         // Distortion (simplified)
        juce::dsp::DelayLine<float>,    // Delay
        juce::dsp::Reverb               // Reverb
    > effectChain;

    float maxDelaySamples = 0.0f; // Store max delay in samples

private:

    // Visualization buffer
    juce::AudioBuffer<float> visualizationBuffer;
    juce::CriticalSection bufferLock; // Thread safety

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DynamicMultiEffectAudioProcessor)
};