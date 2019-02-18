#ifndef H_ISR_H
#define H_ISR_H 1

#include  "include.h"

void DMA4_IRQHandler();
void PORTA_IRQHandler();
void PIT0_IRQHandler();
void PIT1_IRQHandler();
void PIT2_IRQHandler();

void UART3_IRQHandler();
void HardFault();

#endif 