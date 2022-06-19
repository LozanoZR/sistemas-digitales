#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

/* Standard Includes */
#include <stdint.h>
#include <stdbool.h>
int cuenta, j, i=0, k=0, D;
int main(void)
{
    /* Stop Watchdog  */
    MAP_WDT_A_holdTimer();
    P4DIR=0xFF;//configuracion de GPIO e interrupciones
    P4OUT=0xFF;
    P2DIR=0xFF;
    P1DIR=0x00;
    P2OUT=0;
    P1OUT=0xFF;
    P1REN=0xFF;
    P1IES=0x20;
    P1IFG=0;

    NVIC -> ISER[1] = 1 << ((PORT1_IRQn) & 31);//servicio de interrupciones puerto 1
    __enable_irq();
    while(1)
    {
        P2OUT=0x00;
        P1IE=0x20;
        for(j=0;j<=27000;j++){
            P2OUT=0x00;
            if(P1IN & BIT5){//medir cuanto tiempo la señal está en alto y obtener el ciclo de trabajo
                k++;
            }
        }
        P1IE=0x00;
        cuenta=i/1000;
        D=k/3500;


        switch (cuenta){
        case 0:
            P4OUT=0b00000010;
            break;
        case 1:
            P4OUT=0b10011110;
            break;
        case 2:
            P4OUT=0b00100100;
            break;
        case 3:
            P4OUT=0b00001100;
            break;
        case 4:
            P4OUT=0b10011000;
            break;
        case 5:
            P4OUT=0b01001000;
            break;
        case 6:
            P4OUT=0b01000001;
            break;
        case 7:
            P4OUT=0b00011110;
            break;
        case 8:
            P4OUT=0b00000000;
            break;
        case 9:
            P4OUT=0b00001000;
            break;
        default:
            P4OUT=0b01000000;
            break;
        }
        for(j=0;j<=20000;j++){

        }
        i=0;
        k=0;
    }
}
void PORT1_IRQHandler(void){//contar cuantas veces en aproximadamente un segundo tenemos un flanco de subida
    P2OUT=0x01;
    i++;
    P1IFG=0;
}
