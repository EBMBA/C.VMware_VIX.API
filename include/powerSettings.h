/**
 * @file powerSettings.h
 * @author Emile METRAL (EBMBA)
 * @brief Control all power settings
 * @version 0.1
 * @date 2021-10-29
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#if !defined(POWERSETTINGS_H)
#define POWERSETTINGS_H

#include <vmware-vix/vix.h>
#include <vmware-vix/vm_basic_types.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * @brief Display the VM's power state
 * 
 * @param powerState VM's power state 
 */
void printPowerState(VixToolsState powerState);

/**
 * @brief Powers on a virtual machine. 
 * 
 * @param hostHandle Host handle
 * @param pathToVMX Path to the VM 
 * @return VixHandle 
 */
VixHandle powerOn(VixHandle hostHandle, char *pathToVMX);

/**
 * @brief This function pauses a virtual machine.
 * 
 * @param hostHandle Host handle
 * @param pathToVMX Path to the VM 
 * @return VixHandle 
 */
VixHandle pause(VixHandle hostHandle, char *pathToVMX);

/**
 * @brief This function continues execution of a paused virtual machine. 
 * 
 * @param hostHandle Host handle
 * @param pathToVMX Path to the VM 
 * @return VixHandle 
 */
VixHandle unPause(VixHandle hostHandle, char *pathToVMX);

/**
 * @brief This function powers off a virtual machine.
 * 
 * @param hostHandle Host handle
 * @param pathToVMX Path to the VM 
 * @return VixHandle 
 */
VixHandle powerOff(VixHandle hostHandle, char *pathToVMX);

/**
 * @brief This function resets a virtual machine. 
 * 
 * @param hostHandle Host handle
 * @param pathToVMX Path to the VM 
 * @return VixHandle 
 */
VixHandle reset(VixHandle hostHandle, char *pathToVMX);

/**
 * @brief  This function suspends a virtual machine. 
 * 
 * @param hostHandle Host handle
 * @param pathToVMX Path to the VM 
 * @return VixHandle 
 */
VixHandle suspend(VixHandle hostHandle, char *pathToVMX);


#endif // POWERSETTINGS_H
