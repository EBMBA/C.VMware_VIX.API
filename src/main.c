#include <stdio.h>
#include <stdlib.h>
#include <vmware-vix/vix.h>
#include <vmware-vix/vm_basic_types.h>

#include "../include/powerSettings.h"


int main(int argc, char * argv[])
{
	VixHandle hostHandle = VIX_INVALID_HANDLE;
	VixHandle jobHandle = VIX_INVALID_HANDLE;
	int actionOnVM = -1;
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
	while (actionOnVM == -1)
	{
		printf("Choose an option :\n1. POWER SETTINGS \n : ");

		scanf("%d", &actionOnVM);

		switch (actionOnVM)
		{
		case 1:
			jobHandle = powerOn(hostHandle, argv[1]);
			Vix_ReleaseHandle(jobHandle);
			break;
		
		case 2:

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