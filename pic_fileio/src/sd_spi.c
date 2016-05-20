
#include "system_config.h"
#include "fileio.h"
#include "fileio_private.h"
#include "system.h"
#include "sd_spi.h"
#include "sd_spi_private.h"

#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>


#define DISK_IMAGE "disk.img"

// Description:  Used for the mass-storage library to determine capacity
uint32_t finalLBA;
uint16_t gMediaSectorSize;
static FILEIO_MEDIA_INFORMATION mediaInformation;


bool FILEIO_SD_MediaDetect (FILEIO_SD_DRIVE_CONFIG * config)
{
	return true;
}//end MediaDetect


void FILEIO_SD_IOInitialize (FILEIO_SD_DRIVE_CONFIG * config)
{
}


bool FILEIO_SD_MediaDeinitialize(FILEIO_SD_DRIVE_CONFIG * config)
{
    return true;
}


bool FILEIO_SD_SectorRead(FILEIO_SD_DRIVE_CONFIG * config, uint32_t sectorAddress, uint8_t* buffer)
{
	FILE* fp;
	int sz,csz;
	fp = fopen(DISK_IMAGE, "rb");
	if (fp == NULL)return false;
	fseek(fp, 0, FILEIO_SEEK_END);
	sz = ftell(fp);
	csz = sz / 512;
	fseek(fp, 512 * sectorAddress, SEEK_SET);
	fread(buffer, 1, 512, fp);
	fclose(fp);
	return true;
}    


bool FILEIO_SD_SectorWrite(FILEIO_SD_DRIVE_CONFIG * config, uint32_t sectorAddress, uint8_t* buffer, bool allowWriteToZero)
{
	FILE* fp;
	int sz, csz;
	fp = fopen(DISK_IMAGE, "r+b");
	if (fp == NULL)return false;
	fseek(fp, 0, FILEIO_SEEK_END);
	sz = ftell(fp);
	csz = sz / 512;
	fseek(fp, 512 * sectorAddress, SEEK_SET);
	fwrite(buffer, 1, 512, fp);
	fclose(fp);
	return true;
}    


bool FILEIO_SD_WriteProtectStateGet(FILEIO_SD_DRIVE_CONFIG * config)
{
    return false;
}

FILEIO_MEDIA_INFORMATION *  FILEIO_SD_MediaInitialize (FILEIO_SD_DRIVE_CONFIG * config)
{
	mediaInformation.errorCode = MEDIA_NO_ERROR;
	mediaInformation.validityFlags.value = 0;
	return &mediaInformation;
}//end MediaInitialize

