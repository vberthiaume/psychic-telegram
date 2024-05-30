#include "PluginEditor.h"

PluginEditor::PluginEditor (PluginProcessor& p)
    : AudioProcessorEditor (&p), processorRef (p)
{
    //auto helloWorld = juce::String ("Hello from ") + PRODUCT_NAME_WITHOUT_VERSION + " v" VERSION + " running in " + CMAKE_BUILD_TYPE;

    addAndMakeVisible (inspectButton);

    // this chunk of code instantiates and opens the melatonin inspector
    inspectButton.onClick = [&] {
        if (!inspector)
        {
            inspector = std::make_unique<melatonin::Inspector> (*this);
            inspector->onClose = [this]() { inspector.reset(); };
        }

        inspector->setVisible (true);
    };

    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);

    //65 and 108 below in resized are important constants here
    vuMeterL.setNumFrames (65);
    vuMeterL.setAnimationImage (BinaryData::VUMeterL_png, BinaryData::VUMeterL_pngSize);
    addAndMakeVisible (vuMeterL);

    startTimer (25);
}

void PluginEditor::timerCallback()
{
    // Animation of VU meters
    //float smoothRMS = tanh(audioProcessor.curRMS*10);
    bool bypassed = false; //audioProcessor.params.bypassParam->get();
    float vuLevelL = bypassed ? 0.0f : processorRef.curRMSL * 3.0f;
    //float vuLevelR = bypassed ? 0.0f : audioProcessor.curRMSR * 3.0f;
    vuMeterL.updateImageWithValue (vuLevelL);
    //vuMeterR.updateImageWithValue (vuLevelR);
}

void PluginEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
}

void PluginEditor::resized()
{
    auto area = getLocalBounds();
    inspectButton.setBounds (area.removeFromBottom (50));
    vuMeterL.setBounds (area.withSizeKeepingCentre (108, 108));
}
