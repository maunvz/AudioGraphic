/*
Created by Mauricio Narvaez
*/

#include "SoundManager.h"
#include "LedManager.h"
#include "ColorManager.h"
#include "GraphicsManager.h"

int main(){
	LedManager *ledManager = new LedManager();
	GraphicsManager *graphicsManager = new GraphicsManager();
	ColorManager *colorManager = new ColorManager(ledManager, graphicsManager);
	SoundManager *soundManager = new SoundManager(colorManager, graphicsManager);
	ledManager->ledInit();

	/*initialize portaudio*/
	int code = soundManager->initAudio();
	if(code!=0) return code;

	//loop until closed
	PaError err = paNoError;
	while( ( err = Pa_IsStreamActive( soundManager->getStream() ) ) == 1 ) {
		Pa_Sleep(10);
		if(!graphicsManager->manageWindow()) break;
	}
	code = soundManager->closeAudio();
	graphicsManager->destroy();
	colorManager->destroy();

	if(code!=0) return code;
	printf("No errors.\n");
  return 0;
}
