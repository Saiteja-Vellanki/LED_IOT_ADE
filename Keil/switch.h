#ifndef __SWITCH_H__
#define __SWITCH_H__


#include "NUC029xAN.h"
#include <stdio.h>

#ifdef NEW_BOARD
#define TOTAL_SWITCHES   24

#else
#define TOTAL_SWITCHES   3
#endif

#define GPIO_READ(port, pin)   (((port)->PIN >> (pin)) & 0x1U)
#define CLK_AHBCLK_GPIO_EN   (1<<2)

#define SW_PRESSED             (0U)  
#define SW_RELEASED            (1U)

typedef struct 
{
	GPIO_T *port;
	uint8_t pin;
	
}SW_t;



void SW_Init(void);
uint8_t SW_Read(uint8_t id);
uint8_t SW_GetEvent(uint8_t id);
uint8_t SW_GetState(uint8_t id);
void SW_Process(void);


#endif