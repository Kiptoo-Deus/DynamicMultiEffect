#include "PluginProcessor.h"
#include "PluginEditor.h"

// Custom listener class to update sliders
class SliderParameterListener : public juce::AudioProcessorParameter::Listener
{
public:
    SliderParameterListener(juce::Slider& s) : slider(s) {}
    void parameterValueChanged(int /*parameterIndex*/, float newValue) override
    {
        slider.setValue(newValue, juce::dontSendNotification);
    }
    void parameterGestureChanged(int /*parameterIndex*/, bool /*gestureIsStarting*/) override {}
private:
    juce::Slider& slider;
};

DynamicMultiEffectAudioProcessorEditor::DynamicMultiEffectAudioProcessorEditor(DynamicMultiEffectAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p)
{
    gainSlider.setSliderStyle(juce::Slider::LinearVertical);
    gainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
    gainSlider.setRange(0.0f, 10.0f);
    gainSlider.setValue(*p.gainParam);
    gainSlider.onValueChange = [this] { *audioProcessor.gainParam = gainSlider.getValue(); };
    addAndMakeVisible(gainSlider);

    levelSlider.setSliderStyle(juce::Slider::LinearVertical);
    levelSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
    levelSlider.setRange(0.0f, 1.0f);
    levelSlider.setValue(*p.levelParam);
    levelSlider.onValueChange = [this] { *audioProcessor.levelParam = levelSlider.getValue(); };
    addAndMakeVisible(levelSlider);

    gainAttachment = std::make_unique<SliderParameterListener>(gainSlider);
    levelAttachment = std::make_unique<SliderParameterListener>(levelSlider);
    p.gainParam->addListener(gainAttachment.get());
    p.levelParam->addListener(levelAttachment.get());

    setSize(200, 300);
}

DynamicMultiEffectAudioProcessorEditor::~DynamicMultiEffectAudioProcessorEditor()
{
}

void DynamicMultiEffectAudioProcessorEditor::paint(juce::Graphics& g)
{
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
    g.setColour(juce::Colours::white);
    g.setFont(15.0f);
    g.drawText("Dynamic Multi-Effect", getLocalBounds(), juce::Justification::centredTop, true);
}

void DynamicMultiEffectAudioProcessorEditor::resized()
{
    gainSlider.setBounds(40, 50, 50, 200);
    levelSlider.setBounds(110, 50, 50, 200);
}