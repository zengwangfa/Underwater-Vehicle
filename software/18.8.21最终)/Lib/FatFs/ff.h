/*---------------------------------------------------------------------------/
/  FatFs - FAT file system module include file  R0.09     (C)ChaN, 2011
/----------------------------------------------------------------------------/
/ FatFs module is a generic FAT file system module for small embedded systems.
/ This is a free software that opened for education, research and commercial
/ developments under license policy of following trems.
/
/  Copyright (C) 2011, ChaN, all right reserved.
/
/ * The FatFs module is a free software and there is NO WARRANTY.
/ * No restriction on use. You can use, modify and redistribute it for
/   personal, non-profit or commercial product UNDER YOUR RESPONSIBILITY.
/ * Redistributions of source code must retain the above copyright notice.
/
/----------------------------------------------------------------------------*/

#ifndef _FATFS
#define _FATFS  6502    /* Revision ID */

#ifdef __cplusplus
extern "C" {
#endif

#include "integer.h"    /* Basic integer types */
#include "ffconf.h"     /* FatFs configuration options */

#if _FATFS != _FFCONF
#error Wrong configuration file (ffconf.h).
#endif



    /* Definitions of volume management */

#if _MULTI_PARTITION        /* Multiple partition configuration */
    typedef struct
    {
        uint8 pd;   /* Physical drive number */
        uint8 pt;   /* Partition: 0:Auto detect, 1-4:Forced partition) */
    } PARTITION;
    extern PARTITION VolToPart[];   /* Volume - Partition resolution table */
#define LD2PD(vol) (VolToPart[vol].pd)  /* Get physical drive number */
#define LD2PT(vol) (VolToPart[vol].pt)  /* Get partition index */

#else                       /* Single partition configuration */
#define LD2PD(vol) (vol)    /* Each logical drive is bound to the same physical drive number */
#define LD2PT(vol) 0        /* Always mounts the 1st partition or in SFD */

#endif



    /* Type of path name strings on FatFs API */

#if _LFN_UNICODE            /* Unicode string */
#if !_USE_LFN
#error _LFN_UNICODE must be 0 in non-LFN cfg.
#endif
#ifndef _INC_TCHAR
    typedef uint16 TCHAR;
#define _T(x) L ## x
#define _TEXT(x) L ## x
#endif

#else                       /* ANSI/OEM string */
#ifndef _INC_TCHAR
    typedef char TCHAR;
#define _T(x) x
#define _TEXT(x) x
#endif

#endif




    /* File system object structure (FATFS) */
    typedef struct
    {
        uint8   fs_type;        /* FAT子类型，一般在mount时用，置0表示未挂载*/
        uint8   drv;            /* 物理驱动号，一般为0*/
        uint8   csize;          /* 每个簇的扇区数目(1,2,4...128) */
        uint8   n_fats;         /* 文件分配表的数目(1,2) */
        /*FAT文件系统依次为：引导扇区、两个文件分配表、根目录区和数据区*/
        uint8   wflag;          /* 标记文件是否被改动过，为1时要回写*/
        uint8   fsi_flag;       /* 标记文件系统信息是否被改动过，为1时要回写*/
        uint16  id;             /* 文件系统挂载ID */
        uint16  n_rootdir;      /* 根目录区入口(目录项)的个数(用于FAT12/16)*/
#if _MAX_SS != 512
        uint16  ssize;          /* 每扇区的字节数(用于扇区大于512Byte的flash) */
#endif
#if _FS_REENTRANT
        _SYNC_t sobj;           /* 允许重入，即定义同步对象，用在tiny中*/
#endif
#if !_FS_READONLY
        uint32  last_clust;     /* 最后一个被分配的簇*/
        uint32  free_clust;     /* 空闲簇的个数*/
        uint32  fsi_sector;     /* 存放fsinfo的扇区(用于FAT32) */
#endif
#if _FS_RPATH
        uint32  cdir;           /* 允许相对路径时用，存储当前目录起始簇(0:root)*/
#endif
        uint32  n_fatent;       /* FAT入口数(簇的数目 + 2)*/
        uint32  fsize;          /* 每个FAT所占扇区*/
        uint32  fatbase;        /* FAT起始扇区*/
        uint32  dirbase;        /* 根目录起始扇区(FAT32:Cluster#) */
        uint32  database;       /* 数据目录起始扇区*/
        uint32  winsect;        /* 当前缓冲区中存储的扇区号*/
        uint8   win[_MAX_SS];   /* 单个扇区缓存*/
    } FATFS;




    /* File object structure (FIL) */

    typedef struct
    {
        FATFS   *fs;                /* 所在的fs指针*/
        uint16  id;             /* 所在的fs挂载编号*/
        uint8   flag;           /* 文件状态*/
        uint8   pad1;           /* 不知道含义，也未见程序使用*/
        uint32  fptr;           /* 文件读写指针*/
        uint32  fsize;          /* 大小*/
        uint32  sclust;         /* 文件起始簇(fsize=0时为0) */
        uint32  clust;          /* 当前簇*/
        uint32  dsect;          /* 当前数据扇区*/
#if !_FS_READONLY
        uint32  dir_sect;       /* 包含目录项的扇区 */
        uint8   *dir_ptr;       /* Ponter to the directory entry in the window */
#endif
#if _USE_FASTSEEK
        uint32  *cltbl;         /*指向簇链接映射表的指针*/
#endif
#if _FS_SHARE
        uint32  lockid;         /* File lock ID (index of file semaphore table) */
#endif
#if !_FS_TINY
        uint8   buf[_MAX_SS];   /* File data read/write buffer */
#endif
    } FIL;



    /* Directory object structure (DIR) */

    typedef struct
    {
        FATFS   *fs;                /* 同上*/
        uint16  id;
        uint16  index;          /* 当前读写索引号 */
        uint32  sclust;         /* 文件数据区开始簇*/
        uint32  clust;          /* 当前簇*/
        uint32  sect;           /* 当前扇区*/
        uint8   *dir;           /* 扇区缓存中当前SFN入口指针，SFN含义未知，猜测和LFN类似，与文件名相关*/
        uint8   *fn;                /* Pointer to the SFN (in/out) {file[8],ext[3],status[1]} */
#if _USE_LFN
        uint16  *lfn;           /* Pointer to the LFN working buffer */
        uint16  lfn_idx;        /* Last matched LFN index number (0xFFFF:No LFN) */
#endif
    } DIR;




    /* File status structure (FILINFO) */

    typedef struct
    {
        uint32  fsize;          /* File size */
        uint16  fdate;          /* Last modified date */
        uint16  ftime;          /* Last modified time */
        uint8   fattrib;        /* Attribute */
        TCHAR   fname[13];      /* Short file name (8.3 format) */
#if _USE_LFN
        TCHAR   *lfname;            /* Pointer to the LFN buffer */
        uint32  lfsize;         /* Size of LFN buffer in TCHAR */
#endif
    } FILINFO;



    /* File function return code (FRESULT) */

    typedef enum
    {
        FR_OK = 0,              /* (0) Succeeded */
        FR_DISK_ERR,            /* (1) A hard error occured in the low level disk I/O layer */
        FR_INT_ERR,             /* (2) Assertion failed */
        FR_NOT_READY,           /* (3) The physical drive cannot work */
        FR_NO_FILE,             /* (4) Could not find the file */
        FR_NO_PATH,             /* (5) Could not find the path */
        FR_INVALID_NAME,        /* (6) The path name format is invalid */
        FR_DENIED,              /* (7) Acces denied due to prohibited access or directory full */
        FR_EXIST,               /* (8) Acces denied due to prohibited access */
        FR_INVALID_OBJECT,      /* (9) The file/directory object is invalid */
        FR_WRITE_PROTECTED,     /* (10) The physical drive is write protected */
        FR_INVALID_DRIVE,       /* (11) The logical drive number is invalid */
        FR_NOT_ENABLED,         /* (12) The volume has no work area */
        FR_NO_FILESYSTEM,       /* (13) There is no valid FAT volume */
        FR_MKFS_ABORTED,        /* (14) The f_mkfs() aborted due to any parameter error */
        FR_TIMEOUT,             /* (15) Could not get a grant to access the volume within defined period */
        FR_LOCKED,              /* (16) The operation is rejected according to the file shareing policy */
        FR_NOT_ENOUGH_CORE,     /* (17) LFN working buffer could not be allocated */
        FR_TOO_MANY_OPEN_FILES, /* (18) Number of open files > _FS_SHARE */
        FR_INVALID_PARAMETER    /* (19) Given parameter is invalid */
    } FRESULT;



    /*--------------------------------------------------------------*/
    /* FatFs module application interface                           */

    FRESULT f_mount (uint8, FATFS *);                       /* Mount/Unmount a logical drive */
    FRESULT f_open (FIL *, const TCHAR *, uint8);           /* Open or create a file */
    FRESULT f_read (FIL *, void *, uint32, uint32 *);           /* Read data from a file */
    FRESULT f_lseek (FIL *, uint32);                        /* Move file pointer of a file object */
    FRESULT f_close (FIL *);                                /* Close an open file object */
    FRESULT f_opendir (DIR *, const TCHAR *);               /* Open an existing directory */
    FRESULT f_readdir (DIR *, FILINFO *);                   /* Read a directory item */
    FRESULT f_stat (const TCHAR *, FILINFO *);          /* Get file status */
    FRESULT f_write (FIL *, const void *, uint32, uint32 *);    /* Write data to a file */
    FRESULT f_getfree (const TCHAR *, uint32 *, FATFS **);  /* Get number of free clusters on the drive */
    FRESULT f_truncate (FIL *);                         /* Truncate file */
    FRESULT f_sync (FIL *);                             /* Flush cached data of a writing file */
    FRESULT f_unlink (const TCHAR *);                   /* Delete an existing file or directory */
    FRESULT f_mkdir (const TCHAR *);                        /* Create a new directory */
    FRESULT f_chmod (const TCHAR *, uint8, uint8);          /* Change attriburte of the file/dir */
    FRESULT f_utime (const TCHAR *, const FILINFO *);       /* Change timestamp of the file/dir */
    FRESULT f_rename (const TCHAR *, const TCHAR *);        /* Rename/Move a file or directory */
    FRESULT f_chdrive (uint8);                          /* Change current drive */
    FRESULT f_chdir (const TCHAR *);                        /* Change current directory */
    FRESULT f_getcwd (TCHAR *, uint32);                 /* Get current directory */
    FRESULT f_forward (FIL *, uint32( *)(const uint8 *, uint32), uint32, uint32 *); /* Forward data to the stream */
    FRESULT f_mkfs (uint8, uint8, uint32);                  /* Create a file system on the drive */
    FRESULT f_fdisk (uint8, const uint32[], void *);        /* Divide a physical drive into some partitions */
    int f_putc (TCHAR, FIL *);                          /* Put a character to the file */
    int f_puts (const TCHAR *, FIL *);                  /* Put a string to the file */
    int f_printf (FIL *, const TCHAR *, ...);               /* Put a formatted string to the file */
    TCHAR *f_gets (TCHAR *, int, FIL *);                    /* Get a string from the file */

#define f_eof(fp) (((fp)->fptr == (fp)->fsize) ? 1 : 0)
#define f_error(fp) (((fp)->flag & FA__ERROR) ? 1 : 0)
#define f_tell(fp) ((fp)->fptr)
#define f_size(fp) ((fp)->fsize)

#ifndef EOF
#define EOF (-1)
#endif




    /*--------------------------------------------------------------*/
    /* Additional user defined functions                            */

    /* RTC function */
#if !_FS_READONLY
    uint32 get_fattime (void);
#endif

    /* Unicode support functions */
#if _USE_LFN                        /* Unicode - OEM code conversion */
    uint16 ff_convert (uint16, uint32);     /* OEM-Unicode bidirectional conversion */
    uint16 ff_wtoupper (uint16);            /* Unicode upper-case conversion */
#if _USE_LFN == 3                   /* Memory functions */
    void *ff_memalloc (uint32);         /* Allocate memory block */
    void ff_memfree (void *);           /* Free memory block */
#endif
#endif

    /* Sync functions */
#if _FS_REENTRANT
    int ff_cre_syncobj (uint8, _SYNC_t *); /* Create a sync object */
    int ff_req_grant (_SYNC_t);         /* Lock sync object */
    void ff_rel_grant (_SYNC_t);        /* Unlock sync object */
    int ff_del_syncobj (_SYNC_t);       /* Delete a sync object */
#endif




    /*--------------------------------------------------------------*/
    /* Flags and offset address                                     */


    /* File access control and file status flags (FIL.flag) */

#define FA_READ             0x01
#define FA_OPEN_EXISTING    0x00
#define FA__ERROR           0x80

#if !_FS_READONLY
#define FA_WRITE            0x02
#define FA_CREATE_NEW       0x04
#define FA_CREATE_ALWAYS    0x08
#define FA_OPEN_ALWAYS      0x10
#define FA__WRITTEN         0x20
#define FA__DIRTY           0x40
#endif


    /* FAT sub type (FATFS.fs_type) */

#define FS_FAT12    1
#define FS_FAT16    2
#define FS_FAT32    3


    /* File attribute bits for directory entry */

#define AM_RDO  0x01    /* Read only */
#define AM_HID  0x02    /* Hidden */
#define AM_SYS  0x04    /* System */
#define AM_VOL  0x08    /* Volume label */
#define AM_LFN  0x0F    /* LFN entry */
#define AM_DIR  0x10    /* Directory */
#define AM_ARC  0x20    /* Archive */
#define AM_MASK 0x3F    /* Mask of defined bits */


    /* Fast seek feature */
#define CREATE_LINKMAP  0xFFFFFFFF



    /*--------------------------------*/
    /* Multi-byte word access macros  */

#if _WORD_ACCESS == 1   /* Enable word access to the FAT structure */
#define LD_WORD(ptr)        (uint16)(*(uint16*)(uint8*)(ptr))
#define LD_DWORD(ptr)       (uint32)(*(uint32*)(uint8*)(ptr))
#define ST_WORD(ptr,val)    *(uint16*)(uint8*)(ptr)=(uint16)(val)
#define ST_DWORD(ptr,val)   *(uint32*)(uint8*)(ptr)=(uint32)(val)
#else                   /* Use byte-by-byte access to the FAT structure */
#define LD_WORD(ptr)        (uint16)(((uint16)*((uint8*)(ptr)+1)<<8)|(uint16)*(uint8*)(ptr))
#define LD_DWORD(ptr)       (uint32)(((uint32)*((uint8*)(ptr)+3)<<24)|((uint32)*((uint8*)(ptr)+2)<<16)|((uint16)*((uint8*)(ptr)+1)<<8)|*(uint8*)(ptr))
#define ST_WORD(ptr,val)    *(uint8*)(ptr)=(uint8)(val); *((uint8*)(ptr)+1)=(uint8)((uint16)(val)>>8)
#define ST_DWORD(ptr,val)   *(uint8*)(ptr)=(uint8)(val); *((uint8*)(ptr)+1)=(uint8)((uint16)(val)>>8); *((uint8*)(ptr)+2)=(uint8)((uint32)(val)>>16); *((uint8*)(ptr)+3)=(uint8)((uint32)(val)>>24)
#endif

#ifdef __cplusplus
}
#endif

#endif /* _FATFS */
