#include "switch.h"

#ifdef NEW_BOARD
static const SW_t sw_list[TOTAL_SWITCHES] =
{
    {P0, 0}, {P0, 1}, {P0, 2}, {P0, 3},
    {P0, 5}, {P0, 6}, {P0, 7},
    {P1, 0}, {P1, 1}, {P1, 4}, {P1, 5}, {P1, 6}, {P1, 7},
    {P0, 0}, {P2, 1}, {P2, 2}, {P2, 3},
    {P2, 4}, {P2, 5}, {P2, 6}, {P2, 7},
    {P3, 1}, {P3, 2}, {P3, 3},
};

static volatile uint8_t sw_event[TOTAL_SWITCHES] = {0};
static uint8_t          sw_state[TOTAL_SWITCHES] = {0};

uint8_t SW_Read(uint8_t id)
{
    return GPIO_READ(sw_list[id].port, sw_list[id].pin);
}

uint8_t SW_GetEvent(uint8_t id)
{
    uint8_t ev = sw_event[id];
    sw_event[id] = 0u;
    return ev;
}

uint8_t SW_GetState(uint8_t id)
{
    return sw_state[id];
}

void SW_Init(void)
{
    CLK->AHBCLK |= (1u << 2u);

    SYS->REGWRPROT = 0x59u;
    SYS->REGWRPROT = 0x16u;
    SYS->REGWRPROT = 0x88u;

   
    SYS->P0_MFP &= ~( (1u<<0)|(1u<<1)|(1u<<2)|(1u<<3)|(1u<<4)|
                      (1u<<5)|(1u<<6)|(1u<<7)|
                      (1u<<8)|(1u<<9)|(1u<<10)|(1u<<11)|(1u<<12)|
                      (1u<<13)|(1u<<14)|(1u<<15) );

    SYS->P1_MFP &= ~( (1u<<0)|(1u<<1)|(1u<<4)|(1u<<5)|(1u<<6)|(1u<<7)|
                      (1u<<8)|(1u<<9)|(1u<<12)|(1u<<13)|(1u<<14)|(1u<<15) );

    SYS->P2_MFP &= ~( (1u<<0)|(1u<<1)|(1u<<2)|(1u<<3)|
                      (1u<<4)|(1u<<5)|(1u<<6)|(1u<<7)|
                      (1u<<8)|(1u<<9)|(1u<<10)|(1u<<11)|
                      (1u<<12)|(1u<<13)|(1u<<14)|(1u<<15) );

    SYS->P3_MFP &= ~( (1u<<1)|(1u<<2)|(1u<<3)|
                      (1u<<9)|(1u<<10)|(1u<<11) );

    SYS->REGWRPROT = 0x00u;


    P2->PMD &= ~(0x3u << 0);
    P2->PMD |=  (0x1u << 0);
    P2->DOUT &= ~(1u << 0);

    for (uint8_t i = 0; i < TOTAL_SWITCHES; i++)
    {
        GPIO_T *port = sw_list[i].port;
        uint8_t pin  = sw_list[i].pin;

        
        port->PMD &= ~(0x3u << (pin * 2u));

      
        port->IMD &= ~(1u << pin);


        port->IEN |= (1u << (16u + pin));

       
        port->DBEN |= (1u << pin);
    }

   
    GPIO->DBNCECON = 0x00000006u;

    P0->ISRC = 0xFFFFFFFFu;
    P1->ISRC = 0xFFFFFFFFu;
    P2->ISRC = 0xFFFFFFFFu;
    P3->ISRC = 0xFFFFFFFFu;

    NVIC_EnableIRQ(GPIO_P0P1_IRQn);
    NVIC_EnableIRQ(GPIO_P2P3P4_IRQn);
}

void SW_Process(void)
{
    static uint8_t relay_state = 0u;
    uint8_t any_event = 0u;

    for (uint8_t i = 0; i < TOTAL_SWITCHES; i++)
    {
        if (SW_GetEvent(i))
        {
            sw_state[i] ^= 1u;
            any_event = 1u;
        }
    }

    if (any_event)
    {
        relay_state ^= 1u;
        if (relay_state)
            P2->DOUT |=  (1u << 0u);
        else
            P2->DOUT &= ~(1u << 0u);
    }
}


void GPIOP0P1_IRQHandler(void)
{
    uint32_t s0 = P0->ISRC;
    uint32_t s1 = P1->ISRC;
    P0->ISRC = s0;
    P1->ISRC = s1;

    for (uint8_t i = 0; i < TOTAL_SWITCHES; i++)
    {
        if (sw_list[i].port == P0 && (s0 & (1u << sw_list[i].pin)))
            sw_event[i] = 1u;

        if (sw_list[i].port == P1 && (s1 & (1u << sw_list[i].pin)))
            sw_event[i] = 1u;
    }
}

void GPIOP2P3P4_IRQHandler(void)
{
    uint32_t s2 = P2->ISRC;
    uint32_t s3 = P3->ISRC;
    P2->ISRC = s2;
    P3->ISRC = s3;

    for (uint8_t i = 0; i < TOTAL_SWITCHES; i++)
    {
        if (sw_list[i].port == P2 && (s2 & (1u << sw_list[i].pin)))
            sw_event[i] = 1u;

        if (sw_list[i].port == P3 && (s3 & (1u << sw_list[i].pin)))
            sw_event[i] = 1u;
    }
}

#else






static const SW_t sw_list[TOTAL_SWITCHES] =
{
    {P2, 5}, {P2, 6}, {P2, 7},
};

static volatile uint8_t sw_event[TOTAL_SWITCHES] = {0};
static uint8_t          sw_state[TOTAL_SWITCHES] = {0};

uint8_t SW_Read(uint8_t id)
{
    return GPIO_READ(sw_list[id].port, sw_list[id].pin);
}

uint8_t SW_GetEvent(uint8_t id)
{
    uint8_t ev = sw_event[id];
    sw_event[id] = 0u;
    return ev;
}

uint8_t SW_GetState(uint8_t id)
{
    return sw_state[id];
}

void SW_Init(void)
{
    CLK->AHBCLK |= (1u << 2u);

    SYS->REGWRPROT = 0x59u;
    SYS->REGWRPROT = 0x16u;
    SYS->REGWRPROT = 0x88u;

    SYS->P2_MFP = 0x00000000u;

    SYS->REGWRPROT = 0x00u;

    /* P2.0 — relay output, OFF */
    P2->PMD  &= ~(0x3u << 0);
    P2->PMD  |=  (0x1u << 0);
    P2->DOUT &= ~(1u << 0);

    /* P2.5, P2.6, P2.7 — input, rising edge, debounce */
    for (uint8_t i = 0; i < TOTAL_SWITCHES; i++)
    {
        uint8_t pin = sw_list[i].pin;

        P2->PMD  &= ~(0x3u << (pin * 2u));      /* input */
        P2->IMD  &= ~(1u << pin);               /* edge mode */
        P2->IEN  |=  (1u << (16u + pin));       /* rising edge */
        P2->DBEN |=  (1u << pin);               /* debounce */
    }

    GPIO->DBNCECON = 0x00000006u;               /* HCLK /64 */

    P2->ISRC = 0xFFFFFFFFu;

    NVIC_EnableIRQ(GPIO_P2P3P4_IRQn);
}

void SW_Process(void)
{
    static uint8_t relay_state = 0u;

    for (uint8_t i = 0; i < TOTAL_SWITCHES; i++)
    {
        if (SW_GetEvent(i))
        {
            relay_state ^= 1u;

            if (relay_state)
                P2->DOUT |=  (1u << 0u);
            else
                P2->DOUT &= ~(1u << 0u);

            /* clear all other pending events to prevent
               multiple toggles from one physical press  */
            for (uint8_t j = 0; j < TOTAL_SWITCHES; j++)
                sw_event[j] = 0u;

            return;
        }
    }
}

void GPIOP2P3P4_IRQHandler(void)
{
    uint32_t s2 = P2->ISRC;
    P2->ISRC = s2;

    for (uint8_t i = 0; i < TOTAL_SWITCHES; i++)
    {
        if (s2 & (1u << sw_list[i].pin))
            sw_event[i] = 1u;
    }
}

#endif