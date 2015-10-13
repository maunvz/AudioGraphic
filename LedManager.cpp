#include "LedManager.h"

#define LED_MAX 100.0

#define R_PIN_0 7
#define G_PIN_0 9
#define B_PIN_0 8

#define R_PIN_1 0
#define G_PIN_1 2
#define B_PIN_1 3

void LedManager::setLedColors(Color channel_0, Color channel_1) {
	#ifdef USE_LEDS
	softPwmWrite(R_PIN_0, channel_0.r*channel_0.a*LED_MAX);
	softPwmWrite(G_PIN_0, channel_0.g*channel_0.a*LED_MAX);
	softPwmWrite(B_PIN_0, channel_0.b*channel_0.a*LED_MAX);

	softPwmWrite(R_PIN_1, channel_1.r*channel_1.a*LED_MAX);
	softPwmWrite(G_PIN_1, channel_1.g*channel_1.a*LED_MAX);
	softPwmWrite(B_PIN_1, channel_1.b*channel_1.a*LED_MAX);
	#endif
}

void LedManager::ledInit() {
	#ifdef USE_LEDS
	/*initialize led things*/
	wiringPiSetup();

	/* Set up leds */
	softPwmCreate(R_PIN_0, 0, LED_MAX);
	softPwmCreate(G_PIN_0, 0, LED_MAX);
	softPwmCreate(B_PIN_0, 0, LED_MAX);

	softPwmCreate(R_PIN_1, 0, LED_MAX);
	softPwmCreate(G_PIN_1, 0, LED_MAX);
	softPwmCreate(B_PIN_1, 0, LED_MAX);
	#endif
}
