/*
Created by Mauricio Narvaez
Dependencies:
PortAudio - sound input
FFTW3 - calculations
WiringPi - Led Control
*/

#include "SoundManager.h" //also includes ColorManager
#include "LedManager.h" //also includes ColorManager

int main(){
	LedManager *ledManager = new LedManager();
	ColorManager *colorManager = new ColorManager(ledManager);
	SoundManager *soundManager = new SoundManager(colorManager);
	ledManager->ledInit();

	/*initialize portaudio*/
	int code = soundManager->initAudio();
	if(code!=0) return code;

	//loop until closed
	PaError err = paNoError;
	while( ( err = Pa_IsStreamActive( soundManager->getStream() ) ) == 1 ) {
		Pa_Sleep(1000);
	}
	code = soundManager->closeAudio();
	if(code!=0) return code;
	printf("No errors.\n");
  return 0;
}
