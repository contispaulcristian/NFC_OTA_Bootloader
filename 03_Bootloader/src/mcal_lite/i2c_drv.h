/******************************************************************************
 * @file      i2c_drv.h
 * @author    Paul Contis
 * @date      2026-03-09
 * @brief     Public API for the module
 * @details   Describe the architectural role and usage of this module.
 * @note      Target: ATtiny1616 | Clock: 10MHz
 * @copyright (c) 2026 DIY for Geeks. All rights reserved.
 ******************************************************************************/

#ifndef I2C_DRV_H
#define I2C_DRV_H

/* --- Standard Includes --- */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/*----------------------------------------------------------------------------*
 * PUBLIC CONSTANTS & MACROS
 *----------------------------------------------------------------------------*/
/* Baud Rate Calculation for 10MHz F_CPU to get ~385kHz */
/* F_CPU / (2 * 385kHz) - 5 = ~8 */
#define I2C_BAUDRATE_385KHZ 8U

/*----------------------------------------------------------------------------*
 * PUBLIC TYPEDEFS & STRUCTURES
 *----------------------------------------------------------------------------*/
/* --- I2C Bus Status Codes --- */
typedef enum
{
    I2C_OK = 0U,
    I2C_ERR_NACK,
    I2C_ERR_BUSY,
    I2C_ERR_TIMEOUT
} I2c_Status_t;

typedef enum
{
    I2C_STOP = 0U, /* End the transaction (Release the bus) */
    I2C_RESTART    /* Keep the bus (For Repeated Start) */
} I2c_Bus_Action_t;

typedef struct
{
    uint8_t          Slave_Addr;
    uint8_t         *Data;
    uint16_t         Len;
    I2c_Bus_Action_t Action;
} I2c_Data_Transfer_t;

/*----------------------------------------------------------------------------*
 * PUBLIC FUNCTION PROTOTYPES
 *----------------------------------------------------------------------------*/
/* --- Initialization --- */
I2c_Status_t Mcal_I2c_Init(void);

/* --- API (For ST25DV NFC Tag) --- */
I2c_Status_t Mcal_I2c_Write(const I2c_Data_Transfer_t *Data_Structure);
I2c_Status_t Mcal_I2c_Read(const I2c_Data_Transfer_t *Data_Structure);

#endif /* I2C_DRV_H */
