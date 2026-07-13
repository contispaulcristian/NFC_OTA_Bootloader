/******************************************************************************
 * @file      clk_drv.c
 * @author    Paul Contis
 * @date      2026-03-08
 * @brief     Clock Management Driver.
 * @details   Handles the initialization of the Internal High-Frequency
 *            Oscillator (OSC20M) and manages the Main Clock Prescaler
 *            to achieve 10MHz operation.
 * @note      Target: ATtiny1616
 * @copyright (c) 2026 DIY for Geeks. All rights reserved.
 ******************************************************************************/

/*----------------------------------------------------------------------------*
 * MODULE HEADER
 *----------------------------------------------------------------------------*/
#include "clk_drv.h"

/*----------------------------------------------------------------------------*
 * PRIVATE INCLUDES
 *----------------------------------------------------------------------------*/
#include <avr/io.h>
#include <avr/cpufunc.h> /* Required for safely writing CCP registers */

/*----------------------------------------------------------------------------*
 * PRIVATE CONSTANTS & MACROS
 *----------------------------------------------------------------------------*/
/* 20 MHz Internal Oscillator divided by 2 = 10 MHz */
#define MCAL_CLK_10MHZ_PRESCALER (CLKCTRL_PDIV_2X_gc | CLKCTRL_PEN_bm)

/*----------------------------------------------------------------------------*
 * PUBLIC FUNCTION IMPLEMENTATIONS
 *----------------------------------------------------------------------------*/
/**
 * @brief  Initializes the system clock to 10 MHz
 * @note   This is the safe frequency for VDD = 3.3V.
 * @return void
 */
void Mcal_Clk_Init(void)
{
    /* Safely write the prescaler to MCLKCTRLB to achieve 10 MHz */
    ccp_write_io((void *)&CLKCTRL.MCLKCTRLB, MCAL_CLK_10MHZ_PRESCALER);

    /* Wait for the oscillator status to confirm stability */
    /* Note: OSC20M is usually stable from power-on, but verifying
       the status bit is best-practice before enabling communication buses. */
    while (!(CLKCTRL.MCLKSTATUS & CLKCTRL_OSC20MS_bm))
    {
        /* Wait for clock to stabilize */
    }
}
