// SCIDK_TestApp.cpp : definisce il punto di ingresso dell'applicazione console.
//

#include "stdafx.h"
#include "../SciDK_Lib/include/SCIDK_API_C.h"

int main()
{
	UINT32 reg_value;
	int handle;
	tUSBDevice devices[50];
	unsigned int found_devs = 0;
	NI_USBEnumerate(devices, &found_devs);

	if (found_devs > 0)
	{
		if (SCIDK_ConnectUSB(devices[0].SN, &handle) == NI_OK)
		{
			NI_ReadReg(&reg_value, 0xFFFFFFFF, handle);
			printf("Reg value: %X\n", reg_value);
			NI_WriteReg(0x100, 0, handle);
			NI_ReadReg(&reg_value, 0, handle);
			printf("Reg 0 value: %X\n", reg_value);
			NI_WriteReg(0xFF, 1, handle);
			NI_ReadReg(&reg_value, 1, handle);
			printf("Reg 1 value: %X\n", reg_value);
		}
	}
	else
	{
		printf("No device found\n", reg_value);
	}

	
    return 0;
}

