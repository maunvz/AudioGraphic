#ifndef led_def
#define led_def

#ifdef USE_LEDS
#include <wiringPi.h>
#include <softPwm.h>
#endif

#include "ColorManager.h"

class LedManager {
public:
	void setLedColors(Color channel1, Color channel2);
	void ledInit();
};

#endif
