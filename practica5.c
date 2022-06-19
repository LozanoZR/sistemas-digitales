#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

/* Standard Includes */
#include <stdint.h>
#include <stdbool.h>
float periodous, frecuenciaKHz;
int main(void)
{
//12M
    /* Stop Watchdog  */
    MAP_WDT_A_holdTimer();
    TA0CTL=0b0000001000100000;
    //TA0CCTL0=0b0111000100010100;
    P1DIR=0x00;
    P1OUT=0xFF;
    P1REN=0xFF;
    P1IES=0x20;
    P1IFG=0;
    TA0CCR0=0;
    P1IE=0x20;
    NVIC -> ISER[1] = 1 << ((PORT1_IRQn) & 31);//servicio de interrupciones puerto 1
    __enable_irq();
    while(1)
    {

    }

}
void PORT1_IRQHandler(void){
    periodous=TA0R*10.416625;
    frecuenciaKHz=1000/periodous;
    TA0R=0;
}

