#include "NIErrorCode.h"	
#include "ftd2xx.h"
namespace NIUSBPHY
{
    class NI_USBPHY
    {
    public:
	NI_USBPHY::NI_USBPHY();
	NI_USBPHY::~NI_USBPHY();
	NI_RESULT EnumerateUsbDevice (tUSBDevice *pvArg1, unsigned int *numDevs);
	NI_RESULT OpenDeviceBySerialNumber (char *SN);
	NI_RESULT CloseConnection ();
	NI_RESULT WriteToFPGA (unsigned int *d, unsigned int addr, unsigned int length);
	NI_RESULT ReadFromFPGA (unsigned int *d, unsigned int addr, unsigned int length);
	NI_RESULT WriteReg (unsigned int regVal, unsigned int addr);
	NI_RESULT ReadReg (unsigned int *regVal, unsigned int addr);
	private:
	FT_HANDLE ftHandle; 
	int ConnectionStatus;



    };
}
