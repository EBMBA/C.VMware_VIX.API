/**
 * @file main.c
 * @author Emile METRAL (EBMBA)
 * @brief Controll your VMWare Worjstation's VM in cmd
 * @version 0.1
 * @date 2021-10-29
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <vmware-vix/vix.h>
#include <vmware-vix/vm_basic_types.h>

#include "../include/powerSettings.h"
#include "../include/vmwareTools.h"
#include "../include/snapshots.h"

/**
 * @brief MAIN
 * 
 * @param argc 
 * @param argv Arguments
 * @return int 
 */
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
		printf("Choose an option :\n1. POWER SETTINGS \n2. INSTALL VMWARE TOOLS \n3. SNAPSHOT MANAGEMENT \nYOUR CHOICE: ");

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
		
		case 2:
			jobHandle = installTools(hostHandle,argv[1]);
			Vix_ReleaseHandle(jobHandle);
		
		case 3:
			while (secondMenu == -1)
			{
				printf("Choose an option :\n1. CREATE SNAPSHOT \n2. REMOVE SNAPSHOT \n3.  \n4.  \n5.  \n6.  \nYOUR CHOICE: ");
				scanf("%d", &secondMenu);
				printf("\n");

				switch (secondMenu)
				{
				case 1:

					Snapshot snapshotParameters;

					// To solve the problem when using scanf before fgets (src: https://www.go4expert.com/articles/solution-using-scanf-fgets-c-t27148/)
					getchar();

					printf("Enter the snapshost's name : ");
					//scanf("%s", snapshotParameters.name);
					fgets(snapshotParameters.name, MAX_SIZE_NAME, stdin);
					printf("\n");

					printf("Enter the snapshost's description : ");
					fgets(snapshotParameters.description, MAX_SIZE_DESCRIPTION, stdin);
					printf("\n");

					jobHandle = createSnapshot(hostHandle, argv[1], snapshotParameters);
					Vix_ReleaseHandle(jobHandle);
					break;
				
				case 2:
					jobHandle = getSnapshotsList(hostHandle, argv[1]);
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
		default:
			break;
		}

	}
	


	abort:
		Vix_ReleaseHandle(jobHandle);
		Vix_ReleaseHandle(hostHandle);

	return (0);
}