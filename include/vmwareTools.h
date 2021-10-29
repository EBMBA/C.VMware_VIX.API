#if !defined(vmwareTools_h)
#define vmwareTools_h
#include <vmware-vix/vix.h>
#include <vmware-vix/vm_basic_types.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * @brief  Prepares to install VMware Tools on the guest operating system. 
 * 
 * @param hostHandle Host handle
 * @param pathToVMX Path to the VM 
 * @return VixHandle 
 */
VixHandle installTools(VixHandle hostHandle, char *pathToVMX);

#endif // vmwareTools_h
