/*!
 *     COPYRIGHT NOTICE
 *     Copyright (c) 2013,山外科技
 *     All rights reserved.
 *     技术讨论：山外初学论坛 http://www.vcan123.com
 *
 *     除注明出处外，以下所有内容版权均属山外科技所有，未经允许，不得用于商业用途，
 *     修改内容时必须保留山外科技的版权声明。
 *
 * @file       MK60_gpio_cfg.h
 * @brief      gpio的端口定义头文件
 * @author     山外
 * @version    v5.1
 * @date       2014-04-25
 */
#ifndef __MK60_GPIO_CFG_H__
#define __MK60_GPIO_CFG_H__


#if 1       //寄存器位操作，有两种方法，前者效率更高，因而此处使用第一种
#define PT(X,n,REG)         BITBAND_REG(PT##X##_BASE_PTR->##REG,n)              //位操作
#else
#define PT(X,n,REG)         (((Dtype *)(&(PT##X##_BASE_PTR->##REG)))->b##n)
#endif

#define PT_BYTE(X,n,REG)    (((Dtype *)(&(PT##X##_BASE_PTR->##REG)))->B[n])
#define PT_WORD(X,n,REG)    (((Dtype *)(&(PT##X##_BASE_PTR->##REG)))->W[n])
#define PT_DWORD(X,REG)     (((Dtype *)(&(PT##X##_BASE_PTR->##REG)))->DW)
/* Sample usage:
PT_BYTE(A,0,PDOR)  =   (((Dtype *)(&(PTA_BASE_PTR->PDOR)))->B[0])

## ，一般用于把两个宏参数贴合在一起，
这里把 PT A  _BASE_PTR-> PDOR ->B[0] 串接起来

*/

//1位操作
#define     _PTXn_T(ptxn,type)   (ptxn##_##type)
#define     PTXn_T(ptxn,type)    _PTXn_T(ptxn,type)

//8位操作
#define     _PTX_Bn_T(ptx,n,type)  (ptx##_B##n##_##type)
#define     PTX_Bn_T(ptx,n,type)   _PTX_Bn_T(ptx,n,type)

//16位操作
#define     _PTX_Wn_T(ptx,n,type)  (ptx##_W##n##_##type)
#define     PTX_Wn_T(ptx,n,type)   _PTX_Wn_T(ptx,n,type)

//32位操作
#define     _PTX_T(ptx,type)  (ptx##_##type)
#define     PTX_T(ptx,type)   _PTX_T(ptx,type)


//定义PTA的端口
#define PTA0_OUT            PT(A,0,PDOR)
#define PTA1_OUT            PT(A,1,PDOR)
#define PTA2_OUT            PT(A,2,PDOR)
#define PTA3_OUT            PT(A,3,PDOR)
#define PTA4_OUT            PT(A,4,PDOR)
#define PTA5_OUT            PT(A,5,PDOR)
#define PTA6_OUT            PT(A,6,PDOR)
#define PTA7_OUT            PT(A,7,PDOR)
#define PTA8_OUT            PT(A,8,PDOR)
#define PTA9_OUT            PT(A,9,PDOR)
#define PTA10_OUT           PT(A,10,PDOR)
#define PTA11_OUT           PT(A,11,PDOR)
#define PTA12_OUT           PT(A,12,PDOR)
#define PTA13_OUT           PT(A,13,PDOR)
#define PTA14_OUT           PT(A,14,PDOR)
#define PTA15_OUT           PT(A,15,PDOR)
#define PTA16_OUT           PT(A,16,PDOR)
#define PTA17_OUT           PT(A,17,PDOR)
#define PTA18_OUT           PT(A,18,PDOR)
#define PTA19_OUT           PT(A,19,PDOR)
#define PTA20_OUT           PT(A,20,PDOR)
#define PTA21_OUT           PT(A,21,PDOR)
#define PTA22_OUT           PT(A,22,PDOR)
#define PTA23_OUT           PT(A,23,PDOR)
#define PTA24_OUT           PT(A,24,PDOR)
#define PTA25_OUT           PT(A,25,PDOR)
#define PTA26_OUT           PT(A,26,PDOR)
#define PTA27_OUT           PT(A,27,PDOR)
#define PTA28_OUT           PT(A,28,PDOR)
#define PTA29_OUT           PT(A,29,PDOR)
#define PTA30_OUT           PT(A,30,PDOR)
#define PTA31_OUT           PT(A,31,PDOR)

//定义PTB的端口
#define PTB0_OUT           PT(B,0,PDOR)
#define PTB1_OUT           PT(B,1,PDOR)
#define PTB2_OUT           PT(B,2,PDOR)
#define PTB3_OUT           PT(B,3,PDOR)
#define PTB4_OUT           PT(B,4,PDOR)
#define PTB5_OUT           PT(B,5,PDOR)
#define PTB6_OUT           PT(B,6,PDOR)
#define PTB7_OUT           PT(B,7,PDOR)
#define PTB8_OUT           PT(B,8,PDOR)
#define PTB9_OUT           PT(B,9,PDOR)
#define PTB10_OUT           PT(B,10,PDOR)
#define PTB11_OUT           PT(B,11,PDOR)
#define PTB12_OUT           PT(B,12,PDOR)
#define PTB13_OUT           PT(B,13,PDOR)
#define PTB14_OUT           PT(B,14,PDOR)
#define PTB15_OUT           PT(B,15,PDOR)
#define PTB16_OUT           PT(B,16,PDOR)
#define PTB17_OUT           PT(B,17,PDOR)
#define PTB18_OUT           PT(B,18,PDOR)
#define PTB19_OUT           PT(B,19,PDOR)
#define PTB20_OUT           PT(B,20,PDOR)
#define PTB21_OUT           PT(B,21,PDOR)
#define PTB22_OUT           PT(B,22,PDOR)
#define PTB23_OUT           PT(B,23,PDOR)
#define PTB24_OUT           PT(B,24,PDOR)
#define PTB25_OUT           PT(B,25,PDOR)
#define PTB26_OUT           PT(B,26,PDOR)
#define PTB27_OUT           PT(B,27,PDOR)
#define PTB28_OUT           PT(B,28,PDOR)
#define PTB29_OUT           PT(B,29,PDOR)
#define PTB30_OUT           PT(B,30,PDOR)
#define PTB31_OUT           PT(B,31,PDOR)

//定义PTC的端口
#define PTC0_OUT           PT(C,0,PDOR)
#define PTC1_OUT           PT(C,1,PDOR)
#define PTC2_OUT           PT(C,2,PDOR)
#define PTC3_OUT           PT(C,3,PDOR)
#define PTC4_OUT           PT(C,4,PDOR)
#define PTC5_OUT           PT(C,5,PDOR)
#define PTC6_OUT           PT(C,6,PDOR)
#define PTC7_OUT           PT(C,7,PDOR)
#define PTC8_OUT           PT(C,8,PDOR)
#define PTC9_OUT           PT(C,9,PDOR)
#define PTC10_OUT           PT(C,10,PDOR)
#define PTC11_OUT           PT(C,11,PDOR)
#define PTC12_OUT           PT(C,12,PDOR)
#define PTC13_OUT           PT(C,13,PDOR)
#define PTC14_OUT           PT(C,14,PDOR)
#define PTC15_OUT           PT(C,15,PDOR)
#define PTC16_OUT           PT(C,16,PDOR)
#define PTC17_OUT           PT(C,17,PDOR)
#define PTC18_OUT           PT(C,18,PDOR)
#define PTC19_OUT           PT(C,19,PDOR)
#define PTC20_OUT           PT(C,20,PDOR)
#define PTC21_OUT           PT(C,21,PDOR)
#define PTC22_OUT           PT(C,22,PDOR)
#define PTC23_OUT           PT(C,23,PDOR)
#define PTC24_OUT           PT(C,24,PDOR)
#define PTC25_OUT           PT(C,25,PDOR)
#define PTC26_OUT           PT(C,26,PDOR)
#define PTC27_OUT           PT(C,27,PDOR)
#define PTC28_OUT           PT(C,28,PDOR)
#define PTC29_OUT           PT(C,29,PDOR)
#define PTC30_OUT           PT(C,30,PDOR)
#define PTC31_OUT           PT(C,31,PDOR)

//定义PTD的端口
#define PTD0_OUT           PT(D,0,PDOR)
#define PTD1_OUT           PT(D,1,PDOR)
#define PTD2_OUT           PT(D,2,PDOR)
#define PTD3_OUT           PT(D,3,PDOR)
#define PTD4_OUT           PT(D,4,PDOR)
#define PTD5_OUT           PT(D,5,PDOR)
#define PTD6_OUT           PT(D,6,PDOR)
#define PTD7_OUT           PT(D,7,PDOR)
#define PTD8_OUT           PT(D,8,PDOR)
#define PTD9_OUT           PT(D,9,PDOR)
#define PTD10_OUT           PT(D,10,PDOR)
#define PTD11_OUT           PT(D,11,PDOR)
#define PTD12_OUT           PT(D,12,PDOR)
#define PTD13_OUT           PT(D,13,PDOR)
#define PTD14_OUT           PT(D,14,PDOR)
#define PTD15_OUT           PT(D,15,PDOR)
#define PTD16_OUT           PT(D,16,PDOR)
#define PTD17_OUT           PT(D,17,PDOR)
#define PTD18_OUT           PT(D,18,PDOR)
#define PTD19_OUT           PT(D,19,PDOR)
#define PTD20_OUT           PT(D,20,PDOR)
#define PTD21_OUT           PT(D,21,PDOR)
#define PTD22_OUT           PT(D,22,PDOR)
#define PTD23_OUT           PT(D,23,PDOR)
#define PTD24_OUT           PT(D,24,PDOR)
#define PTD25_OUT           PT(D,25,PDOR)
#define PTD26_OUT           PT(D,26,PDOR)
#define PTD27_OUT           PT(D,27,PDOR)
#define PTD28_OUT           PT(D,28,PDOR)
#define PTD29_OUT           PT(D,29,PDOR)
#define PTD30_OUT           PT(D,30,PDOR)
#define PTD31_OUT           PT(D,31,PDOR)

//定义PTE的端口
#define PTE0_OUT           PT(E,0,PDOR)
#define PTE1_OUT           PT(E,1,PDOR)
#define PTE2_OUT           PT(E,2,PDOR)
#define PTE3_OUT           PT(E,3,PDOR)
#define PTE4_OUT           PT(E,4,PDOR)
#define PTE5_OUT           PT(E,5,PDOR)
#define PTE6_OUT           PT(E,6,PDOR)
#define PTE7_OUT           PT(E,7,PDOR)
#define PTE8_OUT           PT(E,8,PDOR)
#define PTE9_OUT           PT(E,9,PDOR)
#define PTE10_OUT           PT(E,10,PDOR)
#define PTE11_OUT           PT(E,11,PDOR)
#define PTE12_OUT           PT(E,12,PDOR)
#define PTE13_OUT           PT(E,13,PDOR)
#define PTE14_OUT           PT(E,14,PDOR)
#define PTE15_OUT           PT(E,15,PDOR)
#define PTE16_OUT           PT(E,16,PDOR)
#define PTE17_OUT           PT(E,17,PDOR)
#define PTE18_OUT           PT(E,18,PDOR)
#define PTE19_OUT           PT(E,19,PDOR)
#define PTE20_OUT           PT(E,20,PDOR)
#define PTE21_OUT           PT(E,21,PDOR)
#define PTE22_OUT           PT(E,22,PDOR)
#define PTE23_OUT           PT(E,23,PDOR)
#define PTE24_OUT           PT(E,24,PDOR)
#define PTE25_OUT           PT(E,25,PDOR)
#define PTE26_OUT           PT(E,26,PDOR)
#define PTE27_OUT           PT(E,27,PDOR)
#define PTE28_OUT           PT(E,28,PDOR)
#define PTE29_OUT           PT(E,29,PDOR)
#define PTE30_OUT           PT(E,30,PDOR)
#define PTE31_OUT           PT(E,31,PDOR)




//定义PTA的输出输入方向
#define PTA0_DDR            PT(A,0,PDDR)
#define PTA1_DDR            PT(A,1,PDDR)
#define PTA2_DDR            PT(A,2,PDDR)
#define PTA3_DDR            PT(A,3,PDDR)
#define PTA4_DDR            PT(A,4,PDDR)
#define PTA5_DDR            PT(A,5,PDDR)
#define PTA6_DDR            PT(A,6,PDDR)
#define PTA7_DDR            PT(A,7,PDDR)
#define PTA8_DDR            PT(A,8,PDDR)
#define PTA9_DDR            PT(A,9,PDDR)
#define PTA10_DDR           PT(A,10,PDDR)
#define PTA11_DDR           PT(A,11,PDDR)
#define PTA12_DDR           PT(A,12,PDDR)
#define PTA13_DDR           PT(A,13,PDDR)
#define PTA14_DDR           PT(A,14,PDDR)
#define PTA15_DDR           PT(A,15,PDDR)
#define PTA16_DDR           PT(A,16,PDDR)
#define PTA17_DDR           PT(A,17,PDDR)
#define PTA18_DDR           PT(A,18,PDDR)
#define PTA19_DDR           PT(A,19,PDDR)
#define PTA20_DDR           PT(A,20,PDDR)
#define PTA21_DDR           PT(A,21,PDDR)
#define PTA22_DDR           PT(A,22,PDDR)
#define PTA23_DDR           PT(A,23,PDDR)
#define PTA24_DDR           PT(A,24,PDDR)
#define PTA25_DDR           PT(A,25,PDDR)
#define PTA26_DDR           PT(A,26,PDDR)
#define PTA27_DDR           PT(A,27,PDDR)
#define PTA28_DDR           PT(A,28,PDDR)
#define PTA29_DDR           PT(A,29,PDDR)
#define PTA30_DDR           PT(A,30,PDDR)
#define PTA31_DDR           PT(A,31,PDDR)

//定义PTB的输出输入方向
#define PTB0_DDR           PT(B,0,PDDR)
#define PTB1_DDR           PT(B,1,PDDR)
#define PTB2_DDR           PT(B,2,PDDR)
#define PTB3_DDR           PT(B,3,PDDR)
#define PTB4_DDR           PT(B,4,PDDR)
#define PTB5_DDR           PT(B,5,PDDR)
#define PTB6_DDR           PT(B,6,PDDR)
#define PTB7_DDR           PT(B,7,PDDR)
#define PTB8_DDR           PT(B,8,PDDR)
#define PTB9_DDR           PT(B,9,PDDR)
#define PTB10_DDR           PT(B,10,PDDR)
#define PTB11_DDR           PT(B,11,PDDR)
#define PTB12_DDR           PT(B,12,PDDR)
#define PTB13_DDR           PT(B,13,PDDR)
#define PTB14_DDR           PT(B,14,PDDR)
#define PTB15_DDR           PT(B,15,PDDR)
#define PTB16_DDR           PT(B,16,PDDR)
#define PTB17_DDR           PT(B,17,PDDR)
#define PTB18_DDR           PT(B,18,PDDR)
#define PTB19_DDR           PT(B,19,PDDR)
#define PTB20_DDR           PT(B,20,PDDR)
#define PTB21_DDR           PT(B,21,PDDR)
#define PTB22_DDR           PT(B,22,PDDR)
#define PTB23_DDR           PT(B,23,PDDR)
#define PTB24_DDR           PT(B,24,PDDR)
#define PTB25_DDR           PT(B,25,PDDR)
#define PTB26_DDR           PT(B,26,PDDR)
#define PTB27_DDR           PT(B,27,PDDR)
#define PTB28_DDR           PT(B,28,PDDR)
#define PTB29_DDR           PT(B,29,PDDR)
#define PTB30_DDR           PT(B,30,PDDR)
#define PTB31_DDR           PT(B,31,PDDR)

//定义PTC的输出输入方向
#define PTC0_DDR           PT(C,0,PDDR)
#define PTC1_DDR           PT(C,1,PDDR)
#define PTC2_DDR           PT(C,2,PDDR)
#define PTC3_DDR           PT(C,3,PDDR)
#define PTC4_DDR           PT(C,4,PDDR)
#define PTC5_DDR           PT(C,5,PDDR)
#define PTC6_DDR           PT(C,6,PDDR)
#define PTC7_DDR           PT(C,7,PDDR)
#define PTC8_DDR           PT(C,8,PDDR)
#define PTC9_DDR           PT(C,9,PDDR)
#define PTC10_DDR           PT(C,10,PDDR)
#define PTC11_DDR           PT(C,11,PDDR)
#define PTC12_DDR           PT(C,12,PDDR)
#define PTC13_DDR           PT(C,13,PDDR)
#define PTC14_DDR           PT(C,14,PDDR)
#define PTC15_DDR           PT(C,15,PDDR)
#define PTC16_DDR           PT(C,16,PDDR)
#define PTC17_DDR           PT(C,17,PDDR)
#define PTC18_DDR           PT(C,18,PDDR)
#define PTC19_DDR           PT(C,19,PDDR)
#define PTC20_DDR           PT(C,20,PDDR)
#define PTC21_DDR           PT(C,21,PDDR)
#define PTC22_DDR           PT(C,22,PDDR)
#define PTC23_DDR           PT(C,23,PDDR)
#define PTC24_DDR           PT(C,24,PDDR)
#define PTC25_DDR           PT(C,25,PDDR)
#define PTC26_DDR           PT(C,26,PDDR)
#define PTC27_DDR           PT(C,27,PDDR)
#define PTC28_DDR           PT(C,28,PDDR)
#define PTC29_DDR           PT(C,29,PDDR)
#define PTC30_DDR           PT(C,30,PDDR)
#define PTC31_DDR           PT(C,31,PDDR)

//定义PTD的输出输入方向
#define PTD0_DDR           PT(D,0,PDDR)
#define PTD1_DDR           PT(D,1,PDDR)
#define PTD2_DDR           PT(D,2,PDDR)
#define PTD3_DDR           PT(D,3,PDDR)
#define PTD4_DDR           PT(D,4,PDDR)
#define PTD5_DDR           PT(D,5,PDDR)
#define PTD6_DDR           PT(D,6,PDDR)
#define PTD7_DDR           PT(D,7,PDDR)
#define PTD8_DDR           PT(D,8,PDDR)
#define PTD9_DDR           PT(D,9,PDDR)
#define PTD10_DDR           PT(D,10,PDDR)
#define PTD11_DDR           PT(D,11,PDDR)
#define PTD12_DDR           PT(D,12,PDDR)
#define PTD13_DDR           PT(D,13,PDDR)
#define PTD14_DDR           PT(D,14,PDDR)
#define PTD15_DDR           PT(D,15,PDDR)
#define PTD16_DDR           PT(D,16,PDDR)
#define PTD17_DDR           PT(D,17,PDDR)
#define PTD18_DDR           PT(D,18,PDDR)
#define PTD19_DDR           PT(D,19,PDDR)
#define PTD20_DDR           PT(D,20,PDDR)
#define PTD21_DDR           PT(D,21,PDDR)
#define PTD22_DDR           PT(D,22,PDDR)
#define PTD23_DDR           PT(D,23,PDDR)
#define PTD24_DDR           PT(D,24,PDDR)
#define PTD25_DDR           PT(D,25,PDDR)
#define PTD26_DDR           PT(D,26,PDDR)
#define PTD27_DDR           PT(D,27,PDDR)
#define PTD28_DDR           PT(D,28,PDDR)
#define PTD29_DDR           PT(D,29,PDDR)
#define PTD30_DDR           PT(D,30,PDDR)
#define PTD31_DDR           PT(D,31,PDDR)

//定义PTE的输出输入方向
#define PTE0_DDR           PT(E,0,PDDR)
#define PTE1_DDR           PT(E,1,PDDR)
#define PTE2_DDR           PT(E,2,PDDR)
#define PTE3_DDR           PT(E,3,PDDR)
#define PTE4_DDR           PT(E,4,PDDR)
#define PTE5_DDR           PT(E,5,PDDR)
#define PTE6_DDR           PT(E,6,PDDR)
#define PTE7_DDR           PT(E,7,PDDR)
#define PTE8_DDR           PT(E,8,PDDR)
#define PTE9_DDR           PT(E,9,PDDR)
#define PTE10_DDR           PT(E,10,PDDR)
#define PTE11_DDR           PT(E,11,PDDR)
#define PTE12_DDR           PT(E,12,PDDR)
#define PTE13_DDR           PT(E,13,PDDR)
#define PTE14_DDR           PT(E,14,PDDR)
#define PTE15_DDR           PT(E,15,PDDR)
#define PTE16_DDR           PT(E,16,PDDR)
#define PTE17_DDR           PT(E,17,PDDR)
#define PTE18_DDR           PT(E,18,PDDR)
#define PTE19_DDR           PT(E,19,PDDR)
#define PTE20_DDR           PT(E,20,PDDR)
#define PTE21_DDR           PT(E,21,PDDR)
#define PTE22_DDR           PT(E,22,PDDR)
#define PTE23_DDR           PT(E,23,PDDR)
#define PTE24_DDR           PT(E,24,PDDR)
#define PTE25_DDR           PT(E,25,PDDR)
#define PTE26_DDR           PT(E,26,PDDR)
#define PTE27_DDR           PT(E,27,PDDR)
#define PTE28_DDR           PT(E,28,PDDR)
#define PTE29_DDR           PT(E,29,PDDR)
#define PTE30_DDR           PT(E,30,PDDR)
#define PTE31_DDR           PT(E,31,PDDR)



//定义PTA的输入端口
#define PTA0_IN            PT(A,0,PDIR)
#define PTA1_IN            PT(A,1,PDIR)
#define PTA2_IN            PT(A,2,PDIR)
#define PTA3_IN            PT(A,3,PDIR)
#define PTA4_IN            PT(A,4,PDIR)
#define PTA5_IN            PT(A,5,PDIR)
#define PTA6_IN            PT(A,6,PDIR)
#define PTA7_IN            PT(A,7,PDIR)
#define PTA8_IN            PT(A,8,PDIR)
#define PTA9_IN            PT(A,9,PDIR)
#define PTA10_IN           PT(A,10,PDIR)
#define PTA11_IN           PT(A,11,PDIR)
#define PTA12_IN           PT(A,12,PDIR)
#define PTA13_IN           PT(A,13,PDIR)
#define PTA14_IN           PT(A,14,PDIR)
#define PTA15_IN           PT(A,15,PDIR)
#define PTA16_IN           PT(A,16,PDIR)
#define PTA17_IN           PT(A,17,PDIR)
#define PTA18_IN           PT(A,18,PDIR)
#define PTA19_IN           PT(A,19,PDIR)
#define PTA20_IN           PT(A,20,PDIR)
#define PTA21_IN           PT(A,21,PDIR)
#define PTA22_IN           PT(A,22,PDIR)
#define PTA23_IN           PT(A,23,PDIR)
#define PTA24_IN           PT(A,24,PDIR)
#define PTA25_IN           PT(A,25,PDIR)
#define PTA26_IN           PT(A,26,PDIR)
#define PTA27_IN           PT(A,27,PDIR)
#define PTA28_IN           PT(A,28,PDIR)
#define PTA29_IN           PT(A,29,PDIR)
#define PTA30_IN           PT(A,30,PDIR)
#define PTA31_IN           PT(A,31,PDIR)

//定义PTB的输入端口
#define PTB0_IN           PT(B,0,PDIR)
#define PTB1_IN           PT(B,1,PDIR)
#define PTB2_IN           PT(B,2,PDIR)
#define PTB3_IN           PT(B,3,PDIR)
#define PTB4_IN           PT(B,4,PDIR)
#define PTB5_IN           PT(B,5,PDIR)
#define PTB6_IN           PT(B,6,PDIR)
#define PTB7_IN           PT(B,7,PDIR)
#define PTB8_IN           PT(B,8,PDIR)
#define PTB9_IN           PT(B,9,PDIR)
#define PTB10_IN           PT(B,10,PDIR)
#define PTB11_IN           PT(B,11,PDIR)
#define PTB12_IN           PT(B,12,PDIR)
#define PTB13_IN           PT(B,13,PDIR)
#define PTB14_IN           PT(B,14,PDIR)
#define PTB15_IN           PT(B,15,PDIR)
#define PTB16_IN           PT(B,16,PDIR)
#define PTB17_IN           PT(B,17,PDIR)
#define PTB18_IN           PT(B,18,PDIR)
#define PTB19_IN           PT(B,19,PDIR)
#define PTB20_IN           PT(B,20,PDIR)
#define PTB21_IN           PT(B,21,PDIR)
#define PTB22_IN           PT(B,22,PDIR)
#define PTB23_IN           PT(B,23,PDIR)
#define PTB24_IN           PT(B,24,PDIR)
#define PTB25_IN           PT(B,25,PDIR)
#define PTB26_IN           PT(B,26,PDIR)
#define PTB27_IN           PT(B,27,PDIR)
#define PTB28_IN           PT(B,28,PDIR)
#define PTB29_IN           PT(B,29,PDIR)
#define PTB30_IN           PT(B,30,PDIR)
#define PTB31_IN           PT(B,31,PDIR)

//定义PTC的输入端口
#define PTC0_IN           PT(C,0,PDIR)
#define PTC1_IN           PT(C,1,PDIR)
#define PTC2_IN           PT(C,2,PDIR)
#define PTC3_IN           PT(C,3,PDIR)
#define PTC4_IN           PT(C,4,PDIR)
#define PTC5_IN           PT(C,5,PDIR)
#define PTC6_IN           PT(C,6,PDIR)
#define PTC7_IN           PT(C,7,PDIR)
#define PTC8_IN           PT(C,8,PDIR)
#define PTC9_IN           PT(C,9,PDIR)
#define PTC10_IN           PT(C,10,PDIR)
#define PTC11_IN           PT(C,11,PDIR)
#define PTC12_IN           PT(C,12,PDIR)
#define PTC13_IN           PT(C,13,PDIR)
#define PTC14_IN           PT(C,14,PDIR)
#define PTC15_IN           PT(C,15,PDIR)
#define PTC16_IN           PT(C,16,PDIR)
#define PTC17_IN           PT(C,17,PDIR)
#define PTC18_IN           PT(C,18,PDIR)
#define PTC19_IN           PT(C,19,PDIR)
#define PTC20_IN           PT(C,20,PDIR)
#define PTC21_IN           PT(C,21,PDIR)
#define PTC22_IN           PT(C,22,PDIR)
#define PTC23_IN           PT(C,23,PDIR)
#define PTC24_IN           PT(C,24,PDIR)
#define PTC25_IN           PT(C,25,PDIR)
#define PTC26_IN           PT(C,26,PDIR)
#define PTC27_IN           PT(C,27,PDIR)
#define PTC28_IN           PT(C,28,PDIR)
#define PTC29_IN           PT(C,29,PDIR)
#define PTC30_IN           PT(C,30,PDIR)
#define PTC31_IN           PT(C,31,PDIR)

//定义PTD的输入端口
#define PTD0_IN           PT(D,0,PDIR)
#define PTD1_IN           PT(D,1,PDIR)
#define PTD2_IN           PT(D,2,PDIR)
#define PTD3_IN           PT(D,3,PDIR)
#define PTD4_IN           PT(D,4,PDIR)
#define PTD5_IN           PT(D,5,PDIR)
#define PTD6_IN           PT(D,6,PDIR)
#define PTD7_IN           PT(D,7,PDIR)
#define PTD8_IN           PT(D,8,PDIR)
#define PTD9_IN           PT(D,9,PDIR)
#define PTD10_IN           PT(D,10,PDIR)
#define PTD11_IN           PT(D,11,PDIR)
#define PTD12_IN           PT(D,12,PDIR)
#define PTD13_IN           PT(D,13,PDIR)
#define PTD14_IN           PT(D,14,PDIR)
#define PTD15_IN           PT(D,15,PDIR)
#define PTD16_IN           PT(D,16,PDIR)
#define PTD17_IN           PT(D,17,PDIR)
#define PTD18_IN           PT(D,18,PDIR)
#define PTD19_IN           PT(D,19,PDIR)
#define PTD20_IN           PT(D,20,PDIR)
#define PTD21_IN           PT(D,21,PDIR)
#define PTD22_IN           PT(D,22,PDIR)
#define PTD23_IN           PT(D,23,PDIR)
#define PTD24_IN           PT(D,24,PDIR)
#define PTD25_IN           PT(D,25,PDIR)
#define PTD26_IN           PT(D,26,PDIR)
#define PTD27_IN           PT(D,27,PDIR)
#define PTD28_IN           PT(D,28,PDIR)
#define PTD29_IN           PT(D,29,PDIR)
#define PTD30_IN           PT(D,30,PDIR)
#define PTD31_IN           PT(D,31,PDIR)

//定义PTE的输入端口
#define PTE0_IN           PT(E,0,PDIR)
#define PTE1_IN           PT(E,1,PDIR)
#define PTE2_IN           PT(E,2,PDIR)
#define PTE3_IN           PT(E,3,PDIR)
#define PTE4_IN           PT(E,4,PDIR)
#define PTE5_IN           PT(E,5,PDIR)
#define PTE6_IN           PT(E,6,PDIR)
#define PTE7_IN           PT(E,7,PDIR)
#define PTE8_IN           PT(E,8,PDIR)
#define PTE9_IN           PT(E,9,PDIR)
#define PTE10_IN           PT(E,10,PDIR)
#define PTE11_IN           PT(E,11,PDIR)
#define PTE12_IN           PT(E,12,PDIR)
#define PTE13_IN           PT(E,13,PDIR)
#define PTE14_IN           PT(E,14,PDIR)
#define PTE15_IN           PT(E,15,PDIR)
#define PTE16_IN           PT(E,16,PDIR)
#define PTE17_IN           PT(E,17,PDIR)
#define PTE18_IN           PT(E,18,PDIR)
#define PTE19_IN           PT(E,19,PDIR)
#define PTE20_IN           PT(E,20,PDIR)
#define PTE21_IN           PT(E,21,PDIR)
#define PTE22_IN           PT(E,22,PDIR)
#define PTE23_IN           PT(E,23,PDIR)
#define PTE24_IN           PT(E,24,PDIR)
#define PTE25_IN           PT(E,25,PDIR)
#define PTE26_IN           PT(E,26,PDIR)
#define PTE27_IN           PT(E,27,PDIR)
#define PTE28_IN           PT(E,28,PDIR)
#define PTE29_IN           PT(E,29,PDIR)
#define PTE30_IN           PT(E,30,PDIR)
#define PTE31_IN           PT(E,31,PDIR)

//定义PTA的翻转电平输出端口
#define PTA0_T            PT(A,0,PTOR)
#define PTA1_T            PT(A,1,PTOR)
#define PTA2_T            PT(A,2,PTOR)
#define PTA3_T            PT(A,3,PTOR)
#define PTA4_T            PT(A,4,PTOR)
#define PTA5_T            PT(A,5,PTOR)
#define PTA6_T            PT(A,6,PTOR)
#define PTA7_T            PT(A,7,PTOR)
#define PTA8_T            PT(A,8,PTOR)
#define PTA9_T            PT(A,9,PTOR)
#define PTA10_T           PT(A,10,PTOR)
#define PTA11_T           PT(A,11,PTOR)
#define PTA12_T           PT(A,12,PTOR)
#define PTA13_T           PT(A,13,PTOR)
#define PTA14_T           PT(A,14,PTOR)
#define PTA15_T           PT(A,15,PTOR)
#define PTA16_T           PT(A,16,PTOR)
#define PTA17_T           PT(A,17,PTOR)
#define PTA18_T           PT(A,18,PTOR)
#define PTA19_T           PT(A,19,PTOR)
#define PTA20_T           PT(A,20,PTOR)
#define PTA21_T           PT(A,21,PTOR)
#define PTA22_T           PT(A,22,PTOR)
#define PTA23_T           PT(A,23,PTOR)
#define PTA24_T           PT(A,24,PTOR)
#define PTA25_T           PT(A,25,PTOR)
#define PTA26_T           PT(A,26,PTOR)
#define PTA27_T           PT(A,27,PTOR)
#define PTA28_T           PT(A,28,PTOR)
#define PTA29_T           PT(A,29,PTOR)
#define PTA30_T           PT(A,30,PTOR)
#define PTA31_T           PT(A,31,PTOR)

//定义PTB的翻转电平输出端口
#define PTB0_T           PT(B,0,PTOR)
#define PTB1_T           PT(B,1,PTOR)
#define PTB2_T           PT(B,2,PTOR)
#define PTB3_T           PT(B,3,PTOR)
#define PTB4_T           PT(B,4,PTOR)
#define PTB5_T           PT(B,5,PTOR)
#define PTB6_T           PT(B,6,PTOR)
#define PTB7_T           PT(B,7,PTOR)
#define PTB8_T           PT(B,8,PTOR)
#define PTB9_T           PT(B,9,PTOR)
#define PTB10_T           PT(B,10,PTOR)
#define PTB11_T           PT(B,11,PTOR)
#define PTB12_T           PT(B,12,PTOR)
#define PTB13_T           PT(B,13,PTOR)
#define PTB14_T           PT(B,14,PTOR)
#define PTB15_T           PT(B,15,PTOR)
#define PTB16_T           PT(B,16,PTOR)
#define PTB17_T           PT(B,17,PTOR)
#define PTB18_T           PT(B,18,PTOR)
#define PTB19_T           PT(B,19,PTOR)
#define PTB20_T           PT(B,20,PTOR)
#define PTB21_T           PT(B,21,PTOR)
#define PTB22_T           PT(B,22,PTOR)
#define PTB23_T           PT(B,23,PTOR)
#define PTB24_T           PT(B,24,PTOR)
#define PTB25_T           PT(B,25,PTOR)
#define PTB26_T           PT(B,26,PTOR)
#define PTB27_T           PT(B,27,PTOR)
#define PTB28_T           PT(B,28,PTOR)
#define PTB29_T           PT(B,29,PTOR)
#define PTB30_T           PT(B,30,PTOR)
#define PTB31_T           PT(B,31,PTOR)

//定义PTC的翻转电平输出端口
#define PTC0_T           PT(C,0,PTOR)
#define PTC1_T           PT(C,1,PTOR)
#define PTC2_T           PT(C,2,PTOR)
#define PTC3_T           PT(C,3,PTOR)
#define PTC4_T           PT(C,4,PTOR)
#define PTC5_T           PT(C,5,PTOR)
#define PTC6_T           PT(C,6,PTOR)
#define PTC7_T           PT(C,7,PTOR)
#define PTC8_T           PT(C,8,PTOR)
#define PTC9_T           PT(C,9,PTOR)
#define PTC10_T           PT(C,10,PTOR)
#define PTC11_T           PT(C,11,PTOR)
#define PTC12_T           PT(C,12,PTOR)
#define PTC13_T           PT(C,13,PTOR)
#define PTC14_T           PT(C,14,PTOR)
#define PTC15_T           PT(C,15,PTOR)
#define PTC16_T           PT(C,16,PTOR)
#define PTC17_T           PT(C,17,PTOR)
#define PTC18_T           PT(C,18,PTOR)
#define PTC19_T           PT(C,19,PTOR)
#define PTC20_T           PT(C,20,PTOR)
#define PTC21_T           PT(C,21,PTOR)
#define PTC22_T           PT(C,22,PTOR)
#define PTC23_T           PT(C,23,PTOR)
#define PTC24_T           PT(C,24,PTOR)
#define PTC25_T           PT(C,25,PTOR)
#define PTC26_T           PT(C,26,PTOR)
#define PTC27_T           PT(C,27,PTOR)
#define PTC28_T           PT(C,28,PTOR)
#define PTC29_T           PT(C,29,PTOR)
#define PTC30_T           PT(C,30,PTOR)
#define PTC31_T           PT(C,31,PTOR)

//定义PTD的翻转电平输出端口
#define PTD0_T           PT(D,0,PTOR)
#define PTD1_T           PT(D,1,PTOR)
#define PTD2_T           PT(D,2,PTOR)
#define PTD3_T           PT(D,3,PTOR)
#define PTD4_T           PT(D,4,PTOR)
#define PTD5_T           PT(D,5,PTOR)
#define PTD6_T           PT(D,6,PTOR)
#define PTD7_T           PT(D,7,PTOR)
#define PTD8_T           PT(D,8,PTOR)
#define PTD9_T           PT(D,9,PTOR)
#define PTD10_T           PT(D,10,PTOR)
#define PTD11_T           PT(D,11,PTOR)
#define PTD12_T           PT(D,12,PTOR)
#define PTD13_T           PT(D,13,PTOR)
#define PTD14_T           PT(D,14,PTOR)
#define PTD15_T           PT(D,15,PTOR)
#define PTD16_T           PT(D,16,PTOR)
#define PTD17_T           PT(D,17,PTOR)
#define PTD18_T           PT(D,18,PTOR)
#define PTD19_T           PT(D,19,PTOR)
#define PTD20_T           PT(D,20,PTOR)
#define PTD21_T           PT(D,21,PTOR)
#define PTD22_T           PT(D,22,PTOR)
#define PTD23_T           PT(D,23,PTOR)
#define PTD24_T           PT(D,24,PTOR)
#define PTD25_T           PT(D,25,PTOR)
#define PTD26_T           PT(D,26,PTOR)
#define PTD27_T           PT(D,27,PTOR)
#define PTD28_T           PT(D,28,PTOR)
#define PTD29_T           PT(D,29,PTOR)
#define PTD30_T           PT(D,30,PTOR)
#define PTD31_T           PT(D,31,PTOR)

//定义PTE的翻转电平输出端口
#define PTE0_T           PT(E,0,PTOR)
#define PTE1_T           PT(E,1,PTOR)
#define PTE2_T           PT(E,2,PTOR)
#define PTE3_T           PT(E,3,PTOR)
#define PTE4_T           PT(E,4,PTOR)
#define PTE5_T           PT(E,5,PTOR)
#define PTE6_T           PT(E,6,PTOR)
#define PTE7_T           PT(E,7,PTOR)
#define PTE8_T           PT(E,8,PTOR)
#define PTE9_T           PT(E,9,PTOR)
#define PTE10_T           PT(E,10,PTOR)
#define PTE11_T           PT(E,11,PTOR)
#define PTE12_T           PT(E,12,PTOR)
#define PTE13_T           PT(E,13,PTOR)
#define PTE14_T           PT(E,14,PTOR)
#define PTE15_T           PT(E,15,PTOR)
#define PTE16_T           PT(E,16,PTOR)
#define PTE17_T           PT(E,17,PTOR)
#define PTE18_T           PT(E,18,PTOR)
#define PTE19_T           PT(E,19,PTOR)
#define PTE20_T           PT(E,20,PTOR)
#define PTE21_T           PT(E,21,PTOR)
#define PTE22_T           PT(E,22,PTOR)
#define PTE23_T           PT(E,23,PTOR)
#define PTE24_T           PT(E,24,PTOR)
#define PTE25_T           PT(E,25,PTOR)
#define PTE26_T           PT(E,26,PTOR)
#define PTE27_T           PT(E,27,PTOR)
#define PTE28_T           PT(E,28,PTOR)
#define PTE29_T           PT(E,29,PTOR)
#define PTE30_T           PT(E,30,PTOR)
#define PTE31_T           PT(E,31,PTOR)


//定义PTA的8位端口
#define PTA_B0_OUT        PT_BYTE(A,0,PDOR)
#define PTA_B1_OUT        PT_BYTE(A,1,PDOR)
#define PTA_B2_OUT        PT_BYTE(A,2,PDOR)
#define PTA_B3_OUT        PT_BYTE(A,3,PDOR)

//定义PTB的8位端口
#define PTB_B0_OUT        PT_BYTE(B,0,PDOR)
#define PTB_B1_OUT        PT_BYTE(B,1,PDOR)
#define PTB_B2_OUT        PT_BYTE(B,2,PDOR)
#define PTB_B3_OUT        PT_BYTE(B,3,PDOR)


//定义PTC的8位端口
#define PTC_B0_OUT        PT_BYTE(C,0,PDOR)
#define PTC_B1_OUT        PT_BYTE(C,1,PDOR)
#define PTC_B2_OUT        PT_BYTE(C,2,PDOR)
#define PTC_B3_OUT        PT_BYTE(C,3,PDOR)

//定义PTD的8位端口
#define PTD_B0_OUT        PT_BYTE(D,0,PDOR)
#define PTD_B1_OUT        PT_BYTE(D,1,PDOR)
#define PTD_B2_OUT        PT_BYTE(D,2,PDOR)
#define PTD_B3_OUT        PT_BYTE(D,3,PDOR)

//定义PTE的8位端口
#define PTE_B0_OUT        PT_BYTE(E,0,PDOR)
#define PTE_B1_OUT        PT_BYTE(E,1,PDOR)
#define PTE_B2_OUT        PT_BYTE(E,2,PDOR)
#define PTE_B3_OUT        PT_BYTE(E,3,PDOR)



//定义PTA的8位输出输入方向端口
#define PTA_B0_DDR        PT_BYTE(A,0,PDDR)
#define PTA_B1_DDR        PT_BYTE(A,1,PDDR)
#define PTA_B2_DDR        PT_BYTE(A,2,PDDR)
#define PTA_B3_DDR        PT_BYTE(A,3,PDDR)

//定义PTB的8位输出输入方向端口
#define PTB_B0_DDR        PT_BYTE(B,0,PDDR)
#define PTB_B1_DDR        PT_BYTE(B,1,PDDR)
#define PTB_B2_DDR        PT_BYTE(B,2,PDDR)
#define PTB_B3_DDR        PT_BYTE(B,3,PDDR)


//定义PTC的8位输出输入方向端口
#define PTC_B0_DDR        PT_BYTE(C,0,PDDR)
#define PTC_B1_DDR        PT_BYTE(C,1,PDDR)
#define PTC_B2_DDR        PT_BYTE(C,2,PDDR)
#define PTC_B3_DDR        PT_BYTE(C,3,PDDR)

//定义PTD的8位输出输入方向端口
#define PTD_B0_DDR        PT_BYTE(D,0,PDDR)
#define PTD_B1_DDR        PT_BYTE(D,1,PDDR)
#define PTD_B2_DDR        PT_BYTE(D,2,PDDR)
#define PTD_B3_DDR        PT_BYTE(D,3,PDDR)

//定义PTE的8位输出输入方向端口
#define PTE_B0_DDR        PT_BYTE(E,0,PDDR)
#define PTE_B1_DDR        PT_BYTE(E,1,PDDR)
#define PTE_B2_DDR        PT_BYTE(E,2,PDDR)
#define PTE_B3_DDR        PT_BYTE(E,3,PDDR)



//定义PTA的8位输入端口
#define PTA_B0_IN        PT_BYTE(A,0,PDIR)
#define PTA_B1_IN        PT_BYTE(A,1,PDIR)
#define PTA_B2_IN        PT_BYTE(A,2,PDIR)
#define PTA_B3_IN        PT_BYTE(A,3,PDIR)

//定义PTB的8位输入端口
#define PTB_B0_IN        PT_BYTE(B,0,PDIR)
#define PTB_B1_IN        PT_BYTE(B,1,PDIR)
#define PTB_B2_IN        PT_BYTE(B,2,PDIR)
#define PTB_B3_IN        PT_BYTE(B,3,PDIR)


//定义PTC的8位输入端口
#define PTC_B0_IN        PT_BYTE(C,0,PDIR)
#define PTC_B1_IN        PT_BYTE(C,1,PDIR)
#define PTC_B2_IN        PT_BYTE(C,2,PDIR)
#define PTC_B3_IN        PT_BYTE(C,3,PDIR)

//定义PTD的8位输入端口
#define PTD_B0_IN        PT_BYTE(D,0,PDIR)
#define PTD_B1_IN        PT_BYTE(D,1,PDIR)
#define PTD_B2_IN        PT_BYTE(D,2,PDIR)
#define PTD_B3_IN        PT_BYTE(D,3,PDIR)

//定义PTE的8位输入端口
#define PTE_B0_IN        PT_BYTE(E,0,PDIR)
#define PTE_B1_IN        PT_BYTE(E,1,PDIR)
#define PTE_B2_IN        PT_BYTE(E,2,PDIR)
#define PTE_B3_IN        PT_BYTE(E,3,PDIR)


//定义PTA的8位输出反转
#define PTA_B0_T        PT_BYTE(A,0,PTOR)
#define PTA_B1_T        PT_BYTE(A,1,PTOR)
#define PTA_B2_T        PT_BYTE(A,2,PTOR)
#define PTA_B3_T        PT_BYTE(A,3,PTOR)

//定义PTB的8位输出反转
#define PTB_B0_T        PT_BYTE(B,0,PTOR)
#define PTB_B1_T        PT_BYTE(B,1,PTOR)
#define PTB_B2_T        PT_BYTE(B,2,PTOR)
#define PTB_B3_T        PT_BYTE(B,3,PTOR)


//定义PTC的8位输出反转
#define PTC_B0_T        PT_BYTE(C,0,PTOR)
#define PTC_B1_T        PT_BYTE(C,1,PTOR)
#define PTC_B2_T        PT_BYTE(C,2,PTOR)
#define PTC_B3_T        PT_BYTE(C,3,PTOR)

//定义PTD的8位输出反转
#define PTD_B0_T        PT_BYTE(D,0,PTOR)
#define PTD_B1_T        PT_BYTE(D,1,PTOR)
#define PTD_B2_T        PT_BYTE(D,2,PTOR)
#define PTD_B3_T        PT_BYTE(D,3,PTOR)

//定义PTE的8位输出反转
#define PTE_B0_T        PT_BYTE(E,0,PTOR)
#define PTE_B1_T        PT_BYTE(E,1,PTOR)
#define PTE_B2_T        PT_BYTE(E,2,PTOR)
#define PTE_B3_T        PT_BYTE(E,3,PTOR)


//定义PTA的16位端口
#define PTA_W0_OUT        PT_WORD(A,0,PDOR)
#define PTA_W1_OUT        PT_WORD(A,1,PDOR)

//定义PTB的16位端口
#define PTB_W0_OUT        PT_WORD(B,0,PDOR)
#define PTB_W1_OUT        PT_WORD(B,1,PDOR)

//定义PTC的16位端口
#define PTC_W0_OUT        PT_WORD(C,0,PDOR)
#define PTC_W1_OUT        PT_WORD(C,1,PDOR)


//定义PTD的16位端口
#define PTD_W0_OUT        PT_WORD(D,0,PDOR)
#define PTD_W1_OUT        PT_WORD(D,1,PDOR)


//定义PTE的16位端口
#define PTE_W0_OUT        PT_WORD(E,0,PDOR)
#define PTE_W1_OUT        PT_WORD(E,1,PDOR)


//定义PTA的16位输出输入方向端口
#define PTA_W0_DDR        PT_WORD(A,0,PDDR)
#define PTA_W1_DDR        PT_WORD(A,1,PDDR)

//定义PTB的16位输出输入方向端口
#define PTB_W0_DDR        PT_WORD(B,0,PDDR)
#define PTB_W1_DDR        PT_WORD(B,1,PDDR)

//定义PTC的16位输出输入方向端口
#define PTC_W0_DDR        PT_WORD(C,0,PDDR)
#define PTC_W1_DDR        PT_WORD(C,1,PDDR)


//定义PTD的16位输出输入方向端口
#define PTD_W0_DDR        PT_WORD(D,0,PDDR)
#define PTD_W1_DDR        PT_WORD(D,1,PDDR)


//定义PTE的16位输出输入方向端口
#define PTE_W0_DDR        PT_WORD(E,0,PDDR)
#define PTE_W1_DDR        PT_WORD(E,1,PDDR)



//定义PTA的16位输入端口
#define PTA_W0_IN        PT_WORD(A,0,PDIR)
#define PTA_W1_IN        PT_WORD(A,1,PDIR)

//定义PTB的16位输入端口
#define PTB_W0_IN        PT_WORD(B,0,PDIR)
#define PTB_W1_IN        PT_WORD(B,1,PDIR)

//定义PTC的16位输入端口
#define PTC_W0_IN        PT_WORD(C,0,PDIR)
#define PTC_W1_IN        PT_WORD(C,1,PDIR)


//定义PTD的16位输入端口
#define PTD_W0_IN        PT_WORD(D,0,PDIR)
#define PTD_W1_IN        PT_WORD(D,1,PDIR)


//定义PTE的16位输入端口
#define PTE_W0_IN        PT_WORD(E,0,PDIR)
#define PTE_W1_IN        PT_WORD(E,1,PDIR)


//定义PTA的16位输出反转
#define PTA_W0_T        PT_WORD(A,0,PTOR)
#define PTA_W1_T        PT_WORD(A,1,PTOR)

//定义PTB的16位输出反转
#define PTB_W0_T        PT_WORD(B,0,PTOR)
#define PTB_W1_T        PT_WORD(B,1,PTOR)

//定义PTC的16位输出反转
#define PTC_W0_T        PT_WORD(C,0,PTOR)
#define PTC_W1_T        PT_WORD(C,1,PTOR)


//定义PTD的16位输出反转
#define PTD_W0_T        PT_WORD(D,0,PTOR)
#define PTD_W1_T        PT_WORD(D,1,PTOR)


//定义PTE的16位输出反转
#define PTE_W0_T        PT_WORD(E,0,PTOR)
#define PTE_W1_T        PT_WORD(E,1,PTOR)


//定义32位端口输出
#define PTA_OUT          PT_DWORD(A,PDOR)
#define PTB_OUT          PT_DWORD(B,PDOR)
#define PTC_OUT          PT_DWORD(C,PDOR)
#define PTD_OUT          PT_DWORD(D,PDOR)
#define PTE_OUT          PT_DWORD(E,PDOR)

//定义32位端口输入
#define PTA_IN          PT_DWORD(A,PDIR)
#define PTB_IN          PT_DWORD(B,PDIR)
#define PTC_IN          PT_DWORD(C,PDIR)
#define PTD_IN          PT_DWORD(D,PDIR)
#define PTE_IN          PT_DWORD(E,PDIR)

//定义32位端口输出输出方向
#define PTA_DDR          PT_DWORD(A,PDDR)
#define PTB_DDR          PT_DWORD(B,PDDR)
#define PTC_DDR          PT_DWORD(C,PDDR)
#define PTD_DDR          PT_DWORD(D,PDDR)
#define PTE_DDR          PT_DWORD(E,PDDR)

//定义32位输出反转
#define PTA_T          PT_DWORD(A,PTOR)
#define PTB_T          PT_DWORD(B,PTOR)
#define PTC_T          PT_DWORD(C,PTOR)
#define PTD_T          PT_DWORD(D,PTOR)
#define PTE_T          PT_DWORD(E,PTOR)


#endif     //__MK60_GPIO_CFG_H__
