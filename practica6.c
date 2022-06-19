//#include "msp.h"
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <stdint.h>
#include<stdbool.h>
#include <string.h>
#include <stdio.h>

/* Select the global Q value */
#define GLOBAL_Q    12
#define orden 4
const float coeficientes_b[orden]={
0.222951453111403047335770111203601118177,
0.277048546888596980419805504425312392414,
0.277048546888596980419805504425312392414,
0.222951453111403047335770111203601118177
};
volatile float memoria [orden];
uint16_t ValorADC;
float VotlajeNormalizado;
float y;
float EMA;
float alpha;
float valor_n;
float SMA;
float Fir;
int N;

char msg[30];

const eUSCI_UART_ConfigV1 uartConfig =
{
        EUSCI_A_UART_CLOCKSOURCE_SMCLK,          // SMCLK Clock Source
        78,                                     // BRDIV = 78
        2,                                       // UCxBRF = 2
        0,                                       // UCxBRS = 0
        EUSCI_A_UART_NO_PARITY,                  // No Parity
        EUSCI_A_UART_LSB_FIRST,                  // LSB First
        EUSCI_A_UART_ONE_STOP_BIT,               // One stop bit
        EUSCI_A_UART_MODE,                       // UART mode
        EUSCI_A_UART_OVERSAMPLING_BAUDRATE_GENERATION,  // Oversampling
        EUSCI_A_UART_8_BIT_LEN                  // 8 bit data length
};
/**
 * main.c
 */
void main(void)
{
    int var=0;
    for(var; var < orden; ++var){
        memoria[var]=0;
    }
    //UART
    /* Halting WDT  */
       MAP_WDT_A_holdTimer();

       /* P1.2 and P1.3 en modo UART  */
       GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P1,
               GPIO_PIN2 | GPIO_PIN3, GPIO_PRIMARY_MODULE_FUNCTION);

       /* DCO to 12MHz */
       CS_setDCOCenteredFrequency(CS_DCO_FREQUENCY_12);

       /* Configuración UART con base a la estructura de arriba */
       UART_initModule(EUSCI_A0_BASE, &uartConfig);

       /* Habilitamos UART */
       UART_enableModule(EUSCI_A0_BASE);

       /* Interrupciones */
       UART_enableInterrupt(EUSCI_A0_BASE, EUSCI_A_UART_RECEIVE_INTERRUPT);
       Interrupt_enableInterrupt(INT_EUSCIA0);
       //Interrupt_enableSleepOnIsrExit();
       Interrupt_enableMaster();
    //ADC
    /*Configuracion flash estado espera*/
    FlashCtl_setWaitState(FLASH_BANK0, 1);
    FlashCtl_setWaitState(FLASH_BANK1, 1);

    /* Configuracion DC0 12MHz*/
    PCM_setPowerState(PCM_AM_LDO_VCORE1);
    CS_setDCOCenteredFrequency(CS_DCO_FREQUENCY_12);

    /*HABILITAMOS FPU*/
    FPU_enableModule();
    FPU_enableLazyStacking();

    /*Configuracion ADC*/
    ADC14_enableModule();
    ADC14_initModule(ADC_CLOCKSOURCE_MCLK, ADC_DIVIDER_1,ADC_DIVIDER_4, ADC_NOROUTE);

    /*GPIO CONFIGURACION*/
    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P5, GPIO_PIN5,GPIO_TERTIARY_MODULE_FUNCTION);

    /*CONFIGURACION ADC MEMORIA*/
    ADC14_configureSingleSampleMode(ADC_MEM0,true);
    ADC14_configureConversionMemory(ADC_MEM0, ADC_VREFPOS_AVCC_VREFNEG_VSS, ADC_INPUT_A0, ADC_NONDIFFERENTIAL_INPUTS);

    /*cONFIGURACION TIMER EN MODO MANUAL*/
    ADC14_enableSampleTimer(ADC_AUTOMATIC_ITERATION);

    ADC14_enableConversion();
    ADC14_toggleConversionTrigger();

    /*Interrupciones*/
    ADC14_enableInterrupt(ADC_INT0);
    Interrupt_enableInterrupt(INT_ADC14);
    Interrupt_enableMaster();
    ValorADC=0;
        VotlajeNormalizado=0;

        EMA=0;
        y=0;
        SMA=0;
        valor_n=0;
        Fir=0;
        N=1;
    while(1){
        PCM_gotoLPM0();
    }
}

/*Servicio de interrupcion
 * cuando el registro ADC_MEM0 esta listo para leer*/

void ADC14_IRQHandler(void){
    uint16_t status= ADC14_getEnabledInterruptStatus();
    ADC14_clearInterruptFlag(ADC_INT0);
    if(ADC_INT0 & status){
        ValorADC= ADC14_getResult(ADC_MEM0);
        VotlajeNormalizado= ((float)ValorADC)/16383*3.3;
        //filtro EMA
        alpha=1;
        y=(alpha*(VotlajeNormalizado))+((1-alpha)*EMA);
        EMA=y;

        //filtro 2
        valor_n= VotlajeNormalizado + valor_n;
        SMA=valor_n/N;
        N++;

        //filtro FIR
        int k=orden;
        for(k;k!=1;k--){
            memoria[k]=memoria[k-1];
        }
        memoria[1]=VotlajeNormalizado;
        Fir=0;
        int n=1;
        for(n;n<=orden; n++){
            Fir += coeficientes_b[n]*memoria[n];
        }

        sprintf(msg,"V= %.3f, EMA= %.3f, SMA= %.5f, FIR= %.3f\r\n",VotlajeNormalizado,y,SMA,Fir);
        UART0_OutString(msg);

    }
}
/*Funcion para envio de string */
void UART0_OutString(char *pt){
    while(*pt){
        UART0_OutChar(*pt);
        pt++;
    }
}
void UART0_OutChar(char letra){
    UART_transmitData(EUSCI_A0_BASE, letra);
}

/* Servicio interrupción EUSCIA_0 */
void EUSCIA0_IRQHandler(void)
{
    char msg[30];
    float voltaje = 3.14;
    uint8_t i = 0;

    uint32_t status = UART_getEnabledInterruptStatus(EUSCI_A0_BASE);
    /* Se envia dato recibido por Tx */
    if(status & EUSCI_A_UART_RECEIVE_INTERRUPT_FLAG)
    {

        i = UART_receiveData(EUSCI_A0_BASE);
        UART_transmitData(EUSCI_A0_BASE, i);
    }
}
