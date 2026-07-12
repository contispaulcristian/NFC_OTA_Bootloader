/******************************************************************************
 * @file      port_drv.h
 * @author    Paul Contis
 * @date      2026-03-09
 * @brief     Public API for the module
 * @details   Describe the architectural role and usage of this module.
 * @note      Target: ATtiny1616 | Clock: 10MHz
 * @copyright (c) 2026 DIY for Geeks. All rights reserved.
 ******************************************************************************/

#ifndef PORT_DRV_H
#define PORT_DRV_H

/* --- Standard Includes --- */
#include <avr/io.h>
#include <stdbool.h>
#include <stdint.h>
/*----------------------------------------------------------------------------*
 * PUBLIC CONSTANTS & MACROS
 *----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*
 * PUBLIC TYPEDEFS & STRUCTURES
 *----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*
 * PUBLIC FUNCTION PROTOTYPES
 *----------------------------------------------------------------------------*/
void    Mcal_Port_Configure(PORT_t *port, uint8_t pin_bm, bool is_output, bool pullup);
void    Mcal_Port_Write_Pin(PORT_t *port, uint8_t pin_bm, bool state);
void    Mcal_Port_Toggle_Pin(PORT_t *port, uint8_t pin_bm);
bool    Mcal_Port_Read_Pin(PORT_t *port, uint8_t pin_bm);
uint8_t Mcal_Port_Read_Raw(PORT_t *port);

#endif /* PORT_DRV_H */
