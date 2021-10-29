/**
 * @file vmwareTools.c
 * @author Emile METRAL (EBMBA)
 * @brief Prepares to install VMware Tools on the guest operating system.
 * @version 0.1
 * @date 2021-10-29
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "../include/vmwareTools.h"

VixHandle installTools(VixHandle hostHandle, char *pathToVMX){
    VixHandle jobHandle = VIX_INVALID_HANDLE;
    VixHandle vmHandle = VIX_INVALID_HANDLE;
    VixToolsState powerState = VIX_POWERSTATE_POWERED_ON;
    VixToolsState toolState = VIX_TOOLSSTATE_NOT_INSTALLED; // See : https://www.vmware.com/support/developer/vix-api/vix16_reference/types/VixPowerState.html
	VixError err = VIX_OK;

    jobHandle = VixVM_Open(hostHandle,
                            pathToVMX,
                            NULL,
                            NULL);

    err = VixJob_Wait(jobHandle,
                        VIX_PROPERTY_JOB_RESULT_HANDLE,
                        &vmHandle,
                        VIX_PROPERTY_NONE);

    if (VIX_OK != err) {
        // Handle the error...
        goto abort;
    }
    
    Vix_ReleaseHandle(jobHandle);

    // Check if Tools is installed :
    err = Vix_GetProperties(vmHandle,
                             VIX_PROPERTY_VM_TOOLS_STATE,
                             &toolState,
                             VIX_PROPERTY_NONE);


    if (VIX_OK != err) {
        // Handle the error...
        goto abort;
    }
    
    // Check if VM is on :
    err = Vix_GetProperties(vmHandle,
                             VIX_PROPERTY_VM_POWER_STATE,
                             &powerState,
                             VIX_PROPERTY_NONE);


    if (VIX_OK != err) {
        // Handle the error...
        goto abort;
    }

    // Installation VMware Tools 
    jobHandle = VixVM_InstallTools(vmHandle,
                                0,
                                NULL,
                                NULL,
                                NULL);

    err = VixJob_Wait(jobHandle,
                        VIX_PROPERTY_NONE);
    
    if (VIX_OK != err) {
        // Handle the error...
        goto abort;
    }
    
    abort:
        Vix_ReleaseHandle(jobHandle);
        Vix_ReleaseHandle(vmHandle);
    
    return jobHandle;
}