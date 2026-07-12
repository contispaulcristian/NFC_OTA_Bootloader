/******************************************************************************
 * @file      nvm_drv.h
 * @author    Paul Contis
 * @date      2026-03-12
 * @brief     Public API for the module
 * @details   Describe the architectural role and usage of this module.
 * @note      Target: ATtiny1616 | Clock: 20MHz
 * @copyright (c) 2026 DIY for Geeks. All rights reserved.
 ******************************************************************************/

#ifndef NVM_DRV_H
#define NVM_DRV_H

/* --- Standard Includes --- */
#include <stdint.h>

/*----------------------------------------------------------------------------*
 * PUBLIC CONSTANTS & MACROS
 *----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*
 * PUBLIC TYPEDEFS & STRUCTURES
 *----------------------------------------------------------------------------*/
typedef enum
{
    NVM_OK = 0U,
    NVM_ERR_INVALID_ADDR,
    NVM_ERR_WRITE_FAILED,
    NVM_ERR_VERIFY_FAILED,
    NVM_ERR_BUSY
} Nvm_Status_t;

/*----------------------------------------------------------------------------*
 * PUBLIC FUNCTION PROTOTYPES
 *----------------------------------------------------------------------------*/
Nvm_Status_t Mcal_Nvm_Erase_Page(uint16_t Address);
Nvm_Status_t Mcal_Nvm_Write_Page(uint16_t Address, const uint8_t *Data);
Nvm_Status_t Mcal_Nvm_Verify_Page(uint16_t Address, const uint8_t *Expected_Data);

#endif /* NVM_DRV_H */
