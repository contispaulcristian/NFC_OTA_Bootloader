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
/* System Registers */
#define HAL_ST25_GPO_CTRL_REG     0x0002U
#define HAL_ST25_MAILBOX_CTRL_REG 0x0003U

/* Dynamic Registers */
//#define HAL_ST25DV_REG_GPO_DYN    0x2000U
#define HAL_ST25_HOST_PUT_MSG_REG 0x2008U
#define HAL_ST25_MBLEN_DYN_REG    0x2006U

/* Bitmasks */
#define ST25_MB_EN_BIT            0x01U
#define ST25_GPO_EN_BIT           0x01U
#define ST25_MB_MSG_ON_BIT        0x02U

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

St25_Status_t St25Dv_Read_Mailbox(uint8_t *Buffer, uint16_t Len);
St25_Status_t St25Dv_Write_Ack(uint8_t Data);

bool St25_Is_Data_Available(void);

#endif /* ST25DV64K_DRIVER_H */
