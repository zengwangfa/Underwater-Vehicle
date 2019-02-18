#ifndef _USB_H_
#define _USB_H_


#include "FslTypes.h"
#include "common.h"
#include "USB_CDC.h"


#define MaxPacketSize   64      //低速USB 为 8 ，全速USB 为 8、16、32、64，高速USB为 64


/* Macros */
#define DISABLE_USB (USBCTL0 = 0x00)    // Disable USB module
#define ENDPOINTS   3

/* EP0 settings */
#define EP0_SIZE            MaxPacketSize

/********** Enpoint Value Options ***********/
/*                                          */
/* IN       Enabled for IN transactions     */
/* OUT      Enabled for OUT transactions    */
/* DISABLE  EndPoint Disabled               */
/*                                          */
/********************************************/

/* EP1 settings */
#define EP1_VALUE           _EP_IN
#define EP1_TYPE            INTERRUPT
#define EP1_SIZE            MaxPacketSize
#define EP1_BUFF_OFFSET     0x18

/* EP2 settings */
#define EP2_VALUE           _EP_IN
#define EP2_TYPE            BULK
#define EP2_SIZE            MaxPacketSize
#define EP2_BUFF_OFFSET     0x20

/* EP3 settings */
#define EP3_VALUE           _EP_OUT
#define EP3_TYPE            BULK
#define EP3_SIZE            MaxPacketSize
#define EP3_BUFF_OFFSET     0x28

/* EP4 settings */
#define EP4_VALUE           DISABLE
#define EP4_SIZE            1
#define EP4_BUFF_OFFSET     0x08

/* EP5 settings */
#define EP5_VALUE           DISABLE
#define EP5_SIZE            1
#define EP5_BUFF_OFFSET     0x08

/* EP6 settings */
#define EP6_VALUE           DISABLE
#define EP6_SIZE            1
#define EP6_BUFF_OFFSET     0x08

/* Macros */
#define EP3_CTR   tBDTtable[4].Stat._byte= kSIE


/* MACROS */
#define usbSIE_CONTROL(EP)   (tBDTtable[EP<<2].Stat._byte= kSIE)
#define usbMCU_CONTROL(EP)   (tBDTtable[EP<<2].Stat._byte= kMCU)
#define usbEP_Reset(EP)      (tBDTtable[EP<<2].Cnt=MaxPacketSize)

#define _EP_IN      USB_ENDPT_EPTXEN_MASK
#define _EP_OUT     USB_ENDPT_EPRXEN_MASK


#define DISABLE     0

#define INTERRUPT   0x03
#define BULK        0x05


// BDT status value
#define kMCU      0x00
#define kSIE      0x80

#define kUDATA0   0x88
#define kUDATA1   0xC8


#define mSETUP_TOKEN    0x0D
#define mOUT_TOKEN      0x01
#define mIN_TOKEN       0x09


// USB commands
#define mGET_STATUS           0
#define mCLR_FEATURE          1
#define mSET_FEATURE          3
#define mSET_ADDRESS          5
#define mGET_DESC             6
#define mSET_DESC             7
#define mGET_CONFIG           8
#define mSET_CONFIG           9
#define mGET_INTF             10
#define mSET_INTF             11
#define mSYNC_FRAME           12
#define mGET_MAXLUN           0xFE      // Mass Storage command

#define mDEVICE                     1
#define mCONFIGURATION              2
#define mSTRING                     3
#define mINTERFACE                  4
#define mENDPOINT                   5
#define mDEVICE_QUALIFIER           6
#define mOTHER_SPEED_CONFIGURATION  7
#define mINTERFACE_POWER            8

/* Request Types */
#define STANDARD_REQ             0x00
#define SPECIFIC_REQ             0x20
#define VENDORSPEC_REQ           0x40
#define DEVICE_REQ               0x00
#define INTERFACE_REQ            0x01
#define ENDPOINT_REQ             0x02

enum
{
    uSETUP,
    uDATA
};

enum
{
    EP0,
    EP1,
    EP2,
    EP3,
    EP4,
    EP5,
    DUMMY,
    LOADER
};


enum
{
    uPOWER,
    uENUMERATED,
    uENABLED,
    uADDRESS,
    uREADY
};

enum
{
    fIN,
    fOUT
};

enum
{
    bEP0OUT_ODD,
    bEP0OUT_EVEN,
    bEP0IN_ODD,
    bEP0IN_EVEN,
    bEP1OUT_ODD,
    bEP1OUT_EVEN,
    bEP1IN_ODD,
    bEP1IN_EVEN,
    bEP2OUT_ODD,
    bEP2OUT_EVEN,
    bEP2IN_ODD,
    bEP2IN_EVEN,
    bEP3OUT_ODD,
    bEP3OUT_EVEN,
    bEP3IN_ODD,
    bEP3IN_EVEN
};


/***** Data Types *****/

typedef struct
{
    uint8 _USB_PERID;
    uint8 _USB_IDCOMP;
    uint8 _USB_REV;
    uint8 _USB_ADDINFO;
    uint8 _USB_OTGISTAT;
    uint8 _USB_OTGICR;
    uint8 _USB_OTGSTAT;
    uint8 _USB_OTGCTL;
    uint8 _USB_ISTAT;
    uint8 _USB_INTEN;
    uint8 _USB_ERRSTAT;
    uint8 _USB_ERREN;
    uint8 _USB_STAT;
    uint8 _USB_CTL;
    uint8 _USB_ADDR;
    uint8 _USB_BDTPAGE1;
    uint8 _USB_FRMNUML;
    uint8 _USB_FRMNUMH;
    uint8 _USB_TOKEN;
    uint8 _USB_SOFTHLD;
    uint8 _USB_BDTPAGE2;
    uint8 _USB_BDTPAGE3;
    uint8 _USB_ENDPT0;
    uint8 _USB_ENDPT1;
    uint8 _USB_ENDPT2;
    uint8 _USB_ENDPT3;
    uint8 _USB_ENDPT4;
    uint8 _USB_ENDPT5;
    uint8 _USB_ENDPT6;
    uint8 _USB_ENDPT7;
    uint8 _USB_ENDPT8;
    uint8 _USB_ENDPT9;
    uint8 _USB_ENDPT10;
    uint8 _USB_ENDPT11;
    uint8 _USB_ENDPT12;
    uint8 _USB_ENDPT13;
    uint8 _USB_ENDPT14;
    uint8 _USB_ENDPT15;
    uint8 _USB_USBCTRL;
    uint8 _USB_OBSERVE;
    uint8 _USB_CONTROL;
    uint8 _USB_USBTRC0;
} USB_Registers;

typedef union _tBDT_STAT
{
    uint8 _byte;
    struct
    {
        uint8 : 1;
        uint8 : 1;
        uint8 BSTALL: 1;             //Buffer Stall Enable
        uint8 DTS: 1;                //Data Toggle Synch Enable
        uint8 NINC: 1;               //Address Increment Disable
        uint8 KEEP: 1;               //BD Keep Enable
        uint8 DATA: 1;               //Data Toggle Synch Value
        uint8 UOWN: 1;               //USB Ownership
    } McuCtlBit;

    struct
    {
        uint8    : 2;
        uint8 PID: 4;                //Packet Identifier
        uint8    : 2;
    } RecPid;
} tBDT_STAT;                         //Buffer Descriptor Status Register

typedef struct _tBDT
{
    tBDT_STAT Stat;
    uint8  dummy;
    uint16 Cnt;
    uint32 Addr;
} tBDT;

typedef struct _tUSB_Setup
{
    uint8 bmRequestType;
    uint8 bRequest;
    uint8 wValue_l;
    uint8 wValue_h;
    uint8 wIndex_l;
    uint8 wIndex_h;
    uint8 wLength_l;
    uint8 wLength_h;
} tUSB_Setup;





/* USB Variables & Flags */
extern uint8        gu8USB_Flags;
extern uint8        gu8USB_State;
extern tUSB_Setup *Setup_Pkt;
extern uint8        gu8EP2_IN_ODD_Buffer[];
extern uint8        gu8EP3_OUT_ODD_Buffer[];
extern uint8        *BufferPointer[];



extern tBDT         tBDTtable[];
extern uint8        gu8Interface;


/* Prototypes */
void    USB_EP0_IN_Handler(void);
void    EP_IN_Transfer(uint8, uint8 *, uint8);
uint8   EP_OUT_Transfer(uint8, uint8 *);
void    USB_Stall_Handler(void);
void    USB_EP0_OUT_Handler(void);
void    USB_EP0_Stall(void);
void    USB_Set_Interface(void);
uint16  USB_EP_OUT_SizeCheck(uint8 u8EP);
void    USB_Reset_Handler(void);
void    USB_Endpoint_Setup_Handler(void);
void    USB_ISR(void);

#endif /* _USB_H_ */
