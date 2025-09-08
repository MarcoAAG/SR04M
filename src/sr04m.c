/**********************************************************************************************************************
 * \file sr04m.c
 *
 * \author MarcoAAG
 *
 * \date May-03-2025
 *
 * \version 1.0 \n \n
 *
 * \brief Implementation of the SR04M driver
 *
 *********************************************************************************************************************/

/* ============================================================================================== */
/*                                         Include Files                                          */
/* ============================================================================================== */
#include <sr04m.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ============================================================================================== */
/*                                        Defines & Macros                                        */
/* ============================================================================================== */
#define TX_LEN       1
#define MODE5_RX_LEN 12
#define MODE4_RX_LEN 4
#define MAX_DIGITS   5
#define MIN_DIGITS   4
#define PREFIX       "Gap="
#define SUFFIX       "mm\r\n"
#define IS_DIGIT(c)  ((c) >= '0' && (c) <= '9')

/* ============================================================================================== */
/*                                    Private Function Prototypes                                 */
/* ============================================================================================== */

/** **********************************************************************************************
 * \defgroup SR04M_Private_IO Internal Register Access Wrappers
 * \brief Internal static functions used to abstract register-level read/write operations.
 * @{
 */

/**
 * \brief Wrapper function to perform a register read using SR04M_Object.
 *
 * Forwards the read request to the function pointer defined in the `SR04M_IO` interface.
 *
 * \param[in]  p_handle Pointer to the SR04M_Object instance (cast to void*).
 * \param[out] p_data   Pointer to the buffer where data will be stored.
 * \param[in]  u_length Number of bytes to read.
 *
 * \return Status code returned by the underlying `readReg` function.
 */
static uint32_t SR04M_u_ReadRegWrap(void* p_handle, uint8_t* p_data, uint8_t u_length);

/**
 * \brief Wrapper function to perform a register write using SR04M_Object.
 *
 * Forwards the write request to the function pointer defined in the `SR04M_IO` interface.
 *
 * \param[in] p_handle Pointer to the SR04M_Object instance (cast to void*).
 * \param[in] p_data   Pointer to the buffer containing data to write.
 * \param[in] u_length Number of bytes to write.
 *
 * \return Status code returned by the underlying `writeReg` function.
 */
static uint32_t SR04M_u_WriteRegWrap(void* p_handle, uint8_t* p_data, uint8_t u_length);

/**
 * \brief Performs a register read through the driver context.
 *
 * Uses the read function and handle stored in the `SR04M_CTX` to perform the operation.
 *
 * \param[in]  p_ctx    Pointer to the SR04M_CTX structure.
 * \param[out] p_data   Pointer to the buffer where data will be stored.
 * \param[in]  u_length Number of bytes to read.
 *
 * \return Status code returned by the context's `readReg` function.
 */
static uint32_t SR04M_u_ReadReg(SR04M_CTX* p_ctx, uint8_t* p_data, uint8_t u_length);

/**
 * \brief Performs a register write through the driver context.
 *
 * Uses the write function and handle stored in the `SR04M_CTX` to perform the operation.
 *
 * \param[in] p_ctx    Pointer to the SR04M_CTX structure.
 * \param[in] p_data   Pointer to the buffer containing data to write.
 * \param[in] u_length Number of bytes to write.
 *
 * \return Status code returned by the context's `writeReg` function.
 */
static uint32_t SR04M_u_WriteReg(SR04M_CTX* p_ctx, uint8_t* p_data, uint8_t u_length);

/** @} */ // end of SR04M_Private_IO

SR04M_Status SR04M_t_PrintingMode(SR04M_Object* p_obj, uint16_t* u_distance);

SR04M_Status SR04M_t_StrValidator(const char* pu_str, uint8_t* pu_arr, uint8_t u_startIndex, uint8_t u_endIndex);

static SR04M_Status SR04M_t_SerialModeLP(SR04M_Object* p_obj, uint16_t* u_distance);

/* ============================================================================================== */
/*                                         Public Functions                                       */
/* ============================================================================================== */

SR04M_Status SR04M_u_Init(SR04M_Object* p_obj, SR04M_IO* p_io)
{
  SR04M_Status u_ret = SR04M_OK;

  if(p_obj == NULL)
  {
    u_ret = SR04M_ERROR;
  }
  else
  {
    p_obj->io.writeReg = p_io->writeReg;
    p_obj->io.readReg  = p_io->readReg;

    p_obj->ctx.readReg  = SR04M_u_ReadRegWrap;
    p_obj->ctx.writeReg = SR04M_u_WriteRegWrap;
    p_obj->ctx.handle   = p_obj;
  }

  return u_ret;
}

SR04M_Status SR04M_u_GetDistance(SR04M_Object* p_obj, uint16_t* u_distance, SR04M_Modes te_mode)
{
  SR04M_Status u_ret = SR04M_OK;

  if((te_mode > MODE5) || (te_mode < MODE1))
  {
    u_ret = SR04M_ERROR;
  }
  else
  {
    switch(te_mode)
    {
      case MODE1:
        /* code */
        break;
      case MODE2:
        /* code */
        break;
      case MODE3:
        /* code */
        break;
      case MODE4:
        u_ret = SR04M_t_SerialModeLP(p_obj, u_distance);
        break;
      case MODE5:
        u_ret = SR04M_t_PrintingMode(p_obj, u_distance);
        /* code */
        break;
      default:
        break;
    }
  }

  return u_ret;
}

/* ============================================================================================== */
/*                                         Private Functions                                      */
/* ============================================================================================== */

static uint32_t SR04M_u_ReadRegWrap(void* p_handle, uint8_t* p_data, uint8_t u_length)
{
  SR04M_Object* p_obj = (SR04M_Object*)p_handle;

  return p_obj->io.readReg(p_data, u_length);
}

static uint32_t SR04M_u_WriteRegWrap(void* p_handle, uint8_t* p_data, uint8_t u_length)
{
  SR04M_Object* p_obj = (SR04M_Object*)p_handle;

  return p_obj->io.writeReg(p_data, u_length);
}

static uint32_t SR04M_u_ReadReg(SR04M_CTX* p_ctx, uint8_t* p_data, uint8_t u_length)
{
  return p_ctx->readReg(p_ctx->handle, p_data, u_length);
}

static uint32_t SR04M_u_WriteReg(SR04M_CTX* p_ctx, uint8_t* p_data, uint8_t u_length)
{
  return p_ctx->writeReg(p_ctx->handle, p_data, u_length);
}

static SR04M_Status SR04M_t_SerialModeLP(SR04M_Object* p_obj, uint16_t* u_distance)
{
  SR04M_Status e_retVal                 = SR04M_OK;
  uint8_t      a_txBuffer[TX_LEN]       = { 1u };
  uint8_t      a_rxBuffer[MODE4_RX_LEN] = { 0u };
  uint8_t      u_checksum;

  SR04M_u_WriteReg(&p_obj->ctx, a_txBuffer, TX_LEN);
  SR04M_u_ReadReg(&p_obj->ctx, a_rxBuffer, MODE4_RX_LEN);

  if(a_rxBuffer[0] == 0xFF)
  {
    u_checksum = (a_rxBuffer[0] + a_rxBuffer[1] + a_rxBuffer[2]) & a_rxBuffer[0];
    if(u_checksum == a_rxBuffer[3])
    {
      *u_distance = (a_rxBuffer[1] * 256) + a_rxBuffer[2];
    }
    else
    {
      e_retVal = SR04M_ERROR;
    }
  }
  else
  {
    e_retVal = SR04M_ERROR;
  }

  return e_retVal;
}

/*
0x47 -> "G"
0x61 -> "a"
0x70 -> "p"
0x37 -> "="
0xXX -> number
0xXX -> number
0xXX -> number
0xXX -> number
0xXX -> number (optional)
0x6D -> "m"
0x6D -> "m"
0x0D -> CR "\r"
0x0A -> LF "\n"
*/
SR04M_Status SR04M_t_PrintingMode(SR04M_Object* p_obj, uint16_t* u_distance)
{
  uint8_t      a_txBuffer[TX_LEN]       = { 1u };
  uint8_t      a_rxBuffer[MODE5_RX_LEN] = { 0u };
  SR04M_Status e_retVal                 = SR04M_OK;
  uint8_t      u_digitStart             = 4u;
  uint16_t     u_value                  = 0u;

  SR04M_u_WriteReg(&p_obj->ctx, a_txBuffer, TX_LEN);
  SR04M_u_ReadReg(&p_obj->ctx, a_rxBuffer, MODE5_RX_LEN);

  // Validate prefix "Gap="
  if(SR04M_t_StrValidator(PREFIX, a_rxBuffer, 0, 3) == SR04M_OK)
  {
    uint8_t u_index      = u_digitStart;
    uint8_t u_digitCount = 0;

    while(u_index < MODE5_RX_LEN && IS_DIGIT(a_rxBuffer[u_index]) && u_digitCount < MAX_DIGITS)
    {
      u_value = (u_value * 10) + (a_rxBuffer[u_index] - '0');
      u_index++;
      u_digitCount++;
    }

    if(u_digitCount < MIN_DIGITS || u_digitCount > MAX_DIGITS)
    {
      e_retVal = SR04M_ERROR;
    }
    else
    {
      if(SR04M_t_StrValidator(SUFFIX, a_rxBuffer, u_index, u_index + 3) == SR04M_OK)
      {
        *u_distance = u_value;
      }
      else
      {
        e_retVal = SR04M_ERROR;
      }
    }
  }
  else
  {
    e_retVal = SR04M_ERROR;
  }

  return e_retVal;
}

SR04M_Status SR04M_t_StrValidator(const char* pu_str, uint8_t* pu_arr, uint8_t u_startIndex, uint8_t u_endIndex)
{
  SR04M_Status e_retVal = SR04M_OK;
  uint8_t      u_index  = u_startIndex;

  for(u_index = u_startIndex; u_index <= u_endIndex; u_index++)
  {
    if(pu_arr[u_index] != pu_str[u_index - u_startIndex])
    {
      e_retVal = SR04M_ERROR;
      break;
    }
  }

  return e_retVal;
}

#ifdef __cplusplus
}
#endif
