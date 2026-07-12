/******************************************************************************
 * @file      st25dv64k_driver.h
 * @author    Paul Contis
 * @date      2026-03-11
 * @brief     Public API for the module
 * @details   Describe the architectural role and usage of this module.
 * @note      Target: ATtiny1616
 * @copyright (c) 2026 DIY for Geeks. All rights reserved.
 ******************************************************************************/

#ifndef ST25DV64K_DRIVER_H
#define ST25DV64K_DRIVER_H

/* --- Standard Includes --- */
#include <stdint.h>
#include <stdbool.h>

/*----------------------------------------------------------------------------*
 * PUBLIC CONSTANTS & MACROS
 *----------------------------------------------------------------------------*/
/* --- ST25DV FTM Dynamic Registers --- */
#define ST25DV_REG_GPO_DYN     0x2000U
#define ST25DV_REG_MB_CTRL_DYN 0x2029U
#define ST25DV_REG_MB_FIFO     0x202BU

/*----------------------------------------------------------------------------*
 * PUBLIC TYPEDEFS & STRUCTURES
 *----------------------------------------------------------------------------*/
typedef enum
{
    ST25_OK = 0U,
    ST25_ERR_COMM,       /* Transport layer failure */
    ST25_ERR_MAILBOX,    /* Mailbox protocol violation */
    ST25_ERR_TIMEOUT,    /* Hardware did not respond */
    ST25_ERR_INVALID_CFG /* System register configuration failure */
} St25_Status_t;

/*----------------------------------------------------------------------------*
 * PUBLIC FUNCTION PROTOTYPES
 *----------------------------------------------------------------------------*/
St25_Status_t ST25DV_Init(void);

St25_Status_t ST25DV_Read_Mailbox(uint8_t *Buffer, uint16_t Len);
St25_Status_t St25_Write_Ack(uint8_t Data);

#endif /* ST25DV64K_DRIVER_H */
