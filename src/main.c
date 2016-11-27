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

}

int leds[] = {
	LED1,
	LED2,
	LED3,
	LED4
};

int main(int argc,char **argv)
{
	int currled=0;
	CHIP_Init();

	// Set systick to 1ms interval
	if (SysTick_Config(CMU_ClockFreqGet(cmuClock_CORE) / 1000)) while (1);
	gpio_init();

	while(1)
	{
		pinout_t led = pins[leds[currled]];
		GPIO_PinOutToggle(led.port, led.pin);
		currled = (currled + 1) % 4;
		Delay(100);
	}

	return 0;
};
