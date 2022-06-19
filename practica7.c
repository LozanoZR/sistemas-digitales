#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

#include <stdbool.h>
#include <stdint.h>

//9600
const eUSCI_UART_ConfigV1 uartConfig1 =
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
//115200
const eUSCI_UART_ConfigV1 uartConfig2 =
{
       EUSCI_A_UART_CLOCKSOURCE_SMCLK,          // SMCLK Clock Source
       6,                                     // BRDIV = 78
       8,                                       // UCxBRF = 2
       32,                                       // UCxBRS = 0
       EUSCI_A_UART_NO_PARITY,                  // No Parity
       EUSCI_A_UART_LSB_FIRST,                  // LSB First
       EUSCI_A_UART_ONE_STOP_BIT,               // One stop bit
       EUSCI_A_UART_MODE,                       // UART mode
       EUSCI_A_UART_OVERSAMPLING_BAUDRATE_GENERATION,  // Oversampling
       EUSCI_A_UART_8_BIT_LEN                  // 8 bit data length
};
uint8_t j=0;
volatile char mensaje[30];
int main(void)
{
    /* Stop Watchdog  */
    MAP_WDT_A_holdTimer();

    /* P1.2 and P1.3 en modo UART  */
        GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P1,GPIO_PIN2 | GPIO_PIN3, GPIO_PRIMARY_MODULE_FUNCTION );

        /* DCO to 12MHz */
        CS_setDCOCenteredFrequency(CS_DCO_FREQUENCY_12);

        UART_initModule(EUSCI_A0_BASE, &uartConfig1);

        /* Habilitamos UART */
        UART_enableModule(EUSCI_A0_BASE);
        /* Interrupciones */
                UART_enableInterrupt(EUSCI_A0_BASE, EUSCI_A_UART_RECEIVE_INTERRUPT);
                Interrupt_enableInterrupt(INT_EUSCIA0);

       //Uart 2
        /* P3.2 and P3.3 en modo UART  */
            GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P3,GPIO_PIN2 | GPIO_PIN3, GPIO_PRIMARY_MODULE_FUNCTION );

            /* DCO to 12MHz */
            CS_setDCOCenteredFrequency(CS_DCO_FREQUENCY_12);

            /* Configuración UART*/
            UART_initModule(EUSCI_A2_BASE, &uartConfig2);

            /* Habilitamos UART */
            UART_enableModule(EUSCI_A2_BASE);
           while(1){

           }//while
}//main


void EUSCIA0_IRQHandler(void)
{
    uint8_t i=0;
    uint32_t status = UART_getEnabledInterruptStatus(EUSCI_A0_BASE);
    /* Se envia dato recibido por Tx */
    if(status & EUSCI_A_UART_RECEIVE_INTERRUPT_FLAG)
    {
        i=UART_receiveData(EUSCI_A0_BASE);
                mensaje[j] = i;
                j++;

                if(i == 10){

                    for(i=0;i<50;i++){
                        if(mensaje[i] != 10){
                            UART_transmitData(EUSCI_A2_BASE, mensaje[i]);
                        }
                        else{
                            UART_transmitData(EUSCI_A2_BASE, mensaje[i]);
                            i=50;
                        }
                    }
                    i=0;
                    j=0;
                }
    }

}
