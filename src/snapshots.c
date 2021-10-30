/**
 * @file snapshots.c
 * @author Emile METRAL (EBMBA)
 * @brief Manage VM's snapshot 
 * @version 0.1
 * @date 2021-10-30
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include "../include/snapshots.h"
#include "../include/powerSettings.h"

VixHandle createSnapshot(VixHandle hostHandle, char *pathToVMX, Snapshot snapshotParameters)
{
    VixHandle jobHandle = VIX_INVALID_HANDLE;
    VixHandle vmHandle = VIX_INVALID_HANDLE;
    VixToolsState powerState = VIX_POWERSTATE_POWERED_ON; // See : https://www.vmware.com/support/developer/vix-api/vix16_reference/types/VixPowerState.html
    VixHandle snapshotHandle = VIX_INVALID_HANDLE;
    VixError err = VIX_OK;

    /// OPEN VM
    jobHandle = VixVM_Open(hostHandle,
                           pathToVMX,
                           NULL,
                           NULL);

    err = VixJob_Wait(jobHandle,
                      VIX_PROPERTY_JOB_RESULT_HANDLE,
                      &vmHandle,
                      VIX_PROPERTY_NONE);

    if (VIX_OK != err)
    {
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
    if (VIX_OK != err || powerState != 8)
    {
        // Handle the error...
        if (powerState != 8)
        {
            printPowerState(powerState);
        }
        goto abort;
    }

    /// CREATE SNAPSHOT
    jobHandle = VixVM_CreateSnapshot(vmHandle,
                                     snapshotParameters.name,
                                     snapshotParameters.description,
                                     VIX_SNAPSHOT_INCLUDE_MEMORY,
                                     VIX_INVALID_HANDLE,
                                     NULL,
                                     NULL);

    err = VixJob_Wait(jobHandle,
                      VIX_PROPERTY_JOB_RESULT_HANDLE,
                      &snapshotHandle,
                      VIX_PROPERTY_NONE);

abort:
    Vix_ReleaseHandle(jobHandle);
    Vix_ReleaseHandle(vmHandle);
    Vix_ReleaseHandle(snapshotHandle);

    return jobHandle;
}

VixHandle getSnapshotsList(VixHandle hostHandle, char *pathToVMX)
{
    VixHandle jobHandle = VIX_INVALID_HANDLE;
    VixHandle vmHandle = VIX_INVALID_HANDLE;
    VixHandle snapshotHandle = VIX_INVALID_HANDLE;
    VixHandle parentSnapshotHandle = VIX_INVALID_HANDLE;
    VixError err = VIX_OK;
    char *snapshotName = NULL;
    char *snapshotDescription = NULL;
    int numRootSnapshots = 0;
    int numChildSnapshots = 0;
    int numParentSnapshots = 0;

    jobHandle = VixVM_Open(hostHandle,
                           pathToVMX,
                           NULL,
                           NULL);

    err = VixJob_Wait(jobHandle,
                      VIX_PROPERTY_JOB_RESULT_HANDLE,
                      &vmHandle,
                      VIX_PROPERTY_NONE);

    if (VIX_OK != err)
    {
        // Handle the error...
        goto abort;
    }

    Vix_ReleaseHandle(jobHandle);

    /*err = VixVM_GetNumRootSnapshots(vmHandle, &numRootSnapshots);
    printf("Num of Snapshot : %d\n", numRootSnapshots);
    if (VIX_OK != err)
    {
        // Handle the error...
        goto abort;
    }

    for (int index = 0; index < numRootSnapshots; index++)
    {
        err = VixVM_GetRootSnapshot(vmHandle,
                                    index,
                                    &snapshotHandle);
        if (VIX_OK != err)
        {
            // Handle the error...
            goto abort;
        }

        err = Vix_GetProperties(snapshotHandle,
                                VIX_PROPERTY_SNAPSHOT_DISPLAYNAME,
                                &snapshotName,
                                VIX_PROPERTY_SNAPSHOT_DESCRIPTION,
                                &snapshotDescription,
                                VIX_PROPERTY_NONE);
        if (VIX_OK != err)
        {
            // Handle the error...
            goto abort;
        }
        printf("%d snapshot name is %s and description is %s\n", index, snapshotName, snapshotDescription);

        err = VixSnapshot_GetNumChildren(snapshotHandle, &numChildSnapshots);
        printf("the snapshot has %d child snapshots\n", numChildSnapshots);
        if (VIX_OK != err)
        {
            // Handle the error...
            goto abort;
        }
    }
    */
    /// Current snapshots :
    err = VixVM_GetCurrentSnapshot(vmHandle,
                                   &snapshotHandle);
    if (VIX_OK != err)
    {
        // Handle the error...
        goto abort;
    }

    err = Vix_GetProperties(snapshotHandle,
                            VIX_PROPERTY_SNAPSHOT_DISPLAYNAME,
                            &snapshotName,
                            VIX_PROPERTY_SNAPSHOT_DESCRIPTION,
                            &snapshotDescription,
                            VIX_PROPERTY_NONE);
    if (VIX_OK != err)
    {
        // Handle the error...
        goto abort;
    }
    printf("%d. Current snapshot name is %s and description is %s\n",numParentSnapshots, snapshotName, snapshotDescription);
    
    err = VixSnapshot_GetParent(snapshotHandle,
                                &parentSnapshotHandle);
    
    if (VIX_OK != err)
    {
        // Handle the error...
        printf("No parents snapshot\n");
        goto abort;
    }

    while (VIX_OK == err)
    {
        err = Vix_GetProperties(parentSnapshotHandle,
                            VIX_PROPERTY_SNAPSHOT_DISPLAYNAME,
                            &snapshotName,
                            VIX_PROPERTY_SNAPSHOT_DESCRIPTION,
                            &snapshotDescription,
                            VIX_PROPERTY_NONE);
        if (VIX_OK != err)
        {
            // Handle the error...
            goto abort;
        }
        numParentSnapshots ++;
        printf("%d. Snapshot name is %s and description is %s\n",numParentSnapshots, snapshotName, snapshotDescription);
        snapshotHandle = parentSnapshotHandle;
        err = VixSnapshot_GetParent(snapshotHandle,
                                &parentSnapshotHandle);
    }
    
    abort:
        Vix_ReleaseHandle(jobHandle);
        Vix_ReleaseHandle(snapshotHandle);
        Vix_ReleaseHandle(vmHandle);
        VixHost_Disconnect(hostHandle);
        Vix_FreeBuffer(snapshotName);

    return snapshotHandle;
}