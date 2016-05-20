#include<stdio.h>
#include"fileio_lfn.h"
#include "sd_spi.h"

FILEIO_SD_DRIVE_CONFIG sdCardMediaParameters =
{
	1,                                  // Use SPI module 1
	USER_SdSpiSetCs_1,                  // User-specified function to set/clear the Chip Select pin.
	USER_SdSpiGetCd_1,                  // User-specified function to get the status of the Card Detect pin.
	USER_SdSpiGetWp_1,                  // User-specified function to get the status of the Write Protect pin.
	USER_SdSpiConfigurePins_1           // User-specified function to configure the pins' TRIS bits.
};


// The gSDDrive structure allows the user to specify which set of driver functions should be used by the
// FILEIO library to interface to the drive.
// This structure must be maintained as long as the user wishes to access the specified drive.
const FILEIO_DRIVE_CONFIG gSdDrive =
{
	(FILEIO_DRIVER_IOInitialize)FILEIO_SD_IOInitialize,                      // Function to initialize the I/O pins used by the driver.
	(FILEIO_DRIVER_MediaDetect)FILEIO_SD_MediaDetect,                       // Function to detect that the media is inserted.
	(FILEIO_DRIVER_MediaInitialize)FILEIO_SD_MediaInitialize,               // Function to initialize the media.
	(FILEIO_DRIVER_MediaDeinitialize)FILEIO_SD_MediaDeinitialize,           // Function to de-initialize the media.
	(FILEIO_DRIVER_SectorRead)FILEIO_SD_SectorRead,                         // Function to read a sector from the media.
	(FILEIO_DRIVER_SectorWrite)FILEIO_SD_SectorWrite,                       // Function to write a sector to the media.
	(FILEIO_DRIVER_WriteProtectStateGet)FILEIO_SD_WriteProtectStateGet,     // Function to determine if the media is write-protected.
};

void c2s(const char* a, short* p)
{
	while (*a)*p++ = *a++ & 255;
	*p = 0;
}

int main()
{
	FILEIO_OBJECT file;
	char buf[16];
	short name[16];

	if (!FILEIO_Initialize())
	{
		return - 1;
	}

	if (FILEIO_MediaDetect(&gSdDrive, &sdCardMediaParameters) == false)
	{
		return -1;
	}

	if (FILEIO_DriveMount('A', &gSdDrive, &sdCardMediaParameters)!=FILEIO_ERROR_NONE)
	{
		return -1;
	}

	c2s("MSDOS.SYS", name);
	if (FILEIO_Open(&file, name, FILEIO_OPEN_READ) == FILEIO_RESULT_FAILURE)
	{
		return -1;
	}

	FILEIO_Read(buf, 1, 16, &file);

	FILEIO_Close(&file);

#if 1
	c2s("TEST.TXT", name);
	if (FILEIO_Open(&file, name, FILEIO_OPEN_READ) == FILEIO_RESULT_FAILURE)
	{
		return -1;
	}

	FILEIO_Read(buf,1,16,&file);

	FILEIO_Close(&file);
#endif

#if 0
	c2s("TEST.TXT", name);
	if (FILEIO_Open(&file, name, FILEIO_OPEN_WRITE | FILEIO_OPEN_CREATE) == FILEIO_RESULT_FAILURE)
	{
		return -1;
	}
	FILEIO_Write("abcdefghijk", 1, 10, &file);
	FILEIO_Close(&file);
#endif

	return 0;
}