#include "msp.h"


/**
 * main.c
 * DCOCLK= 3MHz -> SMCLK -> Timer Clock
 * 3Mhz/20kHz=150/2=75
 * CCR=75
 *
 */
int estado=1;
int j;
int k;
int l;
int est=1;
void main(void)
{
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer
    //CONFIGURACION DE SALIDA P1.0
    P1 -> DIR |= BIT0;

    P1 -> SEL0 |= BIT0;


    P5 -> DIR |= BIT0;
    P5 -> DIR |= BIT1;
    P5 -> DIR |= BIT2;

    P5REN = 0B00000001;
    P5OUT = 0B00000001;

    P5REN = 0B00000010;
    P5OUT = 0B00000010;

    P5REN = 0B00000100;
    P5OUT = 0B00000100;

    P2 -> DIR |= BIT5;
        P2 -> DIR |= BIT7;
        P2 -> DIR |= BIT6;

        P2REN = 0B00100000;
        P2OUT = 0B00100000;

        P2REN = 0B10000000;
        P2OUT = 0B10000000;

        P2REN = 0B01000000;
        P2OUT = 0B01000000;

        P3 -> DIR |= BIT0;
            P2 -> DIR |= BIT4;
            P5 -> DIR |= BIT6;

            P3REN = 0B00000001;
            P3OUT = 0B00000001;

            P2REN = 0B00010000;
            P2OUT = 0B00010000;

            P5REN = 0B01000000;
            P5OUT = 0B01000000;

            P6 -> DIR |= BIT6;
                P5 -> DIR |= BIT7;
                P6 -> DIR |= BIT7;

                P6REN = 0B01000000;
                P6OUT = 0B01000000;

                P5REN = 0B10000000;
                P5OUT = 0B10000000;

                P6REN = 0B10000000;
                P6OUT = 0B10000000;
/*
 *             P2OUT |= BIT5;   //amarillo 2
            P2OUT &= ~BIT7;
            P2OUT &= ~BIT6;

            P3OUT |= BIT0;   //amarillo 3
            P2OUT &= ~BIT4;
            P5OUT &= ~BIT6;

            P6OUT |= BIT6;  //amarillo 4
            P5OUT &= ~BIT7;
            P6OUT &= ~BIT7;
 *
 */

    //SELECCION FUNCION PERIFERICA PRIMARIA
//bOTÓN
    P1 -> DIR |= BIT1;
    P1REN=0b00000010;
    P1OUT=0b00000010;

    //HABILITAMOS ACCESO A ESCRITURA AL REGISTRO CS
    TIMER_A3 -> CTL = TIMER_A_CTL_MC__UP | TIMER_A_CTL_SSEL__SMCLK | TIMER_A_CTL_CLR;
    //CARGAR BASE DE TIEMPO EN CCR0
    TIMER_A3 -> CCR[0]=3000000;
    TIMER_A3 -> CCR[1]=600000;
    //MODO reset/set REGISTRO CAPTURA COMPARACION
    TIMER_A3 -> CCTL[0]= TIMER_A_CCTLN_OUTMOD_3 + TIMER_A_CCTLN_CCIE;
    TIMER_A3 -> CCTL[1]= TIMER_A_CCTLN_OUTMOD_3 + TIMER_A_CCTLN_CCIE;
    __enable_irq();
    NVIC -> ISER[0] |= 1 << ((TA3_0_IRQn) & 31);
    NVIC -> ISER[0] |= 1 << ((TA3_N_IRQn) & 31);
    for(;;){

       if(est==1 & !(P1IN & BIT1)){
             est= 2;
             while(!(P1IN & BIT1)){

             }
           }else if(est==2 & !(P1IN & BIT1)){
           est= 1;
           estado=1;
           while(!(P1IN & BIT1)){

              }
           }switch(est){
           case 1:

        switch(estado){
          case 1:
            //prender verde1, apaga rojo1
            P5OUT &= ~BIT2;//rojo 1
            P5OUT |= BIT0; //verde1
            P2OUT |= BIT6;  //prende rojo 2do
            P5OUT |= BIT6;  //prende rojo 3ro
            P6OUT |= BIT7;   //prende rojo 4to

            if(k==50){
                            estado = 2;
                            k =1;
                        }
                    break;
        case 2:
           // parpadear verd
            P5OUT |= BIT0;
            for(j=0;j<10000;j++){

            }
            P5OUT &= ~BIT0;
                            for(j=0;j<10000;j++){

                            }
            if(k==50){
                estado = 3;
                k =1;
            }
                    break;
        case 3:
            //apaga verde, parpadea amarillo
            P5OUT &= ~BIT0;
            P5OUT |= BIT1;

                        for(j=0;j<10000;j++){

                        }
                        P5OUT &= ~BIT1;
                                        for(j=0;j<10000;j++){

                                        }
                        if(k==50){
                            estado = 4;
                            k =1;
                        }
                    break;
        case 4:




            P5OUT &= ~BIT1;//apaga amarillo
            P5OUT |= BIT2;//prende rojo 1er
            P2OUT |= BIT7;//prender verde 2do
            P2OUT &= ~BIT6;//apagar rojo 2do
            if(k==50){
                            estado = 5;
                            k =1;
                        }

                    break;
      //-------------------------------------------------------------------------------------------------------------
                    case 5:
            //parpadear verde 2do
                        P2OUT |= BIT7;
                                    for(j=0;j<10000;j++){

                                    }
                                    P2OUT &= ~BIT7;
                                                    for(j=0;j<10000;j++){

                                                    }
                                    if(k==50){
                                        estado = 6;
                                        k =1;
                                    }
                    break;
        case 6:
            //apaga verde 2do, parpadea amarillo 2do
                        P2OUT &= ~BIT7;
                        P2OUT |= BIT5;

                                    for(j=0;j<10000;j++){

                                    }
                                    P2OUT &= ~BIT5;
                                                    for(j=0;j<10000;j++){

                                                    }
                                    if(k==50){
                                        estado = 7;
                                        k =1;
                                    }
                    break;
        case 7:




                        P2OUT &= ~BIT5; //apaga amarillo 2do
                        P2OUT |= BIT6;  //prende rojo 2do
                        P2OUT |= BIT4;  //prender verde 3ro
                        P5OUT &= ~BIT6;  //apagar rojo 3ro
                        if(k==50){
                                        estado = 8;
                                        k =1;
                                    }
                    break;
        case 8:
            estado = 9;

                    break;
      //------------------------------------------------------------------------------------------------------------
         case 9:
             // parpadear verd
                         P2OUT |= BIT4;
                         for(j=0;j<10000;j++){

                         }
                         P2OUT &= ~BIT4;
                                         for(j=0;j<10000;j++){

                                         }
                         if(k==50){
                             estado = 10;
                             k =1;
                         }
                    break;
        case 10:
            //apaga verde 3ro, parpadea amarillo 3ro
                        P2OUT &= ~BIT4;
                        P3OUT |= BIT0;

                                    for(j=0;j<10000;j++){

                                    }
                                    P3OUT &= ~BIT0;
                                                    for(j=0;j<10000;j++){

                                                    }
                                    if(k==50){
                                        estado = 11;
                                        k =1;
                                    }
                    break;
        case 11:




                        P3OUT &= ~BIT0; //apaga amarillo 3ro
                        P5OUT |= BIT6;  //prende rojo 3ro
                        P5OUT |= BIT7;  //prender verde 4to
                        P6OUT &= ~BIT7; //apagar rojo 4to
                        if(k==50){
                                        estado = 12;
                                        k =1;
                                    }
                    break;
        case 12:
            // parpadear verd
                        P5OUT |= BIT7;
                        for(j=0;j<10000;j++){

                        }
                        P5OUT &= ~BIT7;
                                        for(j=0;j<10000;j++){

                                        }
                        if(k==50){
                            estado = 13;
                            k =1;
                        }
                    break;
       //---------------------------------------------------------------------------------------------------------------
         case 13:
             //apaga verde 4t, parpadea amarillo 4to
                         P5OUT &= ~BIT7;
                         P6OUT |= BIT6;

                                     for(j=0;j<10000;j++){

                                     }
                                     P6OUT &= ~BIT6;
                                                     for(j=0;j<10000;j++){

                                                     }
                                     if(k==50){
                                         estado = 14;
                                         k =1;
                                     }
                    break;
        case 14:




                        P6OUT &= ~BIT6;  //apaga amarillo 4to
                        P6OUT |= BIT7;   //prende rojo 4to
                        P5OUT |= BIT0;   //prender verde del primero
                        P5OUT &= ~BIT2;  //apagar rojo del primero
                        if(k==50){
                                        estado = 1;
                                        k =1;
                                    }
                    break;
        //---------------------------------------------------------------------------------------------------
        default:
                estado=1;
                    break;
        }//switch estado
        break;
        case 2:
            P5OUT |= BIT1;   //amarillo 1
            P5OUT &= ~BIT0;  //verde
            P5OUT &= ~BIT2;  //rojo

            P2OUT |= BIT5;   //amarillo 2
            P2OUT &= ~BIT7;
            P2OUT &= ~BIT6;

            P3OUT |= BIT0;   //amarillo 3
            P2OUT &= ~BIT4;
            P5OUT &= ~BIT6;

            P6OUT |= BIT6;  //amarillo 4
            P5OUT &= ~BIT7;
            P6OUT &= ~BIT7;
            for(j=0;j<5000;j++){

            }
            P5OUT &= ~BIT1;  //amarillo 1
            P2OUT &= ~BIT5;   //amarillo 2
            P3OUT &= ~BIT0;   //amarillo 3
            P6OUT &= ~BIT6;  //amarillo 4
            for(j=0;j<5000;j++){

                        }
            break;
        default:

            break;

    }//switch est

    }
    //for
}//main
void TA3_0_IRQHandler(void){
    TIMER_A3 -> CCTL[0] &= ~TIMER_A_CCTLN_CCIFG;
    k = k+1;
    if(k>50){
        k=1;
    }
}

void TA3_N_IRQHandler(void){
    if(TIMER_A3 -> CCTL[1] &= ~TIMER_A_CCTLN_CCIFG){
    TIMER_A3 -> CCTL[1] &= ~TIMER_A_CCTLN_CCIFG;
    j = j+1;}
}
