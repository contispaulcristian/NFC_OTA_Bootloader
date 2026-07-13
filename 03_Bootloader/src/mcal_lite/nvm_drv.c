/******************************************************************************
 * @file      nvm_drv.c
 * @author    Paul Contis
 * @date      2026-03-12
 * @brief     Implementation of the module logic
 * @details   Detailed logic and hardware interaction notes.
 * @note      Target: ATtiny1616
 * @copyright (c) 2026 DIY for Geeks. All rights reserved.
 ******************************************************************************/

/*----------------------------------------------------------------------------*
 * MODULE HEADER
 *----------------------------------------------------------------------------*/
#include "nvm_drv.h"

/*----------------------------------------------------------------------------*
 * PRIVATE INCLUDES
 *----------------------------------------------------------------------------*/
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stddef.h>
#include <avr/cpufunc.h> /* Required for _PROTECTED_WRITE */
#include "config.h"

#include "../debug.h"

/*----------------------------------------------------------------------------*
 * PRIVATE CONSTANTS & MACROS
 *----------------------------------------------------------------------------*/
/* Timeout for safety */
#define MCAL_NVM_TIMEOUT         0xFFFFU

#define MCAL_NVM_BOOT_SIZE_LIMIT 0x3000
#define MCAL_NVM_PAGE_SIZE       FLASH_PAGE_SIZE

/*----------------------------------------------------------------------------*
 * PRIVATE TYPEDEFS & STRUCTURES
 *----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*
 * PRIVATE VARIABLES (STATIC DATA)
 *----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*
 * PRIVATE FUNCTION PROTOTYPES (STATIC)
 *----------------------------------------------------------------------------*/
static Nvm_Status_t Mcal_Nvm_Execute_Cmd(uint8_t Cmd);

/*----------------------------------------------------------------------------*
 * PUBLIC FUNCTION IMPLEMENTATIONS
 *----------------------------------------------------------------------------*/
/**
 * @brief  Erases a 64-byte page in Flash memory.
 * @param Address  Address Flash address to erase (Must be 64-byte page aligned)
 * @return Status NVM_OK if successful
 */
Nvm_Status_t Mcal_Nvm_Erase_Page(uint16_t Address)
{
    Nvm_Status_t Status = NVM_OK;

    /* Safety Lock: Prevent erasure of Bootloader section */
    if (Address < MCAL_NVM_BOOT_SIZE_LIMIT)
    {
        Status = NVM_ERR_INVALID_ADDR;
    }
    else if ((Address & (MCAL_NVM_PAGE_SIZE - 1U)) != 0U)
    {
        Status = NVM_ERR_INVALID_ADDR;
    }
    else
    {
        /* Align address to page boundary */
        NVMCTRL.ADDR = Address;

        /* Execute erase command */
        Status = Mcal_Nvm_Execute_Cmd(NVMCTRL_CMD_PAGEERASE_gc);
    }

    return Status;
}

/**
 * @brief Writes a 64-byte page to Flash memory.
 * @param Address Flash address to write (Must be 64-byte page aligned).
 * @param Data  Pointer to the 64-byte buffer in RAM.
 * @return Status NVM_OK if successful.
 */
Nvm_Status_t Mcal_Nvm_Write_Page(uint16_t Address, const uint8_t *Data)
{
    Nvm_Status_t      Status = NVM_OK;
    size_t            Byte_Idx;
    uint16_t          Mapped_Address = Address + MAPPED_PROGMEM_START;
    volatile uint8_t *Buffer_Ptr     = (volatile uint8_t *)(Mapped_Address);

    /* Safety Lock */
    if (Address < MCAL_NVM_BOOT_SIZE_LIMIT)
    {
        Status = NVM_ERR_INVALID_ADDR;
    }
    else if ((Address & (MCAL_NVM_PAGE_SIZE - 1U)) != 0U)
    {
        Status = NVM_ERR_INVALID_ADDR;
    }

    if (Status == NVM_OK)
    {
        /* Clear buffer */
        Status = Mcal_Nvm_Execute_Cmd(NVMCTRL_CMD_PAGEBUFCLR_gc);
    }

    NVMCTRL.ADDR = Address;
    //Status       = Mcal_Nvm_Execute_Cmd(NVMCTRL_CMD_PAGEERASE_gc);

    if (Status == NVM_OK)
    {
        /* Load page buffer manually to avoid direct flash writes */

        for (Byte_Idx = 0U; Byte_Idx < ((size_t)MCAL_NVM_PAGE_SIZE); Byte_Idx++)
        {
            Buffer_Ptr[Byte_Idx] = Data[Byte_Idx];
        }
        //NVMCTRL.ADDR = Address;
        /* Execute Page Write */
        Status = Mcal_Nvm_Execute_Cmd(NVMCTRL_CMD_PAGEERASEWRITE_gc);
    }

    return Status;
}

/**
 * @brief Compares physical Flash content against expected SRAM data.
 * @param Address    Flash address to verify.
 * @param Expected_Data Pointer to the source data in RAM.
 * @return Status NVM_OK if matches, NVM_ERR_VERIFY_FAILED if mismatch
 */
Nvm_Status_t Mcal_Nvm_Verify_Page(uint16_t Address, const uint8_t *Expected_Data)
{
    Nvm_Status_t   Status    = NVM_OK;
    const uint8_t *Flash_Ptr = (const uint8_t *)(Address + MAPPED_PROGMEM_START);
    size_t         Byte_Idx;

    for (Byte_Idx = 0U; ((Byte_Idx < MCAL_NVM_PAGE_SIZE) && (Status == NVM_OK)); Byte_Idx++)
    {
        if (Flash_Ptr[Byte_Idx] != Expected_Data[Byte_Idx])
        {
            Status = NVM_ERR_VERIFY_FAILED;
        }
    }

    return Status;
}

/*----------------------------------------------------------------------------*
 * PRIVATE FUNCTION IMPLEMENTATIONS
 *----------------------------------------------------------------------------*/
/**
 * @brief  Perform a protected command on the NVM controller
 * @param Cmd
 * @return void
 */
static Nvm_Status_t Mcal_Nvm_Execute_Cmd(uint8_t Cmd)
{
    Nvm_Status_t Status  = NVM_OK;
    uint16_t     Timeout = MCAL_NVM_TIMEOUT;
    /* Wait for controller to be ready */
    while ((NVMCTRL.STATUS & (NVMCTRL_FBUSY_bm | NVMCTRL_EEBUSY_bm)) && (Timeout > 0U))
    {
        Timeout--;
    }

    if (Timeout == 0U)
    {
        Status = NVM_ERR_BUSY;
    }
    else
    {
        /* Unlock CCP and trigger command */
        //_PROTECTED_WRITE(NVMCTRL.CTRLA, Cmd);

        /* 2. Clear any lingering hardware error flags before we start */
        NVMCTRL.STATUS = NVMCTRL_WRERROR_bm;

        /* 3. The Strict Hardware Command Sequence */
        uint8_t global_interrupt_state = SREG;
        cli(); /* Disable global interrupts */

        // CCP           = 0x9D; /* Unlock Flash (NVMCTRL_KEY_SPM_gc) */
        // NVMCTRL.CTRLA = Cmd;  /* Trigger the command immediately */
        ccp_write_spm((void *)&NVMCTRL.CTRLA, Cmd);

        SREG = global_interrupt_state; /* Restore interrupts */

        /* Wait for the command to complete and verify */
        Timeout = MCAL_NVM_TIMEOUT;
        while ((NVMCTRL.STATUS & (NVMCTRL_FBUSY_bm | NVMCTRL_EEBUSY_bm)) && (Timeout > 0U))
        {
            Timeout--;
        }

        if (Timeout == 0U)
        {
            Status = NVM_ERR_BUSY;
        }
        else if (NVMCTRL.STATUS & NVMCTRL_WRERROR_bm)
        {
            Status = NVM_ERR_VERIFY_FAILED;

            /*DEBUG CODE*/
            //uint8_t StatusReg = NVMCTRL.STATUS;
            //Blink_Debug(StatusReg, 400);
            /*DEBUG CODE*/
        }
    }

    return Status;
}
