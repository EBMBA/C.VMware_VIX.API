#if !defined(POWERSETTINGS_H)
#define POWERSETTINGS_H

#include <vmware-vix/vix.h>
#include <vmware-vix/vm_basic_types.h>
#include <stdio.h>
#include <stdlib.h>

VixHandle powerOn(VixHandle hostHandle, char *pathToVMX);
VixHandle pause(VixHandle hostHandle, char *pathToVMX);
VixHandle unPause(VixHandle hostHandle, char *pathToVMX);
VixHandle powerOff(VixHandle hostHandle, char *pathToVMX);
VixHandle reset(VixHandle hostHandle, char *pathToVMX);
VixHandle suspend(VixHandle hostHandle, char *pathToVMX);


#endif // POWERSETTINGS_H
