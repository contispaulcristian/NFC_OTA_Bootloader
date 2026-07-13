/******************************************************************************
 * @file      debug.h
 * @author    Paul Contis
 * @date      2026-07-13
 * @brief     Public API for the module
 * @details   Describe the architectural role and usage of this module.
 * @note      Target: ATtiny1616 | Clock: 20MHz
 * @copyright (c) 2026 DIY for Geeks. All rights reserved.
 ******************************************************************************/

#ifndef DEBUG_H
#define DEBUG_H

/* --- Standard Includes --- */
#include <stdint.h>
#include <avr/io.h>
#include <util/delay.h>
#include <stdbool.h>

/*----------------------------------------------------------------------------*
 * PUBLIC CONSTANTS & MACROS
 *----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*
 * PUBLIC TYPEDEFS & STRUCTURES
 *----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*
 * PUBLIC FUNCTION PROTOTYPES
 *----------------------------------------------------------------------------*/
void Blink_Debug(uint8_t count, uint16_t speed);

#endif /* DEBUG_H */
