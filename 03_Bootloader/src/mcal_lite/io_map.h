/******************************************************************************
 * @file      io_map.h
 * @author    Paul Contis
 * @date      2026-03-09
 * @brief     Master Pin Mapping
 * @details   This file maps physical MCU pins to functional system names.
 * @note      Target: ATtiny1616 | Clock: 10MHz
 * @copyright (c) 2026 DIY for Geeks. All rights reserved.
 ******************************************************************************/

#ifndef IO_MAP_H
#define IO_MAP_H

/* --- Standard Includes --- */
#include <avr/io.h>

/*----------------------------------------------------------------------------*
 * PUBLIC CONSTANTS & MACROS
 *----------------------------------------------------------------------------*/
/**
 * @name I2c Port & Pin Definitions
 * @{
 */
#define I2C_COMM_PORT PORTA
#define I2C_SDA_PIN   PIN1_bm /**< I2C Data Pin (Alternative Mux) */
#define I2C_SCL_PIN   PIN2_bm /**< I2C Clock Pin (Alternative Mux) */
/** @} */

/**
 * @name NFC Port & Pin Definitions
 * @{
 */
#define NFC_GPO_PORT  PORTA
#define NFC_GPO_PIN   PIN3_bm /**< NFC Open-Drain Interrupt Input (Active LOW) */

#define NFC_LPD_PORT  PORTB
#define NFC_LPD_PIN   PIN3_bm
/** @} */

/**
 * @name Navigation Button Definitions
 * @{
 */
#define BTN_DPAD_PORT PORTA
#define BTN_UP_PIN    PIN4_bm /**< Navigation UP (Active LOW) */
#define BTN_DOWN_PIN  PIN5_bm /**< Navigation DOWN (Active LOW) */
#define BTN_LEFT_PIN  PIN6_bm /**< Navigation LEFT (Active LOW) */
#define BTN_RIGHT_PIN PIN7_bm /**< Navigation RIGHT (Active LOW) */

/* Helper Mask for all navigation buttons */
#define BTN_MASK_ALL  (PIN_BTN_UP | PIN_BTN_DOWN | PIN_BTN_LEFT | PIN_BTN_RIGHT)
/** @} */

/**
 * @name Power & Drivers
 * @{
 */
#define PORT_SYS      PORTB
#define LED_SDB_PIN   PIN2_bm /* Shutdown pin for IS31 */
#define LDO_LATCH_PIN PIN4_bm /* Power hold for VCC */
/** @} */
/*----------------------------------------------------------------------------*
 * PUBLIC TYPEDEFS & STRUCTURES
 *----------------------------------------------------------------------------*/

#endif /* IO_MAP_H */
