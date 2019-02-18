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
 * @file       diskio.c
 * @brief      FATFS 底层接口实现函数
 * @author     山外科技 & gustavo(brtos作者)
 * @version    v5.0
 * @date       2013-09-19
 */

#include "common.h"
#include "MK60_sdhc.h"
#include "diskio.h"


static volatile DSTATUS   Stat             = STA_NOINIT;    /* 硬盘状态         */

/*!
 *  @brief      硬盘初始化
 *  @param      drv                 设备号(目前代码仅支持为 0)
 *  @since      v5.0
 */
DSTATUS disk_initialize (unsigned char drv)
{
    uint32                      param, c_size, c_size_mult, read_bl_len;
    ESDHC_CMD_t                 command;

    if (drv) return STA_NOINIT;         /* 目前代码仅支持1个设备 */
    if (Stat & STA_NODISK) return Stat; /* 没有插入卡 */
    if ((Stat & STA_NOINIT) == 0)   return 0;/* 没有初始化 */

    SDHC_card.SD_TIMEOUT = 0;
    SDHC_card.NUM_BLOCKS = 0;
    SDHC_card.ADDRESS = 0;
    SDHC_card.SDHC = FALSE;
    SDHC_card.VERSION2 = FALSE;

    /* 初始化和检测卡 */
    if (ESDHC_IOCTL_OK != SDHC_ioctl (ESDHC_IOCTL_INIT, NULL))
    {
        return FALSE;
    }

    /* SDHC 检测 */
    param = 0;
    if (ESDHC_IOCTL_OK != SDHC_ioctl (ESDHC_IOCTL_GET_CARD, &param))
    {
        return FALSE;
    }
    if ((ESDHC_CARD_SD == param) || (ESDHC_CARD_SDHC == param) || (ESDHC_CARD_SDCOMBO == param) || (ESDHC_CARD_SDHCCOMBO == param))
    {
        if ((ESDHC_CARD_SDHC == param) || (ESDHC_CARD_SDHCCOMBO == param))
        {
            SDHC_card.SDHC = TRUE;
        }
    }
    else
    {
        return FALSE;
    }

    /* 卡识别 */
    command.COMMAND = ESDHC_CMD2;
    command.TYPE = ESDHC_TYPE_NORMAL;
    command.ARGUMENT = 0;
    command.READ = FALSE;
    command.BLOCKS = 0;
    if (ESDHC_IOCTL_OK != SDHC_ioctl (ESDHC_IOCTL_SEND_CMD, &command))
    {
        return FALSE;
    }

    /* 获得卡地址 */
    command.COMMAND = ESDHC_CMD3;
    command.TYPE = ESDHC_TYPE_NORMAL;
    command.ARGUMENT = 0;
    command.READ = FALSE;
    command.BLOCKS = 0;
    if (ESDHC_IOCTL_OK != SDHC_ioctl (ESDHC_IOCTL_SEND_CMD, &command))
    {
        return FALSE;
    }
    SDHC_card.ADDRESS = command.RESPONSE[0] & 0xFFFF0000;

    /* 获得卡参数 */
    command.COMMAND = ESDHC_CMD9;
    command.TYPE = ESDHC_TYPE_NORMAL;
    command.ARGUMENT = SDHC_card.ADDRESS;
    command.READ = FALSE;
    command.BLOCKS = 0;
    if (ESDHC_IOCTL_OK != SDHC_ioctl (ESDHC_IOCTL_SEND_CMD, &command))
    {
        return FALSE;
    }
    if (0 == (command.RESPONSE[3] & 0x00C00000))
    {
        read_bl_len = (command.RESPONSE[2] >> 8) & 0x0F;
        c_size = command.RESPONSE[2] & 0x03;
        c_size = (c_size << 10) | (command.RESPONSE[1] >> 22);
        c_size_mult = (command.RESPONSE[1] >> 7) & 0x07;
        SDHC_card.NUM_BLOCKS = (c_size + 1) * (1 << (c_size_mult + 2)) * (1 << (read_bl_len - 9));
    }
    else
    {
        SDHC_card.VERSION2 = TRUE;
        c_size = (command.RESPONSE[1] >> 8) & 0x003FFFFF;
        SDHC_card.NUM_BLOCKS = (c_size + 1) << 10;
    }

    /* 选择卡 */
    command.COMMAND = ESDHC_CMD7;
    command.TYPE = ESDHC_TYPE_NORMAL;
    command.ARGUMENT = SDHC_card.ADDRESS;
    command.READ = FALSE;
    command.BLOCKS = 0;
    if (ESDHC_IOCTL_OK != SDHC_ioctl (ESDHC_IOCTL_SEND_CMD, &command))
    {
        return FALSE;
    }

    /* 设置块的大小 */
    command.COMMAND = ESDHC_CMD16;
    command.TYPE = ESDHC_TYPE_NORMAL;
    command.ARGUMENT = SDCARD_BLOCK_SIZE;
    command.READ = FALSE;
    command.BLOCKS = 0;
    if (ESDHC_IOCTL_OK != SDHC_ioctl (ESDHC_IOCTL_SEND_CMD, &command))
    {
        return FALSE;
    }

    if (ESDHC_BUS_WIDTH_4BIT == SDHC_BUS_WIDTH)
    {
        /* 应用程序特定的命令 */
        command.COMMAND = ESDHC_CMD55;
        command.TYPE = ESDHC_TYPE_NORMAL;
        command.ARGUMENT = SDHC_card.ADDRESS;
        command.READ = FALSE;
        command.BLOCKS = 0;
        if (ESDHC_IOCTL_OK != SDHC_ioctl (ESDHC_IOCTL_SEND_CMD, &command))
        {
            return FALSE;
        }

        /* 设置总线带宽 == 4 */
        command.COMMAND = ESDHC_ACMD6;
        command.TYPE = ESDHC_TYPE_NORMAL;
        command.ARGUMENT = 2;
        command.READ = FALSE;
        command.BLOCKS = 0;
        if (ESDHC_IOCTL_OK != SDHC_ioctl (ESDHC_IOCTL_SEND_CMD, &command))
        {
            return FALSE;
        }

        param = ESDHC_BUS_WIDTH_4BIT;
        if (ESDHC_IOCTL_OK != SDHC_ioctl (ESDHC_IOCTL_SET_BUS_WIDTH, &param))
        {
            return FALSE;
        }
    }

    Stat &= ~STA_NOINIT;        /* 清 STA_NOINIT */

    return (Stat & 0x03);
}

/*!
 *  @brief      从MMC接收数据包
 *  @param      buff                接收 SDCARD_BLOCK_SIZE 个字节的数据块到数据缓冲区地址
 *  @param      btr                 字节数目(必须是 4 的倍数)
 *  @since      v5.0
 */
static int rcvr_datablock (uint8   *buff, uint32  btr)
{
    uint32  bytes, i, j;
    uint32  *ptr = (uint32 *)buff;


    /* Workaround for random bit polling failures - not suitable for IO cards    */

    while(!(SDHC_IRQSTAT & SDHC_IRQSTAT_BRR_MASK));
    SDHC_IRQSTAT =  SDHC_IRQSTAT_BRR_MASK ;

    /* Read data in 4 byte counts */
    bytes = btr;
    while (bytes)
    {
        i = bytes > SDCARD_BLOCK_SIZE ? SDCARD_BLOCK_SIZE : bytes;
        for (j = (i + 3) >> 2; j != 0; j--)
        {
            if (SDHC_IRQSTAT & (SDHC_IRQSTAT_DEBE_MASK | SDHC_IRQSTAT_DCE_MASK | SDHC_IRQSTAT_DTOE_MASK))
            {
                SDHC_IRQSTAT |= SDHC_IRQSTAT_DEBE_MASK | SDHC_IRQSTAT_DCE_MASK | SDHC_IRQSTAT_DTOE_MASK | SDHC_IRQSTAT_BRR_MASK;
                return 0;
            }

            while (0 == (SDHC_PRSSTAT & SDHC_PRSSTAT_BREN_MASK)) {};

            //小端
            *ptr++ = SDHC_DATPORT;
        }
        bytes -= i;
    }

    return 1;                       /* Return with success */
}

/*!
 *  @brief      读扇区
 *  @param      drv                 驱动号(目前代码仅支持为 0)
 *  @param      buff                缓冲区地址
 *  @param      sector              扇区号
 *  @param      count               扇区数(1~255)
 *  @return     DRESULT             执行结果
 *  @since      v5.0
 */
DRESULT disk_read (         //读磁盘扇区
    uint8  drv,             /* 物理驱动编号 (0) */
    uint8  *buff,           /* 指向数据缓冲区来存储读到的数据 */
    uint32 sector,          /* 开始的扇区号 (LBA) */
    uint8  count            /* 扇区总数(1..255) */
)
{
    ESDHC_CMD_t command;

    if (drv || (!count)) return RES_PARERR;       //drv 只能为 0， count 必须不等于0
    if (Stat & STA_NOINIT) return RES_NOTRDY;     //未就绪

    /* 检测参数 */
    if ((NULL == buff))
    {
        return RES_PARERR;//参数无效
    }

    if (!SDHC_card.SDHC)
    {
        sector *= SDCARD_BLOCK_SIZE;    /* 如果需要,转换为字节地址 */
    }

    if (count == 1) /* 单块读 */
    {
        command.COMMAND = ESDHC_CMD17;
        command.TYPE = ESDHC_TYPE_NORMAL;
        command.ARGUMENT = sector;
        command.READ = TRUE;
        command.BLOCKS = count;

        if (ESDHC_IOCTL_OK == SDHC_ioctl (ESDHC_IOCTL_SEND_CMD, &command))
        {
            if (rcvr_datablock(buff, SDCARD_BLOCK_SIZE))
            {
                count = 0;
            }
        }
    }
    else
    {
        /* 多块读 */
        //
        command.COMMAND = ESDHC_CMD18;
        //command.COMMAND = ESDHC_CMD17;
        command.TYPE = ESDHC_TYPE_NORMAL;
        command.ARGUMENT = sector;
        command.READ = TRUE;
        command.BLOCKS = count;

        if (ESDHC_IOCTL_OK == SDHC_ioctl (ESDHC_IOCTL_SEND_CMD, &command))
        {
            if (rcvr_datablock(buff, SDCARD_BLOCK_SIZE * count))
            {
                count = 0;
            }
        }
    }

    return count ? RES_ERROR : RES_OK;
}

/*!
 *  @brief      发送数据包到 MMC
 *  @param      buff                发送 SDCARD_BLOCK_SIZE 个字节的数据块的数据缓冲区地址
 *  @param      btr                 字节数目(必须是 4 的倍数)
 *  @since      v5.0
 */
static int xmit_datablock (const uint8 *buff, uint32 btr    )
{
    uint32  bytes, i;
    uint32  *ptr = (uint32 *)buff;

    /* Write data in 4 byte counts */
    bytes = btr;
    while (bytes)
    {
        i = bytes > SDCARD_BLOCK_SIZE ? SDCARD_BLOCK_SIZE : bytes;
        bytes -= i;
        for (i = (i + 3) >> 2; i != 0; i--)
        {
            if (SDHC_IRQSTAT & (SDHC_IRQSTAT_DEBE_MASK | SDHC_IRQSTAT_DCE_MASK | SDHC_IRQSTAT_DTOE_MASK))
            {
                SDHC_IRQSTAT |= SDHC_IRQSTAT_DEBE_MASK | SDHC_IRQSTAT_DCE_MASK | SDHC_IRQSTAT_DTOE_MASK | SDHC_IRQSTAT_BWR_MASK;
                return IO_ERROR;
            }
            while (0 == (SDHC_PRSSTAT & SDHC_PRSSTAT_BWEN_MASK)) {};

            SDHC_DATPORT = *ptr++;
        }
    }

    return 1;
}



/*!
 *  @brief      写扇区
 *  @param      drv                 驱动号(目前代码仅支持为 0)
 *  @param      buff                缓冲区地址
 *  @param      sector              扇区号
 *  @param      count               扇区数(1~255)
 *  @return     DRESULT             执行结果
 *  @since      v5.0
 */
DRESULT disk_write (uint8  drv, const uint8  *buff, uint32 sector, uint8  count)
{
    ESDHC_CMD_t command;
    //pSDCARD_t    sdcard_ptr = (pSDCARD_t)&SDHC_card;

    if (drv || !count) return RES_PARERR;
    if (Stat & STA_NOINIT) return RES_NOTRDY;
    if (Stat & STA_PROTECT) return RES_WRPRT;

    /* Check parameters */
    if ((NULL == buff))
    {
        return RES_PARERR;      // 参数无效
    }

    if (!SDHC_card.SDHC)
    {
        sector *= SDCARD_BLOCK_SIZE;    /* Convert to byte address if needed */
    }

    if (count == 1) /* Single block write */
    {
        command.COMMAND = ESDHC_CMD24;
        command.TYPE = ESDHC_TYPE_NORMAL;
        command.ARGUMENT = sector;
        command.READ = FALSE;
        command.BLOCKS = count;

        if (ESDHC_IOCTL_OK == SDHC_ioctl (ESDHC_IOCTL_SEND_CMD, &command))
        {
            if (xmit_datablock(buff, SDCARD_BLOCK_SIZE))
            {
                count = 0;
            }
        }
    }
    else
    {
        command.COMMAND = ESDHC_CMD25;
        command.TYPE = ESDHC_TYPE_NORMAL;
        command.ARGUMENT = sector;
        command.READ = FALSE;
        command.BLOCKS = count;

        if (ESDHC_IOCTL_OK == SDHC_ioctl (ESDHC_IOCTL_SEND_CMD, &command))
        {
            if (xmit_datablock(buff, SDCARD_BLOCK_SIZE * count))
            {
                count = 0;
            }
            while((SDHC_IRQSTAT & SDHC_IRQSTAT_TC_MASK)==0);

            if (SDHC_IRQSTAT & (SDHC_IRQSTAT_DEBE_MASK | SDHC_IRQSTAT_DCE_MASK | SDHC_IRQSTAT_DTOE_MASK))
            {
                SDHC_IRQSTAT |= SDHC_IRQSTAT_DEBE_MASK | SDHC_IRQSTAT_DCE_MASK | SDHC_IRQSTAT_DTOE_MASK;
            }
            SDHC_IRQSTAT |= SDHC_IRQSTAT_TC_MASK | SDHC_IRQSTAT_BRR_MASK | SDHC_IRQSTAT_BWR_MASK;


        }
    }

    /* Wait for card ready / transaction state */
    do
    {
        command.COMMAND = ESDHC_CMD13;
        command.TYPE = ESDHC_TYPE_NORMAL;
        command.ARGUMENT = SDHC_card.ADDRESS;
        command.READ = FALSE;
        command.BLOCKS = 0;
        if (ESDHC_IOCTL_OK != SDHC_ioctl (ESDHC_IOCTL_SEND_CMD, &command))
        {
            return RES_ERROR;
        }

        /* Card status error check */
        if (command.RESPONSE[0] & 0xFFD98008)
        {
            return RES_ERROR;
        }
    }
    while (0x000000900 != (command.RESPONSE[0] & 0x00001F00));

    return count ? RES_ERROR : RES_OK;
}


/*!
 *  @brief      硬盘 控制命令
 *  @param      drv                 驱动号
 *  @param      ctrl                控制命令
 *  @param      buff                缓冲区地址，用于接收和发送控制数据
 *  @since      v5.0
 */
DRESULT disk_ioctl (
    uint8 drv,      /* Physical drive nmuber (0) */
    uint8 ctrl,     /* Control code */
    void  *buff     /* Buffer to send/receive control data */
)
{
    DRESULT              res;
    ESDHC_CMD_t command;
    //pSDCARD_t  sdcard_ptr = (pSDCARD_t)&SDHC_card;

    if (drv) return RES_PARERR;

    res = RES_ERROR;

    if (Stat & STA_NOINIT) return RES_NOTRDY;

    switch (ctrl)
    {
    case CTRL_SYNC :        /* Make sure that no pending write process. Do not remove this or written sector might not left updated. */
        res = RES_OK;
        break;

    case GET_SECTOR_COUNT : /* Get number of sectors on the disk (uint32) */
        *(unsigned long *)buff = SDHC_card.NUM_BLOCKS;
        res = RES_OK;
        break;

    case GET_SECTOR_SIZE :  /* Get R/W sector size (WORD) */
        *(unsigned short *)buff = SDCARD_BLOCK_SIZE;
        res = RES_OK;
        break;

    case GET_BLOCK_SIZE :           /* Get erase block size in unit of sector (uint32) */
        // Implementar
        command.COMMAND = ESDHC_CMD9;
        command.TYPE = ESDHC_TYPE_NORMAL;
        command.ARGUMENT = SDHC_card.ADDRESS;
        command.READ = FALSE;
        command.BLOCKS = 0;
        if (ESDHC_IOCTL_OK != SDHC_ioctl (ESDHC_IOCTL_SEND_CMD, &command))
        {
            return RES_ERROR;
        }
        if (0 == (command.RESPONSE[3] & 0x00C00000))
        {
            //SD V1
            *(unsigned long *)buff = ((((command.RESPONSE[2] >> 18) & 0x7F) + 1) << (((command.RESPONSE[3] >> 8) & 0x03) - 1));
        }
        else
        {
            //SD V2
            // Implementar
            //*(uint32*)buff = (((command.RESPONSE[2] >> 18) & 0x7F) << (((command.RESPONSE[3] >> 8) & 0x03) - 1));
        }
        res = RES_OK;
        break;

    default:
        res = RES_PARERR;
    }

    return res;
}


/*!
 *  @brief      获取硬盘状态
 *  @return     硬盘状态
 *  @since      v5.0
 */
DSTATUS disk_status (
    uint8 drv       /* Physical drive nmuber (0) */
)
{
    if (drv) return STA_NOINIT;     /* Supports only single drive */
    return Stat;
}


/*!
 *  @brief      获取时间(为了满足接口需要而添加，实际上并没用实现功能)
 *  @return     结果总是为 0
 *  @since      v5.0
 */
uint32  get_fattime (void)
{
    return   0;
}


