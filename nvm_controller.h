/* Microchip Technology Inc. and its subsidiaries.  You may use this software 
 * and any derivatives exclusively with Microchip products. 
 * 
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER 
 * EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED 
 * WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A 
 * PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION 
 * WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION. 
 *
 * IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
 * INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
 * WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS 
 * BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE 
 * FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS 
 * IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF 
 * ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE 
 * TERMS. 
 */

/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef NVM_CONTROLLER_H__
#define	NVM_CONTROLLER_H__

#include <xc.h> // include processor files - each processor file is guarded.  
#include <stdint.h>
#include <libpic30.h>

#define NVM_ENABLED 0

#if NVM_ENABLED == 1

typedef uint16_t nvm_size_t;
typedef uint16_t nvm_handle_t;
typedef enum 
{
    NVM_SUCCESS = 0,
    NVM_FAILURE = 1
} nvm_error_t;

typedef struct 
{
    nvm_size_t block_size; /// Identifies how big each block should be in this module
    nvm_size_t block_count; /// Identifies how many blocks should exist in this module
} nvm_module_param_t;

/**
 * Function for initializing the Nvm Storage
 * 
 * @return nvm_error_t -> Represents any errors that occured in init
 * 
 * @note this function should be called once - it resets all pointers
 *       if called again (previously saved data will be lost)
 * @note this function is safe to call on reset, as long as everything
 *       is loaded sequentially, it should load in the same order 
 */
nvm_error_t NvmInit(void);

/**
 * Function for initializing a block memory in Nvm Storage
 * 
 * @param pParam -> Parameters by which to initialize block
 * @param pHandle -> Loaded with the handle of the module, pass blank
 * @return -> Represents any error condition
 */
nvm_error_t NvmRegisterBlock(nvm_module_param_t *pParam, 
        nvm_handle_t *pHandle);

/**
 * Function for getting the Id of a specific block in the Nvm Storage Module
 * 
 * @param pHandle -> Handle given in Register block
 * @param blockNum -> Which block the caller is looking for (ref in init params)
 * @param pReqHandle -> Returns correct handle by which data can be reached
 * @return -> Error code
 */
nvm_error_t NvmBlockIdGet(nvm_handle_t *pHandle, 
        nvm_size_t blockNum, 
        nvm_handle_t *pReqHandle);

/**
 * Function for storing data into a specific block in the Nvm Storage Module
 * 
 * @param pDest -> Handle for destination to store
 * @param pData -> Data to store in the given destination
 * @param dataLen -> Length of data to store
 * @param dataOffset -> Offset of data (soas to use non-standard block sizes)
 * @return -> Error code
 */
nvm_error_t NvmBlockStore(nvm_handle_t *pDest,
        nvm_size_t *pData,
        nvm_size_t dataLen,
        nvm_size_t dataOffset);

/**
 * Function for retrieving data from a specific block in Nvm storage module
 * 
 * @param pSrc -> Handle for location to get data from
 * @param pDest -> Pointer to where data should be stored
 * @param dataLen -> Amount of data to fetch
 * @param offset -> Offset of data
 * @return -> Error code
 */
nvm_error_t NvmBlockGet(nvm_handle_t *pSrc,
        nvm_size_t *pDest,
        nvm_size_t dataLen,
        nvm_size_t offset);

#endif

#endif	/* NVM_CONTROLLER_H__ */

