#include "ColorManager.h"
#include "GraphicsManager.h"
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <cstdlib>
#include <unistd.h>
#include "portaudio.h"
#include <fftw3.h>
#include <cmath>

class SoundManager;
typedef struct {
	double *in;
	fftw_complex *out;
	double *channels;
	double *num;
	SoundManager *soundManager;
} paTestData;

class SoundManager {
private:
	ColorManager *colorManager;
	GraphicsManager *graphicsManager;
	PaStream *stream;
	fftw_plan p;
	paTestData *data;
	double channelMinHz(int index);
	double channelMaxHz(int index);
	double channelWidth(int index);
	double volume;
	void drawScreen(double* channels, double x, double y, double z);
	int channelFromHz(double hz);
	double averageChannels(double *channels, double *num, int min, int max);
	double peakChannels(double *channels, double* num, int min, int max);
	void updateLeds(double *channels, double *num);
	void updateGraphics(double *channels, double *num);
	static int processCallback( const void *inputBuffer, void *outputBuffer,
	                           unsigned long framesPerBuffer,
	                           const PaStreamCallbackTimeInfo* timeInfo,
	                           PaStreamCallbackFlags statusFlags,
	                           void *userData );
public:
	SoundManager(ColorManager *colorManager, GraphicsManager *graphicsManager);
	PaStream* getStream();
	int initAudio();
	int closeAudio();
};
