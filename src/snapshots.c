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

/**
 * @brief Initialise a snapshot heap
 * 
 * @param snapshotHeap 
 * @return int 0 to succes or 1 for an error  
 */
int initSnapshotHeap(SnapshotHeap snapshotHeap)
{
    snapshotHeap.index = 0;
    int isAllocated = 0;

    for (int i = 0; i < DEFAULT_SNAPSHOT_HEAP_SIZE; i++)
    {
        snapshotHeap.data[i].name = malloc(MAX_SIZE_NAME * sizeof(char));
        snapshotHeap.data[i].description = malloc(MAX_SIZE_DESCRIPTION * sizeof(char));

        if (snapshotHeap.data[i].name == NULL && snapshotHeap.data[i].description == NULL)
        {
            isAllocated = 1;
            break;
        }
    }

    return isAllocated;
}

/**
 * @brief Memory allocation for Snapshot variable 
 * 
 * @param snapshot 
 * @return int 0 to succes or 1 for an error 
 */
int allocSnapshot(Snapshot snapshot)
{
    snapshot.name = malloc(MAX_SIZE_NAME * sizeof(char));
    snapshot.description = malloc(MAX_SIZE_DESCRIPTION * sizeof(char));

    if (snapshot.name != NULL && snapshot.description != NULL)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

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
    VixHandle childSnapshotHandle = VIX_INVALID_HANDLE;
    VixError err = VIX_OK;
    // char *snapshotName = NULL;
    // char *snapshotDescription = NULL;
    // Snapshot snapshot;

    // int isAllocated = allocSnapshot(snapshot);

    // if (isAllocated == 1)
    // {
    //     goto abort;
    // }

    SnapshotHeap snapshotHeap;

    initSnapshotHeap(snapshotHeap);

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

    err = VixVM_GetNumRootSnapshots(vmHandle, &numRootSnapshots);
    printf("Num of Snapshot : %d\n", numRootSnapshots);
    if (VIX_OK != err)
    {
        // Handle the error...
        goto abort;
    }

    for (int indexRoot = 0; indexRoot < numRootSnapshots; indexRoot++)
    {
        err = VixVM_GetRootSnapshot(vmHandle,
                                    indexRoot,
                                    &snapshotHandle);
        if (VIX_OK != err)
        {
            // Handle the error...
            goto abort;
        }

        err = Vix_GetProperties(snapshotHandle,
                                VIX_PROPERTY_SNAPSHOT_DISPLAYNAME,
                                &snapshotHeap.data[indexRoot].name,
                                VIX_PROPERTY_SNAPSHOT_DESCRIPTION,
                                &snapshotHeap.data[indexRoot].description,
                                VIX_PROPERTY_NONE);
        if (VIX_OK != err)
        {
            // Handle the error...
            goto abort;
        }
        // Increment index 
        snapshotHeap.index ++;

        printf("%d snapshot name is %s and description is %s\n", indexRoot, snapshotHeap.data[indexRoot].name, snapshotHeap.data[indexRoot].description);

        err = VixSnapshot_GetNumChildren(snapshotHandle, &snapshotHeap.data[indexRoot].numChild);

        if (VIX_OK != err)
        {
            // Handle the error...
            goto abort;
        }
        printf("the snapshot has %d child(ren) snapshot(s)\n", snapshotHeap.data[indexRoot].numChild);
        
        // first child is 
        if (indexRoot == 0)
        {
            snapshotHeap.data[indexRoot].indexFirstChild = numRootSnapshots;
        }
        else{
            snapshotHeap.data[indexRoot].indexFirstChild = numRootSnapshots + snapshotHeap.data[indexRoot-1].numChild ;
        }
        

        // Store all children
        for (int indexChildren = 0; indexChildren < snapshotHeap.data[indexRoot].numChild; indexChildren++)
        {
            err = VixSnapshot_GetChild(snapshotHandle, indexChildren, &childSnapshotHandle);
            if (VIX_OK != err)
            {
                // Handle the error...
                goto abort;
            }

            // Write information on snapshot in (index of current child + index of root + num of root) index
            //int indexWriteInformation = indexChildren + indexRoot + numRootSnapshots ;
            // Or just index of the current child + index of the first children :
            int indexWriteInformation = indexChildren + snapshotHeap.data[indexRoot].indexFirstChild;

            err = Vix_GetProperties(childSnapshotHandle,
                                    VIX_PROPERTY_SNAPSHOT_DISPLAYNAME,
                                    &snapshotHeap.data[indexWriteInformation].name,
                                    VIX_PROPERTY_SNAPSHOT_DESCRIPTION,
                                    &snapshotHeap.data[indexWriteInformation].description,
                                    VIX_PROPERTY_NONE);
            if (VIX_OK != err)
            {
                // Handle the error...
                goto abort;
            }
            // Increment index
            snapshotHeap.index ++;
            
            printf("%d snapshot name is %s and description is %s\n", indexWriteInformation, snapshotHeap.data[indexWriteInformation].name, snapshotHeap.data[indexWriteInformation].description);
        }

        // Free snapshot handle to the next root 
        Vix_ReleaseHandle(snapshotHandle);
    }

    // /// Current snapshots :
    // err = VixVM_GetCurrentSnapshot(vmHandle,
    //                                &snapshotHandle);
    // if (VIX_OK != err)
    // {
    //     // Handle the error...
    //     goto abort;
    // }

    // err = Vix_GetProperties(snapshotHandle,
    //                         VIX_PROPERTY_SNAPSHOT_DISPLAYNAME,
    //                         &snapshot.name,
    //                         VIX_PROPERTY_SNAPSHOT_DESCRIPTION,
    //                         &snapshot.description,
    //                         VIX_PROPERTY_NONE);
    // if (VIX_OK != err)
    // {
    //     // Handle the error...
    //     goto abort;
    // }
    // printf("%d. Current snapshot name is %s and description is %s\n", numParentSnapshots, snapshot.name, snapshot.description);

    // err = VixSnapshot_GetParent(snapshotHandle,
    //                             &parentSnapshotHandle);

    // if (VIX_OK != err)
    // {
    //     // Handle the error...
    //     printf("No parents snapshot\n");
    //     goto abort;
    // }

    // while (VIX_OK == err)
    // {
    //     err = Vix_GetProperties(parentSnapshotHandle,
    //                             VIX_PROPERTY_SNAPSHOT_DISPLAYNAME,
    //                             &snapshotName,
    //                             VIX_PROPERTY_SNAPSHOT_DESCRIPTION,
    //                             &snapshotDescription,
    //                             VIX_PROPERTY_NONE);
    //     if (VIX_OK != err)
    //     {
    //         // Handle the error...
    //         goto abort;
    //     }
    //     numParentSnapshots++;
    //     printf("%d. Snapshot name is %s and description is %s\n", numParentSnapshots, snapshotName, snapshotDescription);
    //     snapshotHandle = parentSnapshotHandle;
    //     err = VixSnapshot_GetParent(snapshotHandle,
    //                                 &parentSnapshotHandle);
    // }

abort:
    Vix_ReleaseHandle(jobHandle);
    Vix_ReleaseHandle(snapshotHandle);
    Vix_ReleaseHandle(vmHandle);
    Vix_ReleaseHandle(childSnapshotHandle);
    //VixHost_Disconnect(hostHandle);
    //Vix_FreeBuffer(snapshotName);

    return snapshotHandle;
}