#include "SoundManager.h"

#define SAMPLE_RATE  (44100)
#define FRAMES_PER_BUFFER (1024)
#define DITHER_FLAG     (0)

#define PA_SAMPLE_TYPE paFloat32
typedef float SAMPLE;

void SoundManager::drawScreen(double* channels, double x, double y, double z){
	for (int j=0; j<30; j++) {
		std::cout << "--";
	}
	std::cout << '\n';

	for(int i=0; i<20; i++){
		for (int j=0; j<30; j++) {
			double db = (channels[j])/channelWidth(j);
			std::cout << (db>i?'0':' ');
			std::cout << ' ';
		}
		std::cout << "   ";

		std::cout << (x*2>i?"00":"  ");
		std::cout << ' ';

		std::cout << (y*2>i?"00":"  ");
		std::cout << ' ';

		std::cout << (z*2>i?"00":"  ");
		std::cout << ' ';

		std::cout << '\n';
	}
	std::cout << '\n';
}

static double channelToHz(int index) {
	static double freqs[30] = {20,25,31.5,40,50,63,80,100,125,160,
												200,250,315,400,500,630,800,1000,1250,1600,
												2000,2500,3150,4000,5000,6300,8000,10000,12500,16000};
	if(index < 0) return -1;
	if(index >= 30) return -1;
	return freqs[index];
}

double SoundManager::channelMinHz(int index) {
	static double freqs[30] = {18,22,28,35,45,56,71,90,112,140,
												179,224,282,353,448,560,706,897,1121,1401,
												1794,2242,2803,3531,4484,5605,7062,8908,11210,14012};
	if(index < 0) return -1;
	if(index >= 30) return -1;
	return freqs[index];
}

double SoundManager::channelMaxHz(int index) {
	static double freqs[30] = {22,28,35,45,56,71,90,112,140,179,
												224,282,353,448,560,706,897,1121,1401,1794,
												2242,2803,3531,4484,5605,7062,8908,11210,14012,17936};
	if(index < 0) return -1;
	if(index >= 30) return -1;
	return freqs[index];
}

double SoundManager::channelWidth(int index){
	return channelMaxHz(index)-channelMinHz(index);
}

int SoundManager::channelFromHz(double hz){
	for(int i=30; i>=0; i--){
		if(hz>channelMinHz(i)&&hz<channelMaxHz(i)){
			return i;
		}
	}
	return -1;
}

double SoundManager::averageChannels(double *channels, double *num, int min, int max){
	double total = 0;
	double valid = 0;
	for(int i=min; i<max; i++){
		if(num[i]==0)continue;
		double amt = (channels[i])/channelWidth(i)/num[i];
		total += amt*amt;
		valid++;
	}
	return sqrt(total)/valid;
}

double SoundManager::peakChannels(double *channels, double* num, int min, int max){
	double max_val = 0;
	for(int i=min; i<max; i++){
		if(num[i]==0)continue;
		double amt = (channels[i])/channelWidth(i)/num[i];
		if(amt > max_val) max_val = amt;
	}
	return max_val;
}

void SoundManager::updateLeds(double *channels, double *num){

	double x = (averageChannels(channels, num, 0, 8) + peakChannels(channels, num, 4, 8))/6;
	double y = (averageChannels(channels, num, 9, 12) + peakChannels(channels, num, 9, 12))/6;
	double z = (averageChannels(channels, num, 13, 30) + peakChannels(channels, num, 13, 30))/6;

	volume  = 0.8*volume + .2 * (x+y+z) / 3.0;
	if(volume <= 0.1) volume = 0;

	x /= 2.9;
	y *= 1.5;
	z *= 2.0;

	if(volume > 0){
		double v = pow(volume, .8);
		x /= v;
		y /= v;
		z /= v;
	}

	Color target_1 = {
		0.8-(y/x)+3.0*z,											//G_PIN_0
		0.4-z+x,														//R_PIN_0
		.2+2*(x-y/2),												//B_PIN_0
		1.5-1.8*cbrt(x*y*z)				 		//ALPHA_0
	};

	Color target_0 = {
		1.2*(.8-y)+x/1.5,		//R_PIN_1
		.9*(1.25-y*.8)+0.4,						//G_PIN_1
		z+y/2.0*.9,								//B_PIN_1
		1.5-1.8*cbrt(x*y*z)						//ALPHA_1
	};

	colorManager->setColors(target_0, target_1);
	//data->soundManager->drawScreen(channels, x, y, z);
}

int SoundManager::initAudio(){
	PaStreamParameters inputParameters;
	PaError err = paNoError;

	err = Pa_Initialize();
	if( err != paNoError) {
		printf(  "PortAudio error: %s\n", Pa_GetErrorText( err ) );
		return 1;
	}

	inputParameters.device = Pa_GetDefaultInputDevice(); /* default input device */
	if (inputParameters.device == paNoDevice) {
		fprintf(stderr,"Error: No default input device.\n");
		return 1;
	}

	inputParameters.channelCount = 2;                    /* stereo input */
	inputParameters.sampleFormat = PA_SAMPLE_TYPE;
	inputParameters.suggestedLatency = Pa_GetDeviceInfo( inputParameters.device )->defaultLowInputLatency;
	inputParameters.hostApiSpecificStreamInfo = NULL;

	err = Pa_OpenStream(
		&stream,
		&inputParameters,
		NULL,                  /* &outputParameters, */
		SAMPLE_RATE,
		FRAMES_PER_BUFFER,
		paClipOff,      /* we won't output out of range samples so don't bother clipping them */
		&SoundManager::processCallback,
		data );
	if( err != paNoError) {
		printf(  "PortAudio error: %s\n", Pa_GetErrorText( err ) );
		return 1;
	}

	err = Pa_StartStream( stream );
	if( err != paNoError) {
		printf(  "PortAudio error: %s\n", Pa_GetErrorText( err ) );
		return 1;
	}
	printf("\n=== Working. Press Ctrl+C to quit ===\n"); fflush(stdout);
	return 0;
}

int SoundManager::closeAudio(){
	fftw_destroy_plan(p);
	PaError err = paNoError;
	 //dispose of things here
	err = Pa_CloseStream( stream );
	if( err != paNoError ) {
		printf(  "PortAudio error: %s\n", Pa_GetErrorText( err ) );
		return 1;
	}

	err = Pa_Terminate();
	if( err != paNoError ) {
		printf(  "PortAudio error: %s\n", Pa_GetErrorText( err ) );
		return 1;
	}
	return 0;
}

int SoundManager::processCallback( const void *inputBuffer, void *outputBuffer,
                           unsigned long framesPerBuffer,
                           const PaStreamCallbackTimeInfo* timeInfo,
                           PaStreamCallbackFlags statusFlags,
                           void *userData )
{
	const SAMPLE *rptr = (const SAMPLE*)inputBuffer;
	paTestData *data = (paTestData *) userData;

	for (int i = 0; i < FRAMES_PER_BUFFER; i++) {
	    double multiplier = 0.5 * (1 - cos(2*3.14159265*i/(FRAMES_PER_BUFFER-1)));
	    data->in[i] = multiplier * rptr[i];
	}

	fftw_execute(data->soundManager->p);
	memset(data->channels, 0, sizeof(double)*30);
	memset(data->num, 0, sizeof(double)*30);

	int max_index = FRAMES_PER_BUFFER/2+1;

	for(int i=1; i<max_index; i++) {
		double real = (double)data->out[i][0];
		double imaginary = (double)data->out[i][1];

		double amp = sqrt(real*real + imaginary*imaginary);
		double freq = (double)i*(double)SAMPLE_RATE/(double)FRAMES_PER_BUFFER;

		int channel = data->soundManager->channelFromHz(freq);
		if(channel==-1) continue;
		data->channels[channel] += amp;
		data->num[channel] += 1;
	}
	data->soundManager->updateLeds(data->channels, data->num);
	return 0;
}

SoundManager::SoundManager(ColorManager *colorManager){
	this->colorManager = colorManager;
	data = new paTestData();
	data->in = (double *) fftw_malloc(sizeof(SAMPLE) * FRAMES_PER_BUFFER);
	data->out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * FRAMES_PER_BUFFER);
	data->channels = (double*)malloc(sizeof(double)*30);
	data->num = (double*)malloc(sizeof(double)*30);
	p = fftw_plan_dft_r2c_1d(FRAMES_PER_BUFFER, data->in, data->out, FFTW_ESTIMATE);
	data->soundManager = this;
	volume = 0;
}

PaStream* SoundManager::getStream(){
	return stream;
}
