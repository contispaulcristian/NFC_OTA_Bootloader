/******************************************************************************
 * @file      main.c
 * @author    Paul Contis
 * @date      2026-03-08
 * @brief     Implementation of the module logic
 * @details   Detailed logic and hardware interaction notes.
 * @note      Target: ATtiny1616
 * @copyright (c) 2026 DIY for Geeks. All rights reserved.
 ******************************************************************************/

/*----------------------------------------------------------------------------*
 * MODULE HEADER
 *----------------------------------------------------------------------------*/
//#include "main.h"

/*----------------------------------------------------------------------------*
 * PRIVATE INCLUDES
 *----------------------------------------------------------------------------*/
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

/* Layered Includes */
#include "config.h"

#include "clk_drv.h"
#include "io_map.h"
#include "i2c_drv.h"
#include "nvm_drv.h"
#include "st25dv64k_driver.h"
#include "port_drv.h"
#include "debug.h"

/*----------------------------------------------------------------------------*
 * PRIVATE CONSTANTS & MACROS
 *----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*
 * PRIVATE TYPEDEFS & STRUCTURES
 *----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*
 * PRIVATE VARIABLES (STATIC DATA)
 *----------------------------------------------------------------------------*/
/* Volatile flag for the ISR */
volatile bool mailbox_event_pending = false;

/* ISR for the GPO pin (e.g., PA0) */
ISR(PORTA_PORT_vect)
{
    /* Check if the interrupt is from our GPO pin (PA3) */
    if (PORTA.INTFLAGS & PIN3_bm)
    {
        /* Clear the interrupt flag */
        PORTA.INTFLAGS = PIN3_bm;

        /* Notify main loop */
        mailbox_event_pending = true;
    }
}

/*----------------------------------------------------------------------------*
 * MAIN FUNCTION
 *----------------------------------------------------------------------------*/
int main(void)
{
    /* Hardware Initialization */
    Mcal_Clk_Init();

    /* DEBUG */
    Mcal_Port_Configure(&PORTA, PIN5_bm, true, false);
    Mcal_Port_Write_Pin(&PORTA, PIN5_bm, true);

    Mcal_I2c_Init();

    uint8_t rx_buffer[64]; // Page buffer for NVM

    // const uint8_t nvm_buffer[64] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A,
    //                                  0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10, 0x11, 0x12, 0x13, 0x14,
    //                                  0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E,
    //                                  0x1F, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28,
    //                                  0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F, 0x30, 0x31, 0x32,
    //                                  0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C,
    //                                  0x3D, 0x3E, 0x3F, 0x40 }; // Page buffer from NVM

    // Configure PA3 as input, falling edge interrupt
    PORTA.PIN3CTRL = PORT_ISC_FALLING_gc;

    cli();
    CCP          = 0xD8;            /* CCP_IOREG_gc: Unlock system registers */
    CPUINT.CTRLA = CPUINT_IVSEL_bm; /* Set IVSEL bit to point to Bootloader IVT */

    sei(); // Enable global interrupts

    while (1)
    {
        if (mailbox_event_pending)
        {
            /* Read data from Mailbox */
            if (St25Dv_Read_Mailbox(rx_buffer, 64) == ST25_OK)
            {
                //if ((Mcal_Nvm_Erase_Page(0x3000U)) == NVM_OK)
                {
                    //Blink_Debug(4, 60);
                    /* Write to a safe NVM area (e.g., 0x3000) */
                    if ((Mcal_Nvm_Write_Page(0x3000, rx_buffer) == NVM_OK))
                    {
                        Blink_Debug(4, 1000);
                    }
                }
                //if ((Mcal_Nvm_Verify_Page(0x3000U, nvm_buffer)) == NVM_OK)
                {
                    //Blink_Debug(5, 600);
                }
            }

            /* Reset flag */
            mailbox_event_pending = false;
        }
    }
}
