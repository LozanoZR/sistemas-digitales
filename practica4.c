
/* DriverLib Includes */
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

/* Standard Includes */
#include <stdint.h>

#include <string.h>

volatile uint16_t servo1;
volatile uint16_t servo2;
volatile uint16_t servo3;
float serv1;
        float C_T1;
        int frec1;
        float serv2;
        float C_T2;
        float serv3;
        float C_T3;
void main(void)
{
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
           GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P5, GPIO_PIN4,GPIO_TERTIARY_MODULE_FUNCTION);
           GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P5, GPIO_PIN3,GPIO_TERTIARY_MODULE_FUNCTION);
           /*CONFIGURACION ADC MEMORIA*/
           ADC14_configureSingleSampleMode(ADC_MEM0,true);
           ADC14_configureConversionMemory(ADC_MEM0, ADC_VREFPOS_AVCC_VREFNEG_VSS, ADC_INPUT_A0, ADC_NONDIFFERENTIAL_INPUTS);
           ADC14_configureSingleSampleMode(ADC_MEM1,true);
           ADC14_configureConversionMemory(ADC_MEM1, ADC_VREFPOS_AVCC_VREFNEG_VSS, ADC_INPUT_A1, ADC_NONDIFFERENTIAL_INPUTS);
           ADC14_configureSingleSampleMode(ADC_MEM2,true);
           ADC14_configureConversionMemory(ADC_MEM2, ADC_VREFPOS_AVCC_VREFNEG_VSS, ADC_INPUT_A2, ADC_NONDIFFERENTIAL_INPUTS);

           /*cONFIGURACION TIMER EN MODO MANUAL*/
           ADC14_enableSampleTimer(ADC_AUTOMATIC_ITERATION);

           ADC14_enableConversion();
           ADC14_toggleConversionTrigger();
    //------------------------------------------------------------
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer
            //CONFIGURACION DE SALIDA P10.5
            P10 -> DIR |= BIT5;
            //SELECCION FUNCION PERIFERICA PRIMARIA
            P10 -> SEL0 |= BIT5;
            CS -> KEY = CS_KEY_VAL;

                //DESHABILITAMOS EL ACCESO A ESCRITURA AL REGISTRO CS
                CS -> KEY = 0;
            TIMER_A3 -> CTL = TIMER_A_CTL_MC__UP | TIMER_A_CTL_SSEL__SMCLK | TIMER_A_CTL_CLR;
            //CARGAR BASE DE TIEMPO EN CCR0
            TIMER_A3 -> CCR[0]=60000;
            //MODO reset/set REGISTRO CAPTURA COMPARACION
            TIMER_A3 -> CCTL[1]= TIMER_A_CCTLN_OUTMOD_7;
            __enable_irq();
            NVIC -> ISER[0] |= 1 << ((TA3_0_IRQn) & 31);

    //CONFIGURACION DE SALIDA P5.6
                    P5 -> DIR |= BIT6;
                    //SELECCION FUNCION PERIFERICA PRIMARIA
                    P5 -> SEL0 |= BIT6;
                    CS -> KEY = CS_KEY_VAL;

                        //DESHABILITAMOS EL ACCESO A ESCRITURA AL REGISTRO CS
                        CS -> KEY = 0;
                    TIMER_A2 -> CTL = TIMER_A_CTL_MC__UP | TIMER_A_CTL_SSEL__SMCLK | TIMER_A_CTL_CLR;
                    //CARGAR BASE DE TIEMPO EN CCR0
                    TIMER_A2 -> CCR[0]=60000;
                    //MODO reset/set REGISTRO CAPTURA COMPARACION
                    TIMER_A2 -> CCTL[1]= TIMER_A_CCTLN_OUTMOD_7;
                    __enable_irq();
                    NVIC -> ISER[0] |= 1 << ((TA2_0_IRQn) & 31);

     //CONFIGURACION DE SALIDA P7.7
                            P7 -> DIR |= BIT7;
                            //SELECCION FUNCION PERIFERICA PRIMARIA
                            P7 -> SEL0 |= BIT7;
                            CS -> KEY = CS_KEY_VAL;

                                //DESHABILITAMOS EL ACCESO A ESCRITURA AL REGISTRO CS
                                CS -> KEY = 0;
                            TIMER_A1 -> CTL = TIMER_A_CTL_MC__UP | TIMER_A_CTL_SSEL__SMCLK | TIMER_A_CTL_CLR;
                            //CARGAR BASE DE TIEMPO EN CCR0
                            TIMER_A1 -> CCR[0]=60000;
                            //MODO reset/set REGISTRO CAPTURA COMPARACION
                            TIMER_A1 -> CCTL[1]= TIMER_A_CCTLN_OUTMOD_7;
                            __enable_irq();
                            NVIC -> ISER[0] |= 1 << ((TA1_0_IRQn) & 31);
    /* Enabling Interrupts */
        ADC14_enableInterrupt(ADC_INT0);
        Interrupt_enableInterrupt(INT_ADC14);
        Interrupt_enableMaster();
    while (1)
    {

    }
}

/* This interrupt is fired whenever a conversion is completed and placed in
 * ADC_MEM7. This signals the end of conversion and the results array is
 * grabbed and placed in resultsBuffer */
void ADC14_IRQHandler(void)
{
    uint16_t status= ADC14_getEnabledInterruptStatus();
//---------------------------------------------------------------------------

        ADC14_clearInterruptFlag(ADC_INT0);

            if(ADC_INT0 & status){
                servo1= ADC14_getResult(ADC_MEM0); //Obtener valor ADC
                serv1=servo1;           //Cambio de variable
                C_T1= (serv1/16383)*60000;         //Ciclo de trabajo de 0-1
                frec1=C_T1;

                servo2= ADC14_getResult(ADC_MEM1); //Obtener valor ADC
                serv2=servo2;           //Cambio de variable
                C_T2= (serv2/16383)*60000;         //Ciclo de trabajo de 0-1

                servo3= ADC14_getResult(ADC_MEM2); //Obtener valor ADC
                serv3=servo3;           //Cambio de variable
                C_T3= (serv3/16383)*60000;         //Ciclo de trabajo de 0-1
            }
            TIMER_A3 -> CCR[1]=frec1;
            TIMER_A2 -> CCR[1]=C_T2;
            TIMER_A1 -> CCR[1]=C_T3;
//--------------------------------------------------------------------------
}
