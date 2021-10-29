#include <stdio.h>
#include <stdlib.h>
#include <vmware-vix/vix.h>
#include <vmware-vix/vm_basic_types.h>

#include "../include/powerSettings.h"


int main(int argc, char * argv[])
{
	VixHandle hostHandle = VIX_INVALID_HANDLE;
	VixHandle jobHandle = VIX_INVALID_HANDLE;
	int firstMenu = -1, secondMenu = -1;
	VixError err;

	// Check if program have a correct argument 
	if (argv[1] == NULL)
	{
		printf("Program have to be called like : %s /path/to/vm.vmx\n", argv[0]);
		goto abort;
	}
	 

	// Connect as current user on local host
	jobHandle = VixHost_Connect(	VIX_API_VERSION,
									VIX_SERVICEPROVIDER_VMWARE_WORKSTATION,
									NULL,
									0,
									NULL,
									NULL,
									0,
									VIX_INVALID_HANDLE,
									NULL,
									NULL);
	
	err = VixJob_Wait(jobHandle,
						VIX_PROPERTY_JOB_RESULT_HANDLE,
						&hostHandle,
						VIX_PROPERTY_NONE);


	if (VIX_OK != err)
	{
		printf("Error %ld during the process\n", err);
		goto abort;
	}
	printf("Host connected !\n");

	Vix_ReleaseHandle(jobHandle);

	// Menu :
	while (firstMenu == -1)
	{
		printf("Choose an option :\n1. POWER SETTINGS \n \nYOUR CHOICE: ");

		scanf("%d", &firstMenu);

		switch (firstMenu)
		{
		case 1:
			while (secondMenu == -1)
			{
				printf("Choose an option :\n1. POWER ON \n2. POWER OFF \n3. PAUSE \n4. UNPAUSE \n5. RESET \n6. SUSPEND \nYOUR CHOICE: ");
				scanf("%d", &secondMenu);
				printf("\n");

				switch (secondMenu)
				{
				case 1:
					jobHandle = powerOn(hostHandle, argv[1]);
					Vix_ReleaseHandle(jobHandle);
					break;
				
				case 2:
					printf("WARNING : VMWARE TOOLS HAVE TO BE INSTALLED !\n");
					jobHandle = powerOff(hostHandle, argv[1]);
					Vix_ReleaseHandle(jobHandle);
					break;
				
				case 3:
					jobHandle = pause(hostHandle, argv[1]);
					Vix_ReleaseHandle(jobHandle);
					break;

				case 4:
					jobHandle = unPause(hostHandle, argv[1]);
					Vix_ReleaseHandle(jobHandle);
					break;

				case 5:
					printf("WARNING : The virtual machine will be powered off at the hardware level. Any state of the guest that has not been committed to disk will be lost.\n");
					jobHandle = reset(hostHandle, argv[1]);
					Vix_ReleaseHandle(jobHandle);
					break;
				
				case 6:
					jobHandle = suspend(hostHandle, argv[1]);
					Vix_ReleaseHandle(jobHandle);
					break;

				default:
					break;
				}
			}
			
			break;
		
		default:
			break;
		}

	}
	


	abort:
		Vix_ReleaseHandle(jobHandle);
		Vix_ReleaseHandle(hostHandle);

	return (0);
}