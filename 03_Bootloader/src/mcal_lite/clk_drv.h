/******************************************************************************
 * @file      clk_drv.h
 * @author    Paul Contis
 * @date      2026-03-08
 * @brief     Public API for the module
 * @details   Describe the architectural role and usage of this module.
 * @note      Target: ATtiny1616
 * @copyright (c) 2026 DIY for Geeks. All rights reserved.
 ******************************************************************************/

#ifndef CLK_DRV_H
#define CLK_DRV_H

/*----------------------------------------------------------------------------*
 * PUBLIC CONSTANTS & MACROS
 *----------------------------------------------------------------------------*/
/** * @brief System Clock Frequency in Hz.
 * Used by <util/delay.h> and other timing-dependent drivers.
 */
#ifndef F_CPU
#define F_CPU 10000000UL /* 10 MHz - Safe for Vdd = 3.3V */
#endif

/*----------------------------------------------------------------------------*
 * PUBLIC FUNCTION PROTOTYPES
 *----------------------------------------------------------------------------*/
void Mcal_Clk_Init(void);

#endif /* CLK_DRV_H */
