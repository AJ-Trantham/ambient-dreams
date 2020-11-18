#pragma once

#include <JuceHeader.h>

class Note {

public:
	Note(double frequency, double level, double sampleRate);
	~Note();

	double getSineWave();
	double getFrequency();

private:
	double frequency;
	double level;
	double currentAngle;
	double angleDelta;
	double sampleRate;
};

