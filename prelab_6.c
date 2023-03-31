// ------------------------------------------------------------------------------
// DATOS Y ENCABEZADO
//-------------------------------------------------------------------------------
    
// Archivo:                 prelab6.c
// Dispositivo:             PIC16F887
// Autor:                   GUSTABO CÓRDOVA 
// Programa:                Configurar Timer 0 / Estado de LED
// Creado:                  25 FEB, 2023 
// Última modificación:     26 FEB, 2023

// ------------------------------------------------------------------------------
// CONFIGURACIÓN DEL PIC
//-------------------------------------------------------------------------------

// CONFIG1
#pragma config FOSC     =   INTRC_NOCLKOUT  // Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE     =   OFF             // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE    =   OFF             // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE    =   OFF             // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP       =   OFF             // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD      =   OFF             // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN    =   OFF             // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO     =   OFF             // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN    =   OFF             // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP      =   OFF             // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V    =   BOR40V          // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT      =   OFF             // Flash Program Memory Self Write Enable bits (Write protection off)

// ------------------------------------------------------------------------------
// VARIABLES
//-------------------------------------------------------------------------------

#include <xc.h>
#include <stdint.h>
 
#define _XTAL_FREQ 8000000UL                // UL se coloca como un seguro para tener valores positivos
 
void __interrupt() isr(void)                // Declaración de la función de interrupción
{ 
    if (T0IF)                               // Verifica si la bandera de interrupción del Timer0 está activa
    {
        PORTAbits.RA0 = ~PORTAbits.RA0;     // Invierte el estado del pin RA0
        TMR0 = 156;                          // Recarga el valor del Timer0 para generar una interrupción en 200 us
        T0IF = 0;                           // Limpia la bandera de interrupción del Timer0
    }
}
 
void delay_ms(uint16_t ms)                  // Función para generar retardos en milisegundos
{
    for (uint16_t i = 0; i < ms; i++)
    {
        __delay_ms(1);                      // Llamada a la función de retardos del compilador
    }
}

// ------------------------------------------------------------------------------
// SETUP
//-------------------------------------------------------------------------------

void setup(void)                            // Función para configurar los registros del microcontrolador
{
    ANSEL  = 0;                             // Configura los pines como entradas/salidas digitales
    ANSELH = 0;
    TRISA = 0;                   // Coloca el pin 0 del puerto A como salida digital
    //TRISD = 0;
 
// Configurando el oscilador 
    OSCCONbits.IRCF = 0b111;                // 8MHz
    OSCCONbits.SCS = 1;                     // Set
    
// Configurando el timer0
    OPTION_REGbits.T0CS = 0;
    OPTION_REGbits.PSA = 1;
    OPTION_REGbits.PS = 0b111;
    TMR0 = 156;                              // Interrupción y registro del TMR0

// Configurando la interrupción del TMR0
    INTCONbits.T0IF = 0;
    INTCONbits.T0IE = 1;
    //INTCONbits.PEIE = 0;
    INTCONbits.GIE = 1;                     // Global (por lo general se activa de último)
}


// ------------------------------------------------------------------------------
// LOOP PRINCIPAL
//-------------------------------------------------------------------------------
 
void main(void) 
{ 
    setup();                // Llamada a la función de configuración
 
    while(1)                // Ciclo infinito
    {
        //PORTA++;            // Incrementa el valor del puerto A en 1
        //delay_ms(100);      // Genera un retardo de 165 ms
    }
     return;
}

