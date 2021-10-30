#if !defined(snapshots_h)
#define snapshots_h

// Include parts :
#include <vmware-vix/vix.h>
#include <vmware-vix/vm_basic_types.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_SIZE_NAME 100
#define MAX_SIZE_DESCRIPTION 1000

typedef struct 
{
    char name[MAX_SIZE_NAME];
    char description[MAX_SIZE_DESCRIPTION];
} Snapshot;


VixHandle createSnapshot(VixHandle hostHandle, char *pathToVMX, Snapshot snapshotParameters);

VixHandle removeSnapshot(VixHandle hostHandle, char *pathToVMX);

VixHandle getSnapshotsList(VixHandle hostHandle, char *pathToVMX);


#endif // snapshots_h
