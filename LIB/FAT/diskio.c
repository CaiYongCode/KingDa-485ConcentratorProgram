/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2013        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control module to the FatFs module with a defined API.        */
/*-----------------------------------------------------------------------*/

#include "diskio.h"		/* FatFs lower layer API */
#include "..\..\BSP\INC\W25QXX.h"  
//#include "usbdisk.h"	/* Example: USB drive control */
//#include "atadrive.h"	/* Example: ATA drive control */
//#include "sdcard.h"		/* Example: MMC/SDC contorl */

/* Definitions of physical drive number for each media */
#define ATA		0
#define MMC		1
#define USB		2


DSTATUS ATA_disk_initialize(void);
DSTATUS MMC_disk_initialize(void);
DSTATUS USB_disk_initialize(void);

DSTATUS ATA_disk_status(void);
DSTATUS MMC_disk_status(void);
DSTATUS USB_disk_status(void);

DRESULT ATA_disk_read(BYTE *buff, DWORD sector, UINT count);
DRESULT MMC_disk_read(BYTE *buff, DWORD sector, UINT count);
DRESULT USB_disk_read(BYTE *buff, DWORD sector, UINT count);

DRESULT ATA_disk_write (const BYTE *buff, DWORD sector, UINT count);
DRESULT MMC_disk_write (const BYTE *buff, DWORD sector, UINT count);
DRESULT USB_disk_write (const BYTE *buff, DWORD sector, UINT count);

DRESULT ATA_disk_ioctl(BYTE cmd, void *buff);
DRESULT MMC_disk_ioctl(BYTE cmd, void *buff);
DRESULT USB_disk_ioctl(BYTE cmd, void *buff);


/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
	BYTE pdrv				/* Physical drive nmuber (0..) */
)
{
	DSTATUS stat;
	//int result;

	switch (pdrv) {
	case ATA :
		stat = ATA_disk_initialize();

		// translate the reslut code here

		return stat;

	case MMC :
		stat = MMC_disk_initialize();

		// translate the reslut code here

		return stat;

	case USB :
		stat = USB_disk_initialize();

		// translate the reslut code here

		return stat;
	}
	return STA_NOINIT;
}



/*-----------------------------------------------------------------------*/
/* Get Disk Status                                                       */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
	BYTE pdrv		/* Physical drive nmuber (0..) */
)
{
	DSTATUS stat;
	//int result;

	switch (pdrv) {
	case ATA :
		stat = ATA_disk_status();

		// translate the reslut code here

		return stat;

	case MMC :
		stat = MMC_disk_status();

		// translate the reslut code here

		return stat;

	case USB :
		stat = USB_disk_status();

		// translate the reslut code here

		return stat;
	}
	return STA_NOINIT;
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE *buff,		/* Data buffer to store read data */
	DWORD sector,	/* Sector address (LBA) */
	UINT count		/* Number of sectors to read (1..128) */
)
{
	DRESULT res;   
	//int result;

	switch (pdrv) {
	case ATA :
		// translate the arguments here

		res = ATA_disk_read(buff, sector, count);

		// translate the reslut code here

		return res;

	case MMC :
		// translate the arguments here

		res = MMC_disk_read(buff, sector, count);

		// translate the reslut code here

		return res;

	case USB :
		// translate the arguments here

		res = USB_disk_read(buff, sector, count);

		// translate the reslut code here

		return res;
	}
	return RES_PARERR;
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

#if _USE_WRITE
DRESULT disk_write (
	BYTE pdrv,			/* Physical drive nmuber (0..) */
	const BYTE *buff,	/* Data to be written */
	DWORD sector,		/* Sector address (LBA) */
	UINT count			/* Number of sectors to write (1..128) */
)
{
	DRESULT res;
	//int result;

	switch (pdrv) {
	case ATA :
		// translate the arguments here

		res = ATA_disk_write(buff, sector, count);

		// translate the reslut code here

		return res ;

	case MMC :
		// translate the arguments here

		res = MMC_disk_write(buff, sector, count);

		// translate the reslut code here

		return res ;

	case USB :
		// translate the arguments here

		res = USB_disk_write(buff, sector, count);

		// translate the reslut code here

		return res ;
	}
	return RES_PARERR;
}
#endif


/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

#if _USE_IOCTL
DRESULT disk_ioctl (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE cmd,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
	DRESULT res;
//	int result;

	switch (pdrv) {
	case ATA :
		// pre-process here

		res = ATA_disk_ioctl(cmd, buff);

		// post-process here

		return res ;

	case MMC :
		// pre-process here

		res = MMC_disk_ioctl(cmd, buff);

		// post-process here

		return res;

	case USB :
		// pre-process here

		res = USB_disk_ioctl(cmd, buff);

		// post-process here

		return res;
	}
	return RES_PARERR;
}


DSTATUS ATA_disk_initialize(void)
{
 Launch_W25QXX ( ); 
 return RES_OK;
}

DSTATUS MMC_disk_initialize(void)
{
 return RES_OK;
}

DSTATUS USB_disk_initialize(void)
{
 return RES_OK;
}

DSTATUS ATA_disk_status(void)
{
 return RES_OK;
}

DSTATUS MMC_disk_status(void)
{
 return RES_OK;
}

DSTATUS USB_disk_status(void)
{
 return RES_OK;
}


DRESULT ATA_disk_read(BYTE *buff, DWORD sector, UINT count)
{
 Flash_Read(sector << 12 ,buff,count <<12);
 return RES_OK;
}

DRESULT MMC_disk_read(BYTE *buff, DWORD sector, UINT count)
{
 return RES_OK;
}

DRESULT USB_disk_read(BYTE *buff, DWORD sector, UINT count)
{
 return RES_OK;
}

DRESULT ATA_disk_write (const BYTE *buff, DWORD sector, UINT count)
{
 Page_Erase(SECTOR_ERASE_4K ,sector << 12);
 Writedata_NoCheck_W25Q( sector<<12 , (BYTE *)buff, count << 12 );
 return RES_OK;
}

DRESULT MMC_disk_write (const BYTE *buff, DWORD sector, UINT count)
{
 return RES_OK;
}

DRESULT USB_disk_write (const BYTE *buff, DWORD sector, UINT count)
{
 return RES_OK;
}

DRESULT ATA_disk_ioctl(BYTE cmd, void *buff)
{
 DRESULT res;
 //BYTE n, csd[16];
 //DWORD *dp, st, ed, csize;GET_SECTOR_SIZE
 res = RES_ERROR;
 switch (cmd) 
 {
  case CTRL_SYNC :		/* Wait for end of internal write process of the drive */
	//if (select()) 
	res = RES_OK;
	break;
  case GET_SECTOR_SIZE :	
     *(DWORD*)buff = 4096;
	 res = RES_OK;
  case GET_SECTOR_COUNT :	/* Get drive capacity in unit of sector (DWORD) */
     *(DWORD*)buff = (16 * 1024*1024 ) /4096;
	 res = RES_OK;
	break;
  case GET_BLOCK_SIZE :	/* Get erase block size in unit of sector (DWORD) */
    *(DWORD*)buff = 1 ;//(16 * 1024*1024 ) /4096;
    res = RES_OK;
	break;

  case CTRL_ERASE_SECTOR :	/* Erase a block of sectors (used when _USE_ERASE == 1) */
	*(DWORD*)buff = 4096;
	res = RES_OK;	/* FatFs does not check result of this command */
	 break;
  default:
	res = RES_PARERR;
 }
 return res;	
}

DRESULT MMC_disk_ioctl(BYTE cmd, void *buff)
{
 return RES_OK;
}

DRESULT USB_disk_ioctl(BYTE cmd, void *buff)
{
 return RES_OK;
}

DWORD get_fattime (void)
{return 0;}


#endif
