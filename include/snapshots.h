#if !defined(snapshots_h)
#define snapshots_h

// Include parts :
#include <vmware-vix/vix.h>
#include <vmware-vix/vm_basic_types.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_SIZE_NAME 100
#define MAX_SIZE_DESCRIPTION 1000
#define DEFAULT_SNAPSHOT_HEAP_SIZE 100

/**
 * @brief Each snapshot have a name and a description (null or not)
 * 
 */
typedef struct 
{
    char *name;
    char *description;
    int indexFirstChild;
    int numChild;
} Snapshot;

typedef struct 
{
    Snapshot data[DEFAULT_SNAPSHOT_HEAP_SIZE];
    int index;
}SnapshotHeap; 

/**
 * @brief Initialise a snapshot heap
 * 
 * @param snapshotHeap 
 * @return int 0 to succes or 1 for an error  
 */
int initSnapshotHeap(SnapshotHeap snapshotHeap);

/**
 * @brief Memory allocation for Snapshot variable 
 * 
 * @param snapshot 
 * @return int 0 to succes or 1 for an error  
 */
int allocSnapshot(Snapshot snapshot);

VixHandle createSnapshot(VixHandle hostHandle, char *pathToVMX, Snapshot snapshotParameters);

VixHandle removeSnapshot(VixHandle hostHandle, char *pathToVMX);

VixHandle getSnapshotsList(VixHandle hostHandle, char *pathToVMX);


#endif // snapshots_h
