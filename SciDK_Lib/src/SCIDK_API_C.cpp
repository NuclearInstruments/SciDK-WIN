#include "stdafx.h"
#include "SCIDK_API_C.h"
#include "FTDI_CPP_DLL.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

using namespace NIUSBPHY;
struct 
{
	NI_USBPHY *niSciDK_HAL;
	int valid;
	unsigned int SN;
} Devices[MAX_NUMBER_OF_DEVICE];



SCIDK_API NI_RESULT SCIDK_ConnectUSB(char *SN, NI_HANDLE *handle)
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

				uint32_t value;

				NI_WriteReg(0,
					0xFFFFFFFF,
					 handle);

				NI_ReadReg(&value,
					0xFFFFFFFF,
					handle);
				return NI_OK;
			}
		

			//Connection failed
			delete Devices[*handle].niSciDK_HAL;
			return NI_ERROR;
		}

	
}

SCIDK_API NI_RESULT NI_CloseConnection(NI_HANDLE *handle)
{
	if (Devices[*handle].valid == 1)
	{
		Devices[*handle].valid = 0;
		Devices[*handle].niSciDK_HAL->CloseConnection ();
		delete Devices[*handle].niSciDK_HAL;
		return NI_OK;
	}
	else
	{
		return NI_INVALID_HANDLE;
	}
}




SCIDK_API NI_RESULT NI_USBEnumerate( char *pvArg1, char* board_model, unsigned int *numDevs	)
{
	NI_USBPHY enumClass;
	return enumClass.EnumerateUsbDevice(pvArg1, board_model, numDevs);
}



SCIDK_API NI_RESULT NI_WriteReg(uint32_t value, 
									  uint32_t address,
	NI_HANDLE * handle	)
{
	NI_RESULT Status;
	if (Devices[*handle].valid == 1)
	{
		Status = Devices[*handle].niSciDK_HAL->WriteReg(value, address);
		return Status;
	}
	else
	{
		return NI_INVALID_HANDLE;
	}
}

SCIDK_API NI_RESULT NI_ReadReg(uint32_t *value, 
									  uint32_t address,
	NI_HANDLE * handle	)
{
	NI_RESULT Status;
	if (Devices[*handle].valid == 1)
	{
		Status = Devices[*handle].niSciDK_HAL->ReadReg(value, address);
		return Status;
	}
	else
	{
		return NI_INVALID_HANDLE;
	}
}

SCIDK_API NI_RESULT NI_WriteData(uint32_t *value, 
									  uint32_t length,
									  uint32_t address,
									  uint32_t BusMode,
									  uint32_t timeout_ms,
								      NI_HANDLE * handle,
									  uint32_t *written_data)
{
	NI_RESULT Status;
	if (Devices[*handle].valid == 1)
	{
		Status = Devices[*handle].niSciDK_HAL->WriteToFPGA(value, address, length, BusMode, timeout_ms);
		*written_data = length;
		return Status;
	}
	else
	{
		return NI_INVALID_HANDLE;
	}
}

SCIDK_API NI_RESULT NI_ReadData(uint32_t *value, 
									  uint32_t length,
									  uint32_t address,
									  uint32_t BusMode,
									  uint32_t timeout_ms,
									  NI_HANDLE * handle,
									  uint32_t *read_data,
									  uint32_t *valid_data)
{
	NI_RESULT Status;
	if (Devices[*handle].valid == 1)
	{
		Status = Devices[*handle].niSciDK_HAL->ReadFromFPGA(value, address, length, BusMode, timeout_ms);
		*read_data = length;
		*valid_data = length;
		return Status;
	}
	else
	{
		return NI_INVALID_HANDLE;
	}
}


SCIDK_API NI_RESULT NI_IICUser_OpenController(uint32_t ControlAddress, uint32_t StatusAddress, NI_HANDLE * handle, NI_IIC_HANDLE *IIC_Handle)
{
	return NI_ERROR_GENERIC;
}
SCIDK_API NI_RESULT NI_IICUser_ReadData(uint8_t address, uint8_t *value, int32_t len, uint8_t *value_read, int32_t len_read, NI_IIC_HANDLE *IIC_Handle)
{
	return NI_ERROR_GENERIC;
}
SCIDK_API NI_RESULT  NI_IICUser_WriteData(uint8_t address,
	uint8_t *value,
	int32_t len,
	NI_IIC_HANDLE *IIC_Handle)
{
	return NI_ERROR_GENERIC;
}
SCIDK_API char *ReadFirmwareInformationApp(NI_HANDLE * handle)
{
	return "SCIDK-TEST";
}


SCIDK_API NI_RESULT SECWriteWord(unsigned int address, unsigned int word, NI_HANDLE *handle)
{

	const uint32_t RFA_ICAPDNA_EEPROM = 0xFFFFFFFF;

	if (NI_WriteReg(1 << 24, RFA_ICAPDNA_EEPROM, handle) != 0)
		return -1;


	Sleep(10);
	if (NI_WriteReg((address << 16) + (2 << 24), RFA_ICAPDNA_EEPROM, handle) != 0)
		return -1;


	Sleep(10);

	if (NI_WriteReg(word + (address << 16) + (4 << 24), RFA_ICAPDNA_EEPROM, handle) != 0)
		return -1;



	Sleep(10);

	return 0;
}




/*
Read Security EEPROM
*/

SCIDK_API NI_RESULT SECReadWord(unsigned int address, unsigned int *word, NI_HANDLE *handle)
{
	const uint32_t RFA_ICAPDNA_EEPROM = 0xFFFFFFFF;
	uint32_t w;

	if (NI_WriteReg((address << 16) + (8 << 24), RFA_ICAPDNA_EEPROM, handle) != 0)
		return -1;


	Sleep(10);

	if (NI_ReadReg(&w, RFA_ICAPDNA_EEPROM, handle) != 0)
		return -1;


	*word = w & 0xFFFF;

	return 0;
}

SCIDK_API NI_RESULT ActivateEEPROM(char *key, uint32_t sn,  NI_HANDLE *handle) {
	if (strlen(key) != 32) return NI_INVALID_KEY;

	for (int i = 0; i < 8; i++) {
		char subst[5];
		memcpy(subst, &key[i * 4], 4);
		subst[4] = '\0';
		SECWriteWord(7-i, strtol(subst, NULL, 16),  handle);
	}
	SECWriteWord(8, sn & 0XFFFF, handle);
	SECWriteWord(9, (sn>>16) & 0XFFFF, handle);

	SECWriteWord(10, 1, handle);
	SECWriteWord(11, 0, handle);
}


SCIDK_API NI_RESULT ActivateRAM(char *key, uint32_t sn, NI_HANDLE *handle) {
	const uint32_t RFA_ICAPDNA_EEPROM = 0xFFFFFFFF;
	if (strlen(key) != 32) return NI_INVALID_KEY;

	for (int i = 0; i < 8; i++) {
		char subst[5];
		memcpy(subst, &key[i * 4], 4);
		subst[4] = '\0';
		uint16_t vv = strtol(subst, NULL, 16);
		if (NI_WriteReg((7-i << 16) + (0xf << 28) + vv, RFA_ICAPDNA_EEPROM, handle) != 0)
			return -1;

	}
	if (NI_WriteReg((8 << 16) + (0xf << 28) + (sn & 0XFFFF), RFA_ICAPDNA_EEPROM, handle) != 0)
		return -1;
	if (NI_WriteReg((9 << 16) + (0xf << 28) + ((sn>>16) & 0XFFFF), RFA_ICAPDNA_EEPROM, handle) != 0)
		return -1;
	if (NI_WriteReg((10 << 16) + (0xf << 28) + (1 & 0XFFFF), RFA_ICAPDNA_EEPROM, handle) != 0)
		return -1;
	if (NI_WriteReg((11 << 16) + (0xf << 28) + (0 & 0XFFFF), RFA_ICAPDNA_EEPROM, handle) != 0)
		return -1;

	if (NI_WriteReg((12 << 16) + (0xf << 28) + 1, RFA_ICAPDNA_EEPROM, handle) != 0)
		return -1;
	return NI_OK;
}


SCIDK_API NI_RESULT ReadLicenseInfo(char *DNA, uint32_t *sn, bool *Activated, bool *inGracePeriod, int *GraceSeconds, NI_HANDLE *handle) {
	const uint32_t RFA_ICAPDNA_EEPROM = 0xFFFFFFFF;
	uint32_t w,w2;

	if (NI_WriteReg(0x1 << 28, RFA_ICAPDNA_EEPROM, handle) != 0)
		return -1;

	if (NI_ReadReg(&w, RFA_ICAPDNA_EEPROM, handle) != 0)
		return -1;

	*Activated = (w >> 17) & 0x1;
	*inGracePeriod = (w >> 16) & 0x1;
	*GraceSeconds = (w & 0xFFFF);
	*GraceSeconds = (*GraceSeconds << 16) / 3250000;

	if (NI_WriteReg(0x2 << 28, RFA_ICAPDNA_EEPROM, handle) != 0)
		return -1;
	if (NI_ReadReg(&w, RFA_ICAPDNA_EEPROM, handle) != 0)
		return -1;
	if (NI_WriteReg(0x3 << 28, RFA_ICAPDNA_EEPROM, handle) != 0)
		return -1;
	if (NI_ReadReg(&w2, RFA_ICAPDNA_EEPROM, handle) != 0)
		return -1;

	sprintf(DNA, "%08x%08x", w2, w);

	if (NI_WriteReg(0x4 << 28, RFA_ICAPDNA_EEPROM, handle) != 0)
		return -1;
	if (NI_ReadReg(&w, RFA_ICAPDNA_EEPROM, handle) != 0)
		return -1;

	*sn = w;

	return 0;

}
