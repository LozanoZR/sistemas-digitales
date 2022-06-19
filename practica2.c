/* --COPYRIGHT--,BSD
 * Copyright (c) 2017, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * --/COPYRIGHT--*/
/******************************************************************************
 * MSP432 Empty Project
 *
 * Description: An empty project that uses DriverLib
 *
 *                MSP432P401
 *             ------------------
 *         /|\|                  |
 *          | |                  |
 *          --|RST               |
 *            |                  |
 *            |                  |
 *            |                  |
 *            |                  |
 *            |                  |
 * Author: 
*******************************************************************************/
/* DriverLib Includes */
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

/* Standard Includes */
#include <stdint.h>
#include <stdbool.h>

/**
 * main.c
 * DCOCLK= 3MHz -> SMCLK -> Timer Clock
 * 3Mhz/20kHz=150/2=75
 * CCR=75
 *
 */
int estado=1;
void main(void)
{
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer
    //CONFIGURACION DE SALIDA
    P2 -> DIR |= BIT0+BIT1+BIT2;
    //SELECCION FUNCION PERIFERICA PRIMARIA
    P2 -> OUT |= BIT0+BIT1+BIT2;

    P1 -> DIR |= BIT1;
    P1REN=0b00000010;
    P1OUT=0b00000010;

    for(;;){

        if(estado==1 && !(P1IN & BIT1)){
             estado= estado + 1;
             while(!(P1IN & BIT1)){

             }
           }else if(estado==2 && !(P1IN & BIT1)){
           estado= estado + 1;
           while(!(P1IN & BIT1)){

              }
           }else if(estado==3 && !(P1IN & BIT1)){
           estado= estado + 1;
           while(!(P1IN & BIT1)){

              }
           }else if(estado==4 && !(P1IN & BIT1)){
           estado= 1;
           while(!(P1IN & BIT1)){

              }
           }
        switch(estado){
        case 1:
            P2 -> OUT = BIT0;
            P2 -> OUT &= ~BIT1;
            P2 -> OUT &= ~BIT2;
                    break;
        case 2:
            P2 -> OUT &= ~BIT0;
            P2 -> OUT = BIT1;
            P2 -> OUT &= ~BIT2;
                    break;
        case 3:
            P2 -> OUT &= ~BIT0;
            P2 -> OUT &= ~BIT1;
            P2 -> OUT = BIT2;

                    break;
        case 4:
            P2 -> OUT = BIT0;
            P2 -> OUT &= ~BIT1;
            P2 -> OUT = BIT2;
                    break;
        default:

                    break;
        }

    }

}
