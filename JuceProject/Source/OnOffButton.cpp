#include "OnOffButton.h"

/*
* Represents a note and maintains internal values
* which are responsible for giving the current value
* of the sine wave.
*/
OnOffButton::OnOffButton() {
}

OnOffButton::~OnOffButton() {

}

void OnOffButton::paintButton(juce::Graphics& g, bool isMouseOverButton, bool isButtonDown)
{
	float dotSize = 10.0f;
	float strokeClip = 5.0f;

	juce::Colour red = juce::Colour(255, 0, 0);
	juce::Colour white = juce::Colour(0, 0, 0);

	g.setColour(juce::Colours::white);
	g.fillEllipse(0, 0, 25, 25);

	if (isButtonDown)
	{
		g.setColour(juce::Colours::red);
	}
	else if (isMouseOverButton)
	{
		g.setColour(juce::Colours::red.brighter());
	}
	else
	{
		g.setColour(juce::Colours::red);
	}

	//Ellipse
	g.fillEllipse(2, 2, 21, 21);

	//Text
	g.setColour(juce::Colours::black);
	g.setFont(juce::Font(dotSize / 1.5f, juce::Font::plain));
}