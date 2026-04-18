/******************************************************************************
 * @file     main.c
 * @version  V1.00
 * $Revision: 1 $
 * $Date: 14/10/01 10:36a $
 * @brief    Template project for NUC029 series MCU
 *
 * @note
 * @copyright SPDX-License-Identifier: Apache-2.0
 * @copyright Copyright (C) 2014 Nuvoton Technology Corp. All rights reserved.
*****************************************************************************/
#include <stdio.h>
#include "NUC029xAN.h"
#include "switch.h"

extern char GetChar(void);
void sw_tes(void);
void delay(volatile uint32_t count)
{
    while(count--);
}
void SYS_Init(void)
{
    /* Unlock protected registers */
    SYS_UnlockReg();

    /* Enable IP clock */
    CLK->APBCLK = CLK_APBCLK_UART0_EN_Msk;

    /* Update System Core Clock */
    /* User can use SystemCoreClockUpdate() to calculate SystemCoreClock and CyclesPerUs automatically. */
    SystemCoreClockUpdate();

    /* Set P3 multi-function pins for UART0 RXD and TXD  */
    SYS->P3_MFP &= ~(SYS_MFP_P30_Msk | SYS_MFP_P31_Msk);
    SYS->P3_MFP |= (SYS_MFP_P30_RXD0 | SYS_MFP_P31_TXD0);

    /* Lock protected registers */
    SYS_LockReg();

}


int main(void)
{
    SYS_Init();
    UART_Open(UART0, 115200);

    SW_Init();                              

    printf("Switch Interrupt Demo\n\n");

    while (1)
    {
        SW_Process();

        for (uint8_t i = 0; i < TOTAL_SWITCHES; i++)
        {
            if (SW_GetState(i))
                printf("SW%d ON\n", i + 1);
        }

        //CLK_SysTickDelay(200000);           
    }
}


