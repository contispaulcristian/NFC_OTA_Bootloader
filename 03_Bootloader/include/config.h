/******************************************************************************
 * @file      config.h
 * @author    Paul Contis
 * @date      2026-03-08
 * @brief     Public API for the module
 * @details   Describe the architectural role and usage of this module.
 * @note      Target: ATtiny1616 | Clock: 20MHz
 * @copyright (c) 2026 DIY for Geeks. All rights reserved.
 ******************************************************************************/

#ifndef CONFIG_H
#define CONFIG_H

/* --- Standard Includes --- */
#include <stdint.h>
#include <stdbool.h>

/*----------------------------------------------------------------------------*
 * PUBLIC CONSTANTS & MACROS
 *----------------------------------------------------------------------------*/

/* --- Bootloader Memory Map --- */
/*--------------------------------------------------------------
 * Flash Memory Configuration
 *-------------------------------------------------------------*/
/* ATtiny1616: 16KB Flash starting at 0x8000 */
#define FLASH_START_ADDR      0x8000U

#define FLASH_SIZE_BYTES      (16384U)
#define FLASH_PAGE_SIZE       (64U)
/* First 4KB (0x1000 bytes) reserved for this bootloader. */
#define BOOTLOADER_SIZE_BYTES (4096U)

#define APP_START_ADDR        0x9000U /* Unified Address for Jump FLASH_START_ADDR +0x1000 (4kb)*/

#define BOOTEND_VALUE         (BOOTLOADER_SIZE_BYTES / 256) /* 0x10U - (4096 / 256) = 16 blocks */
#define APPEND_VALUE          0x38U                         /* (14336 / 256) = 56 blocks */

/* ATtiny1616: 256Bytes EEPROM starting at 0x1400 */
//#define EEPROM_START_ADDR      0x1400U
//#define EEPROM_PAGE_SIZE       32U      /* Hardware page size for ATtiny1616 */

/* --- UI LED Matrix Settings --- */
#define IS31FL3730_ADDR       0x60U /* I2c Address for 8x8 Led Matrix driver IC */

/* --- NFC & Transfer Settings --- */
#define CHUNK_SIZE            256U  /* ST25DV FTM Buffer = 2 Flash Pages */
#define ST25DV_SYS_ADDR       0x57U /* I2C Address for Mailbox/Registers */
#define ST25DV_USER_ADDR      0x53U /* I2C Address for User EEPROM */

/* --- Security Definitions --- */
//#define PUBLIC_KEY_SIZE        32U      /* Ed25519 Public Key length */
//#define SIGNATURE_SIZE         64U      /* Ed25519 Signature length */

/*----------------------------------------------------------------------------*
 * PUBLIC TYPEDEFS & STRUCTURES
 *----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*
 * CONFIGURATION VALIDATION
 *----------------------------------------------------------------------------*/
#ifndef __AVR_ATtiny1616__
#error "This driver is intended for the ATtiny1616."
#endif

#if (FLASH_PAGE_SIZE != 64U)
#error "Invalid FLASH_PAGE_SIZE."
#endif

#if ((BOOTLOADER_SIZE_BYTES % FLASH_PAGE_SIZE) != 0U)
#error "BOOTLOADER_SIZE_BYTES must be page aligned."
#endif

#if (BOOTLOADER_SIZE_BYTES >= FLASH_SIZE_BYTES)
#error "BOOTLOADER_SIZE_BYTES exceeds Flash memory."
#endif

#if (BOOTLOADER_SIZE_BYTES == 0U)
#error "BOOTLOADER_SIZE_BYTES cannot be zero."
#endif

#endif /* CONFIG_H */
