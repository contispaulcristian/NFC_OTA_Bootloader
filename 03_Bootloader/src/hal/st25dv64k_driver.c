/******************************************************************************
 * @file      st25dv64k_driver.c
 * @author    Paul Contis
 * @date      2026-03-11
 * @brief     Implementation of the module logic
 * @details   Detailed logic and hardware interaction notes.
 * @note      Target: ATtiny1616
 * @copyright (c) 2026 DIY for Geeks. All rights reserved.
 ******************************************************************************/

/*----------------------------------------------------------------------------*
 * MODULE HEADER
 *----------------------------------------------------------------------------*/
#include "st25dv64k_driver.h"

/*----------------------------------------------------------------------------*
 * PRIVATE INCLUDES
 *----------------------------------------------------------------------------*/
#include "i2c_drv.h"
#include "config.h"

#include "port_drv.h" /*debug purpose*/
#include <util/delay.h>

#include "../debug.h"

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
 * @brief Configures the ST25DV registers to enable Mailbox and GPO interrupts.
 * Must be called once at bootloader startup.
 */
St25_Status_t St25_Init(void)
{
    St25_Status_t Status = ST25_OK;
    /* Reset Mailbox */

    /* Configure GPO */
    /* Write ST25_GPO_CTRL_REG: Enable GPO and Mailbox Message interrupt */

    /* nable Mailbox (Last step) */
    /* Write ST25_MAILBOX_CTRL_REG: Enable Mailbox */

    /* Verification of Configuration */
    /* Read MAILBOX_CTRL and GPO_CTRL to ensure they hold the expected values */
    return Status;
}

/**
 * @brief Reads a chunk of data from the ST25DV Mailbox.
 * @param Buffer Pointer to destination array.
 * @param Length Number of bytes to read.
 * @return Status
 */
St25_Status_t St25Dv_Read_Mailbox(uint8_t *Buffer, uint16_t Length)
{
    St25_Status_t Status = ST25_OK;
    /* We need to send the 16-bit register address: 0x2008 */
    uint8_t reg_addr[2];
    reg_addr[0] = (uint8_t)(HAL_ST25_HOST_PUT_MSG_REG >> 8);   // 0x20
    reg_addr[1] = (uint8_t)(HAL_ST25_HOST_PUT_MSG_REG & 0xFF); // 0x08

    /*Dummy Write the Register Address using REPEATED START */
    I2c_Data_Transfer_t write_transfer = {
        .Slave_Addr = 0x53U,
        .Data       = reg_addr,
        .Len        = 2,
        .Action     = I2C_RESTART /* CRITICAL: Do not release the bus! */
    };

    if (Mcal_I2c_Write(&write_transfer) != I2C_OK)
    {
        //Blink_Debug(2, 400);
        Status = ST25_ERR_COMM;
    }

    /* Read the payload using STOP */
    if (Status == 0U)
    {
        I2c_Data_Transfer_t read_transfer = {
            .Slave_Addr = 0x53U,
            .Data       = Buffer,
            .Len        = Length,
            .Action     = I2C_STOP /* Finish transaction and release bus */
        };

        if (Mcal_I2c_Read(&read_transfer) != I2C_OK)
        {
            Status = ST25_ERR_COMM;
            //Blink_Debug(2, 400);
        }
    }
    return Status;
}

/**
 * @brief Writes an acknowledgment byte back to the Mailbox HOST_PUT_MSG register.
 * @param Data The status byte (e.g., ACK or NACK).
 * @return Status
 */
St25_Status_t ST25DV_Write_Ack(uint8_t Data)
{
    St25_Status_t Status = ST25_OK;

    return Status;
}

/*----------------------------------------------------------------------------*
 * PRIVATE FUNCTION IMPLEMENTATIONS
 *----------------------------------------------------------------------------*/
// /**
//  * @brief Register Accessor for System Config (Fixed Address)
//  */
// static St25_Status_t St25DV_Write_System_Reg(uint16_t Reg_Addr, uint8_t Data)
// {
// }

// /**
//  * @brief Register Accessor for Dynamic Data (Mailbox/Status)
//  */
// static St25_Status_t St25DV_Write_Dynamic_Reg(uint16_t Reg_Addr, const uint8_t *Data, uint16_t Len)
// {
// }

// /**
//  * @brief Register Accessor for Dynamic Data (Mailbox/Status)
//  */
// /* 2. Low-level read from a system register */
// static St25_Status_t St25DV_Read_Reg(uint16_t Reg_Addr, uint8_t *Data);

// /**
//  * @brief Mailbox Status check
// */
// static bool St25DV_Is_Mailbox_Ready(void)
// {
// }

// St25_I2c_Transport(uint8_t Target_Addr, uint16_t Reg, uint8_t *Data, uint16_t Len)
// {
// }
