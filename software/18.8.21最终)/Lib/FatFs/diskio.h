/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,山外科技
 *     All rights reserved.
 *     技术讨论：山外初学论坛 http://www.vcan123.com
 *
 *     除注明出处外，以下所有内容版权均属山外科技所有，未经允许，不得用于商业用途，
 *     修改内容时必须保留山外科技的版权声明。
 *
 *     本驱动由brtos代码提取并修改而成:http://code.google.com/p/brtos/downloads/detail?name=BRTOS%201.66%20Kinetis%20-%20SD%20card%20-%20BMP%2024%20bit.rar&can=2&q=
 *
 * @file       diskio.h
 * @brief      FATFS 底层接口实现函数
 * @author     山外科技 & gustavo(brtos作者)
 * @version    v5.0
 * @date       2013-09-19
 */


#ifndef _DISKIO
#define _DISKIO

#define _READONLY   0   /* 1: Remove write functions */
#define _USE_IOCTL  1   /* 1: Use disk_ioctl fucntion */


#include "common.h"
#include "integer.h"


/* Status of Disk Functions */
typedef uint8   DSTATUS;

/* Results of Disk Functions */
typedef enum
{
    RES_OK = 0,                 /* 0: 成功      */
    RES_ERROR,                  /* 1: R/W 错误  */
    RES_WRPRT,                  /* 2: 写保护    */
    RES_NOTRDY,                 /* 3: 未就绪    */
    RES_PARERR                  /* 4: 参数无效  */
} DRESULT;


/* Disk Status Bits (DSTATUS) */
#define STA_NOINIT      0x01    /* 磁盘未初始化 1表示未初始化，0表示已初始化   */
#define STA_NODISK      0x02    /* 磁盘未插入    */
#define STA_PROTECT     0x04    /* 写保护        */


/* Generic command (defined for FatFs) */
#define CTRL_SYNC           0   /* 冲刷磁盘缓冲 */
#define GET_SECTOR_COUNT    1   /* 获取磁盘大小 */
#define GET_SECTOR_SIZE     2   /* 获取扇区大小 (for multiple sector size (_MAX_SS >= 1024)) */
#define GET_BLOCK_SIZE      3   /* 获取块大小 (for only f_mkfs()) */
#define CTRL_ERASE_SECTOR   4   /* 强制删除一个扇区 (for only _USE_ERASE) */

/* Generic command */
#define CTRL_POWER          5   /* Get/Set power status */
#define CTRL_LOCK           6   /* Lock/Unlock media removal */
#define CTRL_EJECT          7   /* Eject media */

/* MMC/SDC specific ioctl command */
#define MMC_GET_TYPE        10  /* Get card type */
#define MMC_GET_CSD         11  /* Get CSD */
#define MMC_GET_CID         12  /* Get CID */
#define MMC_GET_OCR         13  /* Get OCR */
#define MMC_GET_SDSTAT      14  /* Get SD status */

/* ATA/CF specific ioctl command */
#define ATA_GET_REV         20  /* Get F/W revision */
#define ATA_GET_MODEL       21  /* Get model name */
#define ATA_GET_SN          22  /* Get serial number */

/* NAND specific ioctl command */
#define NAND_FORMAT         30  /* Create physical format */


/*--------------移植时需要编写的几个底层函数--------------*/
extern DSTATUS disk_initialize (uint8);                          //初始化磁盘（只支持磁盘0）
extern DSTATUS disk_status (uint8);                              //返回磁盘状态
extern DRESULT disk_read (uint8, uint8 *, uint32, uint8);           //读磁盘扇区
extern DRESULT disk_ioctl (uint8, uint8, void *);                 //磁盘控制
extern uint32   get_fattime (void);                              //获取当前时间

#if _READONLY == 0
DRESULT disk_write (uint8, const uint8 *, uint32, uint8);           //写磁盘扇区
#endif


#endif
