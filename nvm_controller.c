/*
 * File:   nvm_controller.c
 * Author: KSK0419
 *
 * Created on March 24, 2016, 11:33 AM
 */


#include "xc.h"
#include "nvm_controller.h"

#if NVM_ENABLED == 1

// Declare a compiler memory space for eeprom
int __attribute__((space(eedata))) eeData;

static void initNvm(void)
{

}

static void WrEE(_prog_addressT address, 
        uint16_t data)
{
    _erase_eedata( address, _EE_WORD );
    _wait_eedata();
    _write_eedata_word( address, data );
    _wait_eedata();
}

static void RdEE(_prog_addressT address, 
        uint16_t *pData)
{
    uint16_t *pD = pData;
    _memcpy_p2d16( pD, address, 2);
}

nvm_error_t NvmInit(void)
{
    return NVM_SUCCESS;
}

nvm_error_t NvmRegisterBlock(nvm_module_param_t *pParam, 
        nvm_handle_t *pHandle)
{
    return NVM_SUCCESS;
}

nvm_error_t NvmBlockIdGet(nvm_handle_t *pHandle, 
        nvm_size_t blockNum, 
        nvm_handle_t *pReqHandle)
{
    return NVM_SUCCESS;
}

nvm_error_t NvmBlockStore(nvm_handle_t *pDest,
        nvm_size_t *pData,
        nvm_size_t dataLen,
        nvm_size_t dataOffset)
{
    return NVM_SUCCESS;
}

nvm_error_t NvmBlockGet(nvm_handle_t *pSrc,
        nvm_size_t *pDest,
        nvm_size_t dataLen,
        nvm_size_t offset)
{
    return NVM_SUCCESS;
}

#endif

