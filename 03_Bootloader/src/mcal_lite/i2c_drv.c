/******************************************************************************
 * @file      i2c_drv.c
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
#include "i2c_drv.h"

/*----------------------------------------------------------------------------*
 * PRIVATE INCLUDES
 *----------------------------------------------------------------------------*/
#include <avr/io.h>
#include <util/delay.h>
/*----------------------------------------------------------------------------*
 * PRIVATE CONSTANTS & MACROS
 *----------------------------------------------------------------------------*/
#define MCAL_I2C_TIMEOUT_LIMIT 0xFFFFU

#define MCAL_I2C_READ_BIT      0x01U
#define MCAL_I2C_WRITE_BIT     0x00U
/*----------------------------------------------------------------------------*
 * PRIVATE TYPEDEFS & STRUCTURES
 *----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*
 * PRIVATE VARIABLES (STATIC DATA)
 *----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*
 * PRIVATE FUNCTION PROTOTYPES (STATIC)
 *----------------------------------------------------------------------------*/
static I2c_Status_t I2c_Wait_For_Hardware_Flag(uint8_t Flag_Mask);

/*----------------------------------------------------------------------------*
 * PUBLIC FUNCTION IMPLEMENTATIONS
 *----------------------------------------------------------------------------*/

/**
 * @brief  	Initializes the TWI hardware with a pre-calculated baud value.
 * @param  	void
 * @return	Status
 */
I2c_Status_t Mcal_I2c_Init(void)
{
    I2c_Status_t Status = I2C_ERR_BUSY;
    /* Set Pin Multiplexer to Alternative Pins (PA1/PA2) */
    PORTMUX.CTRLB |= PORTMUX_TWI0_bm;

    /* Set Baud Rate for ~385kHz at 10MHz F_CPU */
    TWI0.MBAUD = I2C_BAUDRATE_385KHZ;

    /* Enable TWI Master. Smart Mode (SMEN) is explicitly disabled
   * to allow precise manual control over ACK/NACK and Repeated Starts. */
    TWI0.MCTRLA = TWI_ENABLE_bm;

    /* Force Bus State to IDLE */
    TWI0.MSTATUS = TWI_BUSSTATE_IDLE_gc;

    /* Wait for the bus to clear */
    _delay_ms(10U);

    /* Integrity check. Ensure bus is actually free */
    if (TWI0.MSTATUS == TWI_BUSSTATE_IDLE_gc)
    {
        Status = I2C_OK;
    }

    return Status;
}

/**
 * @brief  	Writes a sequence of bytes to an I2C slave device.
 * @param  	*Data_Structure	- Pointer to data
 * @return	Status			- Indicating success, timeout, or NACK
 */
I2c_Status_t Mcal_I2c_Write(const I2c_Data_Transfer_t *Data_Structure)
{
    I2c_Status_t Status = I2c_Wait_For_Hardware_Flag(TWI_BUSSTATE_IDLE_gc);
    uint16_t     Byte_Idx;

    if ((TWI0.MSTATUS & TWI_BUSSTATE_gm) != TWI_BUSSTATE_IDLE_gc)
    {
        Status = I2C_ERR_BUSY;
    }

    if (Status == I2C_OK)
    {
        /* Write slave address + Write Bit (0) */
        TWI0.MADDR = (uint8_t)((Data_Structure->Slave_Addr << 1U) | MCAL_I2C_WRITE_BIT);

        for (Byte_Idx = 0U; ((Byte_Idx < Data_Structure->Len) && (Status == I2C_OK)); Byte_Idx++)
        {
            Status = I2c_Wait_For_Hardware_Flag(TWI_WIF_bm);
            if (Status == I2C_OK)
            {
                TWI0.MDATA = Data_Structure->Data[Byte_Idx];
            }
        }
    }

    /* Wait for final Byte_Idx to be transmitted */
    if (Status == I2C_OK)
    {
        Status = I2c_Wait_For_Hardware_Flag(TWI_WIF_bm);
    }

    /* Issue STOP if repeated START is not requested */
    if ((Status == I2C_OK) && (Data_Structure->Action != I2C_RESTART))
    {
        TWI0.MCTRLB = TWI_MCMD_STOP_gc;
    }

    return Status;
}

/**
 * @brief  	Reads a sequence of bytes from an I2C slave device.
 * @param  	Data_Structure	- Pointer to data
 * @return	Status			- Indicating success, timeout, or NACK
 */
I2c_Status_t Mcal_I2c_Read(const I2c_Data_Transfer_t *Data_Structure)
{
    I2c_Status_t Status = I2C_OK;
    uint16_t     Byte_Idx;

    if ((TWI0.MSTATUS & TWI_BUSSTATE_gm) != TWI_BUSSTATE_IDLE_gc)
    {
        Status = I2C_ERR_BUSY;
    }

    if (Status == I2C_OK)
    {
        /* Write slave address + Read Bit (1) - Triggers Repeated Start */
        TWI0.MADDR = (uint8_t)((Data_Structure->Slave_Addr << 1U) | MCAL_I2C_READ_BIT);

        for (Byte_Idx = 0U; (Byte_Idx < Data_Structure->Len) && (Status == I2C_OK); Byte_Idx++)
        {
            Status = I2c_Wait_For_Hardware_Flag(TWI_RIF_bm);

            if (Status == I2C_OK)
            {
                Data_Structure->Data[Byte_Idx] = TWI0.MDATA;

                /* Manual ACK/NACK control for the final Byte_Idx */
                if ((Byte_Idx == (Data_Structure->Len - 1U))
                    && (Data_Structure->Action == I2C_STOP))
                {
                    TWI0.MCTRLB = TWI_ACKACT_NACK_gc | TWI_MCMD_STOP_gc;
                }
                else
                {
                    TWI0.MCTRLB = TWI_ACKACT_ACK_gc | TWI_MCMD_RECVTRANS_gc;
                }
            }
        }
    }
    return Status;
}

/*----------------------------------------------------------------------------*
 * PRIVATE FUNCTION IMPLEMENTATIONS
 *----------------------------------------------------------------------------*/
/**
 * @brief  	Polls hardware status flags for completion or error
 * @param  	Flag_Mask	- The bitmask (WIF, RIF, or BUSSTATE) to wait for in MSTATUS.
 * @return	Status
 */
static I2c_Status_t I2c_Wait_For_Hardware_Flag(uint8_t Flag_Mask)
{
    I2c_Status_t Status  = I2C_OK;
    uint16_t     Timeout = MCAL_I2C_TIMEOUT_LIMIT;

    /* Poll until flag is set or timeout expired or error bits are detected */
    while (((TWI0.MSTATUS & Flag_Mask) == 0U)
           && ((TWI0.MSTATUS & (TWI_RXACK_bm | TWI_BUSERR_bm | TWI_ARBLOST_bm)) == 0U)
           && (Timeout > 0U))
    {
        Timeout--;
    }

    /* Analyze the exit state */
    if (Timeout == 0U)
    {
        Status = I2C_ERR_TIMEOUT;
    }
    else if (TWI0.MSTATUS & (TWI_RXACK_bm | TWI_BUSERR_bm | TWI_ARBLOST_bm))
    {
        Status = I2C_ERR_NACK;
    }
    else
    {
        Status = I2C_OK;
    }

    return Status;
}
