#include "../include/powerSettings.h"

VixHandle powerOn(VixHandle hostHandle, char *pathToVMX){
	VixHandle jobHandle = VIX_INVALID_HANDLE;
    VixHandle vmHandle = VIX_INVALID_HANDLE;
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

VixHandle pause(VixHandle hostHandle, char *pathToVMX){
	VixHandle jobHandle = VIX_INVALID_HANDLE;
    VixHandle vmHandle = VIX_INVALID_HANDLE;
	VixError err = VIX_OK;
}

VixHandle unPause(VixHandle hostHandle, char *pathToVMX){
	VixHandle jobHandle = VIX_INVALID_HANDLE;
    VixHandle vmHandle = VIX_INVALID_HANDLE;
	VixError err = VIX_OK;
}

VixHandle powerOff(VixHandle hostHandle, char *pathToVMX){
	VixHandle jobHandle = VIX_INVALID_HANDLE;
    VixHandle vmHandle = VIX_INVALID_HANDLE;
	VixError err = VIX_OK;
}

VixHandle reset(VixHandle hostHandle, char *pathToVMX){
	VixHandle jobHandle = VIX_INVALID_HANDLE;
    VixHandle vmHandle = VIX_INVALID_HANDLE;
	VixError err = VIX_OK;
}

VixHandle suspend(VixHandle hostHandle, char *pathToVMX){
	VixHandle jobHandle = VIX_INVALID_HANDLE;
	VixError err = VIX_OK;
}
