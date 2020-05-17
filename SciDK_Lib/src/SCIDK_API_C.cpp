#include "stdafx.h"
#include "SCIDK_API_C.h"
#include "FTDI_CPP_DLL.h"
#include <math.h>
#include <stdlib.h>

using namespace NIUSBPHY;
struct 
{
	NI_USBPHY *niSciDK_HAL;
	int valid;
	unsigned int SN;
} Devices[MAX_NUMBER_OF_DEVICE];



SCIDK_API NI_RESULT SCIDK_ConnectUSB(char *SN, int *handle)
{
	int newdevid;
   
	int i;
	unsigned int reg;
	newdevid =-1;

		for (i=0;i<MAX_NUMBER_OF_DEVICE;i++)
		{
			if (Devices[i].valid == 0)
			{
				newdevid = i;
				break;
			}
		}
		if (newdevid == -1)
		{
			return NI_TOO_MANY_DEVICES_CONNECTED;
		}
		else
		{
			*handle = newdevid;
			Devices[*handle].niSciDK_HAL = new NI_USBPHY () ;


			if (Devices[*handle].niSciDK_HAL->OpenDeviceBySerialNumber (SN) == NI_OK )
			{
				Devices[*handle].valid = 1;
				NI_RESULT Status;
				Devices[*handle].SN = atoi(SN);
				return NI_OK;
			}
		

			//Connection failed
			delete Devices[*handle].niSciDK_HAL;
			return NI_ERROR;
		}

	
}

SCIDK_API NI_RESULT NI_CloseConnection(int handle)
{
	if (Devices[handle].valid == 1)
	{
		Devices[handle].valid = 0;
		Devices[handle].niSciDK_HAL->CloseConnection ();
		delete Devices[handle].niSciDK_HAL;
		return NI_OK;
	}
	else
	{
		return NI_INVALID_HANDLE;
	}
}




SCIDK_API NI_RESULT NI_USBEnumerate( tUSBDevice *pvArg1, unsigned int *numDevs	)
{
	NI_USBPHY enumClass;
	return enumClass.EnumerateUsbDevice(pvArg1,numDevs);
}



SCIDK_API NI_RESULT NI_WriteReg(UINT32 value, 
									  UINT32 address,
									  int handle	)
{
	NI_RESULT Status;
	if (Devices[handle].valid == 1)
	{
		Status = Devices[handle].niSciDK_HAL->WriteReg(value, address);
		return Status;
	}
	else
	{
		return NI_INVALID_HANDLE;
	}
}

SCIDK_API NI_RESULT NI_ReadReg(UINT32 *value, 
									  UINT32 address,
									  int handle	)
{
	NI_RESULT Status;
	if (Devices[handle].valid == 1)
	{
		Status = Devices[handle].niSciDK_HAL->ReadReg(value, address);
		return Status;
	}
	else
	{
		return NI_INVALID_HANDLE;
	}
}

SCIDK_API NI_RESULT NI_WriteData(UINT32 *value, 
									  UINT32 address,
									  UINT32 length,
									  int handle	)
{
	NI_RESULT Status;
	if (Devices[handle].valid == 1)
	{
		Status = Devices[handle].niSciDK_HAL->WriteToFPGA(value, address, length);
		return Status;
	}
	else
	{
		return NI_INVALID_HANDLE;
	}
}

SCIDK_API NI_RESULT NI_ReadData(UINT32 *value, 
									  UINT32 address,
									  UINT32 length,
									  int handle	)
{
	NI_RESULT Status;
	if (Devices[handle].valid == 1)
	{
		Status = Devices[handle].niSciDK_HAL->ReadFromFPGA(value, address, length);
		return Status;
	}
	else
	{
		return NI_INVALID_HANDLE;
	}
}




