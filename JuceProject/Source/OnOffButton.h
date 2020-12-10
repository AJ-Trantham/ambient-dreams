#pragma once

#include <JuceHeader.h>

class OnOffButton : public juce::ToggleButton {

public:
	OnOffButton();
	~OnOffButton();

	void paintButton(juce::Graphics& g, bool isMouseOverButton, bool isButtonDown) override;

private:
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(OnOffButton);
};