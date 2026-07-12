/******************************************************************************
 * @file      port_drv.c
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
#include "port_drv.h"

/*----------------------------------------------------------------------------*
 * PRIVATE INCLUDES
 *----------------------------------------------------------------------------*/
#include <avr/io.h>
#include <stdbool.h>
#include <stdint.h>

/*----------------------------------------------------------------------------*
 * PRIVATE CONSTANTS & MACROS
 *----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*
 * PRIVATE TYPEDEFS & STRUCTURES
 *----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*
 * PRIVATE VARIABLES (STATIC DATA)
 *----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*
 * PRIVATE FUNCTION PROTOTYPES (STATIC)
 *----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*
 * PUBLIC FUNCTION IMPLEMENTATIONS
 *----------------------------------------------------------------------------*/
/**
 * @brief  Configures the direction and input features of a specific pin.
 * @param  port      Pointer to the PORT peripheral (e.g., &PORTA).
 * @param  pin_bm    Bitmask of the pin(s) to configure (e.g., PIN3_bm).
 * @param  is_output Set to true for Output, false for Input.
 * @param  pullup    Set to true to enable the internal pull-up resistor.
 */
void Mcal_Port_Configure(PORT_t *port, uint8_t pin_bm, bool is_output, bool pullup)
{
    if (is_output)
    {
        port->DIRSET = pin_bm;
    }
    else
    {
        port->DIRCLR = pin_bm;
    }

    /* Configure Input Sensing / Pull-up for each pin in the mask */
    for (uint8_t i = 0; i < 8; i++)
    {
        if (pin_bm & (1 << i))
        {
            /* Access the PINnCTRL register array directly */
            if (pullup)
            {
                (&(port->PIN0CTRL))[i] |= PORT_PULLUPEN_bm;
            }
            else
            {
                (&(port->PIN0CTRL))[i] &= ~PORT_PULLUPEN_bm;
            }
        }
    }
    return;
}

/**
 * @brief  Writes a digital value to a specific output pin.
 * @param  port   Pointer to the PORT peripheral.
 * @param  pin_bm Bitmask of the pin to modify.
 * @param  state  true for HIGH (VCC), false for LOW (GND).
 */
void Mcal_Port_Write_Pin(PORT_t *port, uint8_t pin_bm, bool state)
{
    if (state)
    {
        port->OUTSET = pin_bm;
    }
    else
    {
        port->OUTCLR = pin_bm;
    }
}

/**
 * @brief  Toggles the state of an output pin.
 * @param  port   Pointer to the PORT peripheral.
 * @param  pin_bm Bitmask of the pin to toggle.
 */
void Mcal_Port_Toggle_Pin(PORT_t *port, uint8_t pin_bm)
{
    port->OUTTGL = pin_bm;
}

/**
 * @brief  Reads the current physical state of a pin.
 * @param  port   Pointer to the PORT peripheral.
 * @param  pin_bm Bitmask of the pin to read.
 * @return true if the pin is HIGH, false if LOW.
 */
bool Mcal_Port_Read_Pin(PORT_t *port, uint8_t pin_bm)
{
    return (port->IN & pin_bm);
}

/**
 * @brief  Reads an entire 8-bit port.
 * @param  port Pointer to the PORT peripheral.
 * @return uint8_t The raw value of the PORTx.IN register.
 */
uint8_t Mcal_Port_Read_Raw(PORT_t *port)
{
    return (port->IN);
}

/*----------------------------------------------------------------------------*
 * PRIVATE FUNCTION IMPLEMENTATIONS
 *----------------------------------------------------------------------------*/
