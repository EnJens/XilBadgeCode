#include <stdint.h>
#include <stdbool.h>
#include "em_device.h"
#include "em_chip.h"
#include "em_cmu.h"
#include "hw_pinout.h"

volatile uint32_t msTicks; /* counts 1ms timeTicks */

void Delay(uint32_t dlyTicks);


void SysTick_Handler(void)
{
  msTicks++;       /* increment counter necessary in Delay()*/
}

void Delay(uint32_t dlyTicks)
{
  uint32_t curTicks;

  curTicks = msTicks;
  while ((msTicks - curTicks) < dlyTicks) ;
}

void Blink(uint32_t port,uint32_t pin, uint32_t dlyTicks)
{
	GPIO_PinOutToggle(port, pin);
	Delay(dlyTicks);
	GPIO_PinOutToggle(port, pin);
	Delay(dlyTicks);
}

int gpio_init(void)
{
	CMU_ClockEnable(cmuClock_HFPER, true);
	CMU_ClockEnable(cmuClock_GPIO, true);
	GPIO_PinModeSet(pins[LED1].port, pins[LED1].pin, gpioModePushPull, 0);
	GPIO_PinModeSet(pins[LED2].port, pins[LED2].pin, gpioModePushPull, 0);
	GPIO_PinModeSet(pins[LED3].port, pins[LED3].pin, gpioModePushPull, 0);
	GPIO_PinModeSet(pins[LED4].port, pins[LED4].pin, gpioModePushPull, 0);
	// Input pull, Out value sets to Pullup
	GPIO_PinModeSet(pins[USR1].port, pins[USR1].pin, gpioModeInputPull, 1);
	GPIO_PinModeSet(pins[USR2].port, pins[USR2].pin, gpioModeInputPull, 1);

	return 0;

}

int leds[] = {
	LED2,
	LED4,
	LED1,
	LED3,
};

int main()
{
	int currled=0;
	CHIP_Init();

	// Set systick to 1ms interval
	if (SysTick_Config(CMU_ClockFreqGet(cmuClock_CORE) / 1000)) while (1);
	gpio_init();

	while(1)
	{
		pinout_t led = pins[leds[currled]];



		if (!GPIO_PinInGet(pins[USR1].port, pins[USR1].pin))
		{
			Blink(led.port, led.pin, 500);

		}else{
			currled = (currled + 1) % 4;

			Blink(led.port, led.pin, 500);
		}
	}

	return 0;
};
