#ifndef _HW_PINOUT_H_
#define _HW_PINOUT_H_

#include <stdint.h>

typedef struct {
    uint32_t port;
	uint32_t pin;
} pinout_t;

enum {
    LED1,
	LED2,
	LED3,
	LED4,
	USR1,
	USR2,
	CAP_S_1,
	CAP_S_2,
};

extern const pinout_t pins[];

#endif /* _HW_PINOUT_H_ */
