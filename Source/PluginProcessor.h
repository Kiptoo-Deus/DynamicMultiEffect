#pragma once
#include <JuceHeader.h>

class DynamicMultiEffectAudioProcessor : public juce::AudioProcessor
{
public:
    DynamicMultiEffectAudioProcessor();
    ~DynamicMultiEffectAudioProcessor() override;

    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;
    bool isBusesLayoutSupported(const BusesLayout& layouts) const override;
    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    double getTailLengthSeconds() const override;
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram(int) override;
    const juce::String getProgramName(int) override;
    void changeProgramName(int, const juce::String&) override;
    void getStateInformation(juce::MemoryBlock&) override;
    void setStateInformation(const void*, int) override;

    juce::AudioParameterFloat* gainParam;
    juce::AudioParameterFloat* levelParam;

private:
    float sampleRate;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DynamicMultiEffectAudioProcessor)
        bool isMidiEffect() const;
};
