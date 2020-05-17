
#ifndef __SCIDK_API_C_H
#define __SCIDK_API_C_H

#include "NIErrorCode.h"

#ifdef __cplusplus
extern "C" {
#endif


#ifdef SCIDK_API_EXPORTS
#define SCIDK_API extern "C" __declspec(dllexport) 
	//__declspec(dllexport)
#else
#define SCIDK_API __declspec(dllimport)
#endif

#define MAX_NUMBER_OF_DEVICE (100)



SCIDK_API NI_RESULT SCIDK_ConnectUSB(char *SN, int *handle);


SCIDK_API NI_RESULT NI_CloseConnection(int handle);

SCIDK_API NI_RESULT NI_USBEnumerate(tUSBDevice *pvArg1, unsigned int *numDevs);

SCIDK_API NI_RESULT NI_WriteReg(UINT32 value,
									  UINT32 address,
									  int handle	);

SCIDK_API NI_RESULT NI_ReadReg(UINT32 *value,
									  UINT32 address,
									  int handle	);

SCIDK_API NI_RESULT NI_WriteData(UINT32 *value,
									  UINT32 address,
									  UINT32 length,
									  int handle	);

SCIDK_API NI_RESULT NI_ReadData(UINT32 *value,
									  UINT32 address,
									  UINT32 length,
									  int handle	);


#ifdef __cplusplus
}
#endif

#endif // __SCIDK_API_C_H