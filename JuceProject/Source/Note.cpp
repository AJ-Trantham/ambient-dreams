#include "Note.h"

/*
* Represents a note and maintains internal values
* which are responsible for giving the current value 
* of the sine wave. 
*/
Note::Note(double frequency, double level, double sampleRate) {
	this->frequency = frequency;
	this->level = level * 0.125;
	this->sampleRate = sampleRate;
	this->currentAngle = 0.0;
	auto cyclesPerSample = this->frequency / this->sampleRate;
	angleDelta = cyclesPerSample * 2.0 * juce::MathConstants<double>::pi;
}

Note::~Note() {
	
}

/* 
* Gets the next sine wave value for the note
*/
double Note::getSineWave() {
	double currentSample = (float)std::sin(currentAngle);
	currentAngle += angleDelta;
	return (currentSample * level);
	
}

double Note::getFrequency() {
	return frequency;
}
