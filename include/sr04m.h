/**********************************************************************************************************************
 * \file sr04m.h
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

#ifndef SR04M_H
#define SR04M_H

/* ============================================================================================== */
/*                                         Include Files                                          */
/* ============================================================================================== */
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ============================================================================================== */
/*                                        Defines & Macros                                        */
/* ============================================================================================== */

/* ============================================================================================== */
/*                                         Public Types                                           */
/* ============================================================================================== */
typedef uint8_t (*SR04M_Init_Func)(void);
typedef uint32_t (*SR04M_WriteRegFunc)(uint8_t*, uint8_t);
typedef uint32_t (*SR04M_ReadRegFunc)(uint8_t*, uint8_t);
typedef uint32_t (*SR04M_WriteFunc)(void*, uint8_t*, uint8_t);
typedef uint32_t (*SR04M_ReadFunc)(void*, uint8_t*, uint8_t);

typedef enum
{
  MODE1 = 1,
  MODE2,
  MODE3,
  MODE4,
  MODE5
} SR04M_Modes;

typedef enum
{
  SR04M_OK           = 0,
  SR04M_ERROR        = -1,
  SR04M_ERR_CHECKSUM = -2,
  SR04M_ERR_HEADER   = -3,
  SR04M_ERR_DEINIT   = -4
} SR04M_Status;

/** **********************************************************************************************
 * \defgroup SR04M_Context SR04M Driver Context
 * \brief Data structure used to configure and access the SR04M ultrasonic sensor.
 * @{
 */

/**
 * \brief SR04M driver context structure.
 *
 * Holds function pointers for low-level register access and a device handle.
 * This context must be initialized before using the SR04M driver functions.
 */
typedef struct
{
  /**
   * \brief Function pointer to write to a device register.
   *
   * This function is called to perform register writes.
   * It should match the SR04M_WriteFunc signature defined by the driver.
   *
   * \note The implementation is platform-dependent.
   */
  SR04M_WriteFunc writeReg;

  /**
   * \brief Function pointer to read from a device register.
   *
   * This function is called to perform register reads.
   * It should match the SR04M_ReadFunc signature defined by the driver.
   *
   * \note The implementation is platform-dependent.
   */
  SR04M_ReadFunc readReg;

  /**
   * \brief Device-specific handle passed to the low-level functions.
   *
   * Typically used to pass peripheral context (e.g., I2C/SPI handle).
   * This pointer is forwarded to the `writeReg` and `readReg` functions.
   */
  void* handle;

} SR04M_CTX;

/** @} */ // end of SR04M_Context

/** **********************************************************************************************
 * \defgroup SR04M_IO_Interface SR04M Low-Level IO Interface
 * \brief Structure for low-level I/O function pointers for the SR04M ultrasonic sensor.
 * @{
 */

/**
 * \brief SR04M low-level I/O function interface.
 *
 * Contains function pointers used to perform register-level read and write operations.
 * This interface abstracts the hardware-dependent access mechanisms such as I2C or SPI.
 */
typedef struct
{
  SR04M_Init_Func init;

  /**
   * \brief Function pointer to write to a device register.
   *
   * Called by the driver to perform register writes.
   * Must conform to the SR04M_WriteRegFunc signature.
   *
   * \note The implementation is platform-specific.
   */
  SR04M_WriteRegFunc writeReg;

  /**
   * \brief Function pointer to read from a device register.
   *
   * Called by the driver to perform register reads.
   * Must conform to the SR04M_ReadRegFunc signature.
   *
   * \note The implementation is platform-specific.
   */
  SR04M_ReadRegFunc readReg;

} SR04M_IO;

/** @} */ // end of SR04M_IO_Interface

/** **********************************************************************************************
 * \defgroup SR04M_Object_Instance SR04M Driver Instance Object
 * \brief Structure representing a complete instance of the SR04M driver.
 * @{
 */

/**
 * \brief SR04M driver instance object.
 *
 * Represents a complete driver instance including I/O interface, context, and internal state.
 * This structure is typically used by application code to manage the SR04M driver lifecycle.
 */
typedef struct
{
  /**
   * \brief Low-level I/O interface for register access.
   *
   * Contains platform-specific read and write function pointers required to communicate
   * with the SR04M sensor at the register level.
   *
   * \see SR04M_IO
   */
  SR04M_IO io;

  /**
   * \brief Driver context structure.
   *
   * Includes function pointers and device handle used internally by the driver to
   * manage operations and maintain state.
   *
   * \see SR04M_CTX
   */
  SR04M_CTX ctx;

  /**
   * \brief Flag indicating whether the driver has been initialized.
   *
   * Value is non-zero if the driver has completed initialization; zero otherwise.
   */
  uint8_t isInitialized;

  /**
   * \brief Mode to obtain raw value
   *
   */
  SR04M_Modes mode;

} SR04M_Object;

/** @} */ // end of SR04M_Object_Instance

typedef struct
{
  SR04M_Status (*Init)(SR04M_Object*);
  SR04M_Status (*GetDistance)(SR04M_Object*, uint16_t*);
} SR04M_Drv;

extern SR04M_Drv SR04M_Driver;

/* ============================================================================================== */
/*                                         Public Functions                                       */
/* ============================================================================================== */

SR04M_Status SR04M_e_RegisterBusIO(SR04M_Object* p_obj, SR04M_IO* p_io);

/** **********************************************************************************************
 * \brief Initializes the SR04M driver instance.
 *
 * This function configures the driver with the provided I/O interface and prepares it
 * for operation. It sets up internal state and must be called before any other driver function.
 *
 * \param[in,out] p_obj Pointer to the SR04M_Object instance to be initialized.
 * \param[in]     p_io  Pointer to the SR04M_IO structure containing low-level I/O functions.
 *
 * \retval SR04M_OK    Initialization successful.
 * \retval SR04M_ERROR Null pointer or invalid function pointers provided.
 */
SR04M_Status SR04M_u_Init(SR04M_Object* p_obj);

/** **********************************************************************************************
 * \brief Measures and returns the distance detected by the SR04M sensor.
 *
 * This function triggers the measurement process and returns the measured distance in centimeters.
 *
 * \param[in] p_obj Pointer to the initialized SR04M_Object instance.
 *
 * \return Distance in centimeters (0–400). Returns 0 if measurement fails or sensor is not initialized.
 */
SR04M_Status SR04M_u_GetDistance(SR04M_Object* p_obj, uint16_t* u_distance);

#ifdef __cplusplus
}
#endif

#endif // SR04M_H