/**
 * @file powerSettings.c
 * @author Emile METRAL (EBMBA)
 * @brief Control all power settings 
 * @version 0.1
 * @date 2021-10-29
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include "../include/powerSettings.h"

/**
 * @brief Display the VM's power state
 * 
 * @param powerState VM's power state 
 */
void printPowerState(VixToolsState powerState){
    switch (powerState)
    {
    case 1:
        printf("VM is powering off\n");
        break;
    
    case 2:
        printf("VM is off\n");
        break;
    
    case 4:
        printf("VM is powering on\n");
        break;

    case 8:
        printf("VM is on\n");
        break;
    
    case 10:
        printf("VM is suspending\n");
        break;
    
    case 20:
        printf("VM is suspended\n");
        break;
    
    case 80:
        printf("VM is resetting\n");
        break;
    
    case 100:
        printf("VM is blocked and waiting for user interaction\n");
        break;
    
    case 200:
        printf("VM is paused\n");
        break;
    
    case 800:
        printf("VM is resuming\n");
        break;
    default:
        break;
    }
}

/**
 * @brief Powers on a virtual machine. 
 * 
 * @param hostHandle Host handle
 * @param pathToVMX Path to the VM 
 * @return VixHandle 
 */
VixHandle powerOn(VixHandle hostHandle, char *pathToVMX){
	VixHandle jobHandle = VIX_INVALID_HANDLE;
    VixHandle vmHandle = VIX_INVALID_HANDLE;
    VixToolsState powerState = VIX_POWERSTATE_POWERED_ON; // See : https://www.vmware.com/support/developer/vix-api/vix16_reference/types/VixPowerState.html
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

    /// CHECK IF VM IS ALREADY ON
    err = Vix_GetProperties(vmHandle,
                             VIX_PROPERTY_VM_POWER_STATE,
                             &powerState,
                             VIX_PROPERTY_NONE);

    if (VIX_OK != err || powerState != 8) {
        // Handle the error...
        if (powerState != 8)
        {
            printPowerState(powerState);
        }
        goto abort;     
    }

    /// POWER ON VM
    jobHandle = VixVM_PowerOn(vmHandle,
                                VIX_VMPOWEROP_NORMAL, // or VIX_VMPOWEROP_LAUNCH_GUI to launch workstation's GUI 
                                VIX_INVALID_HANDLE,
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

/**
 * @brief This function pauses a virtual machine.
 * 
 * @param hostHandle Host handle
 * @param pathToVMX Path to the VM 
 * @return VixHandle 
 */
VixHandle pause(VixHandle hostHandle, char *pathToVMX){
	VixHandle jobHandle = VIX_INVALID_HANDLE;
    VixHandle vmHandle = VIX_INVALID_HANDLE;
    VixToolsState powerState = VIX_POWERSTATE_POWERED_ON; // See : https://www.vmware.com/support/developer/vix-api/vix16_reference/types/VixPowerState.html
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

    /// CHECK IF VM IS ON
    err = Vix_GetProperties(vmHandle,
                             VIX_PROPERTY_VM_POWER_STATE,
                             &powerState,
                             VIX_PROPERTY_NONE);

    if (VIX_OK != err || powerState != 8) {
        // Handle the error...
        if (powerState != 8)
        {
            printPowerState(powerState);
        }
        goto abort;     
    }

    /// PAUSE VM
    jobHandle = VixVM_Pause(vmHandle,
                                0,
                                VIX_INVALID_HANDLE,
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

/**
 * @brief This function continues execution of a paused virtual machine. 
 * 
 * @param hostHandle Host handle
 * @param pathToVMX Path to the VM 
 * @return VixHandle 
 */
VixHandle unPause(VixHandle hostHandle, char *pathToVMX){
	VixHandle jobHandle = VIX_INVALID_HANDLE;
    VixHandle vmHandle = VIX_INVALID_HANDLE;
    VixToolsState powerState = VIX_POWERSTATE_POWERED_ON; // See : https://www.vmware.com/support/developer/vix-api/vix16_reference/types/VixPowerState.html
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

    /// CHECK IF VM IS PAUSED
    err = Vix_GetProperties(vmHandle,
                             VIX_PROPERTY_VM_POWER_STATE,
                             &powerState,
                             VIX_PROPERTY_NONE);

    if (VIX_OK != err || powerState != 200) {
        // Handle the error...
        if (powerState != 200)
        {
            printPowerState(powerState);
        }
        goto abort;     
    }

    /// UNPAUSE VM
    jobHandle = VixVM_Unpause(vmHandle,
                                0,
                                VIX_INVALID_HANDLE,
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

/**
 * @brief This function powers off a virtual machine.
 * 
 * @param hostHandle Host handle
 * @param pathToVMX Path to the VM 
 * @return VixHandle 
 */
VixHandle powerOff(VixHandle hostHandle, char *pathToVMX){
	VixHandle jobHandle = VIX_INVALID_HANDLE;
    VixHandle vmHandle = VIX_INVALID_HANDLE;
    VixToolsState powerState = VIX_POWERSTATE_POWERED_ON; // See : https://www.vmware.com/support/developer/vix-api/vix16_reference/types/VixPowerState.html
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

    /// CHECK IF VM IS ON 
    err = Vix_GetProperties(vmHandle,
                             VIX_PROPERTY_VM_POWER_STATE,
                             &powerState,
                             VIX_PROPERTY_NONE);

    //printf("Error %ld \nPower state : %d\n", err, powerState);
    if (VIX_OK != err || powerState != 8) {
        // Handle the error...
        if (powerState != 8)
        {
            printPowerState(powerState);
        }
        goto abort;     
    }

    jobHandle = VixVM_PowerOff(vmHandle,
                                VIX_VMPOWEROP_FROM_GUEST,
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

/**
 * @brief This function resets a virtual machine. 
 * 
 * @param hostHandle Host handle
 * @param pathToVMX Path to the VM 
 * @return VixHandle 
 */
VixHandle reset(VixHandle hostHandle, char *pathToVMX){
	VixHandle jobHandle = VIX_INVALID_HANDLE;
    VixHandle vmHandle = VIX_INVALID_HANDLE;
    VixToolsState powerState = VIX_POWERSTATE_POWERED_ON; // See : https://www.vmware.com/support/developer/vix-api/vix16_reference/types/VixPowerState.html
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

    /// Check if VM isn't off :
    err = Vix_GetProperties(vmHandle,
                             VIX_PROPERTY_VM_POWER_STATE,
                             &powerState,
                             VIX_PROPERTY_NONE);


    if (VIX_OK != err || powerState == 2 ) {
        // Handle the error...
        if (powerState == 2)
        {
            printPowerState(powerState);
        }
        goto abort;     
    }
    
    // Reset part : 
    jobHandle = VixVM_Reset(vmHandle,
                                VIX_VMPOWEROP_NORMAL,
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

/**
 * @brief  This function suspends a virtual machine. 
 * 
 * @param hostHandle Host handle
 * @param pathToVMX Path to the VM 
 * @return VixHandle 
 */
VixHandle suspend(VixHandle hostHandle, char *pathToVMX){
	VixHandle jobHandle = VIX_INVALID_HANDLE;
    VixHandle vmHandle = VIX_INVALID_HANDLE;
    VixToolsState powerState = VIX_POWERSTATE_POWERED_ON; // See : https://www.vmware.com/support/developer/vix-api/vix16_reference/types/VixPowerState.html
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

    /// Check if VM is on :
    err = Vix_GetProperties(vmHandle,
                             VIX_PROPERTY_VM_POWER_STATE,
                             &powerState,
                             VIX_PROPERTY_NONE);


    if (VIX_OK != err || powerState != 8) {
        // Handle the error...
        if (powerState != 8)
        {
            printPowerState(powerState);
        }
        goto abort;     
    }

    /// SUSPEND PART
    jobHandle = VixVM_Suspend(vmHandle,
                                0,
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
