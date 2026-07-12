/******************************************************************************
 * @file      fuses.c
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
//#include "fuses.h"
/*----------------------------------------------------------------------------*
 * PRIVATE INCLUDES
 *----------------------------------------------------------------------------*/
#include <avr/io.h>
#include <avr/fuse.h>
#include "config.h"
/*----------------------------------------------------------------------------*
 * PRIVATE CONSTANTS & MACROS
 *----------------------------------------------------------------------------*/
/* Configuration for Secure Bootloader */
FUSES = {
    //.WDTCFG = FUSE_WDP_8MS_gc,      // Watchdog: Useful for forcing reset if NFC fails ?TBD
    //.BODCFG = FUSE_ACTIVE_ENABLED_gc | FUSE_LVL_BODLEVEL0_gc, // Brown-out: Protects Flash
    //.OSCCFG = FUSE_FREQSEL_20MHZ_gc, // Set internal oscillator to 20MHz for Ed25519 ? TBD
    /***********test */
    .WDTCFG     = 0x00, /* Watchdog Timer: OFF */
    .BODCFG     = 0x00, /* Brown-out Detector: OFF */
    /* 0x02 = 20 MHz (Bits 1:0 are 10) */
    .OSCCFG     = 0x02,
    .reserved_1 = { 0x00 },
    .TCD0CFG    = 0x00,
    /* SYSCFG0
     * 0x04 in binary is 00000100.+
     * Bits [3:2] are the RSTPINCFG.
     * '01' means "UPDI PIN ENABLED".
     * '00' would mean GPIO (Bricked).
     * By hardcoding 0x04 - permanently guarantee UPDI stays open. */
    .SYSCFG0    = 0x04,
    /* 0x07 means 64ms startup delay (safest for stable power) */
    .SYSCFG1    = 0x07,
    /* --- Partitioning Logic --- */
    .BOOTEND    = 0x00, //BOOTEND_VALUE,                 4KB Bootloader (16 * 256 bytes)
    .APPEND     = 0x00  //APPEND_VALUE                Metadata starts at last 512 bytes (62 * 256)

};

// SECURITY
// .LOCKBIT = FUSE_LB_RWLOCK_gc, // Prevents reading or writing via UPDI

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

/*----------------------------------------------------------------------------*
 * PRIVATE FUNCTION IMPLEMENTATIONS
 *----------------------------------------------------------------------------*/
