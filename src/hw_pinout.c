#include "hw_pinout.h"
#include <em_gpio.h>

const pinout_t pins[] = {
	[LED1] = { gpioPortE, 12 },
	[LED2] = { gpioPortF, 2 },
	[LED3] = { gpioPortD, 7},
	[LED4] = { gpioPortB, 11 },
	[USR1] = { gpioPortD, 6 },
	[USR2] = { gpioPortA, 0 },
	[CAP_S_1] = { gpioPortC, 15 },
	[CAP_S_2] = { gpioPortC, 14 },
};

const uint32_t pin_count = sizeof(pins) / sizeof(pinout_t);
