// ------------------------------------------------------------------------------
// DATOS Y ENCABEZADO
//-------------------------------------------------------------------------------
    
// Archivo:                 lab6.c
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

#define _tmr0_value 200         // Usando define para colocar el valor del timer0
#define _XTAL_FREQ 8000000      // Se coloca el valor de la frecuencia que se eligió

void __interrupt() isr(void)    // Rutina de interrupción ISR que no recibe parámetros
{ 
    if (T0IF){
        PORTA++;
        TMR0 = _tmr0_value;     // Le ponemos valor al TMR0
        T0IF = 0;               // Bandera
    }
}

void setup(void);               // Prototipo del setup
void main(void) 
{ 
    
// ------------------------------------------------------------------------------
// LOOP PRINCIPAL
//-------------------------------------------------------------------------------
    
    setup();
    while(1){
        // Leer el primer potenciómetro en AN0
        ADCON0bits.CHS = 0b0000;    // Seleccionar AN0 como entrada analógica
        ADCON0bits.GO = 1;          // Comenzar la conversión
        while(ADIF==0);             // Esperar a que se complete la conversión
        ADIF = 0;                   // Limpiar la bandera de interrupción
        int adc1 = (ADRESH<<8) + ADRESL;    // Guardar el valor de la conversión en ADRESH y ADRESL

        // Leer el segundo potenciómetro en AN1
        ADCON0bits.CHS = 0b0001;    // Seleccionar AN1 como entrada analógica
        ADCON0bits.GO = 1;          // Comenzar la conversión
        while(ADIF==0);             // Esperar a que se complete la conversión
        ADIF = 0;                   // Limpiar la bandera de interrupción
        int adc2 = (ADRESH<<8) + ADRESL;    // Guardar el valor de la conversión en ADRESH y ADRESL

        PORTC = (char)(adc1>>2);    // Para meter el valor dentro de un puerto. Se le quitan 2 para ser 8 
        PORTD = (char)(adc2>>2);    // Para meter el valor dentro de un puerto. Se le quitan 2 para ser 8 
        __delay_ms(10);
    }
   return;
} 
// ------------------------------------------------------------------------------
// SETUP
//-------------------------------------------------------------------------------

void setup(void)
{
    ANSEL   =    0;         
    ANSELH  =    0;
    TRISC   =    0;                              // Contador 1
    TRISD   =    0;
    TRISB   =    0;                              // Pots

    
// Configurando el oscilador 
    OSCCONbits.IRCF = 0b111;                // 8MHz
    OSCCONbits.SCS = 1;                     // Set
    
// Configurando el timer0
    OPTION_REGbits.T0CS = 0;
    OPTION_REGbits.PSA = 1;
    OPTION_REGbits.PS = 0b111;
    TMR0 = _tmr0_value;                     // Interrupción y registro del TMR0

// Configurando la interrupción del TMR0
    INTCONbits.T0IF = 0;
    INTCONbits.T0IE = 1;
    //INTCONbits.PEIE = 0;
    INTCONbits.GIE = 1;                     // Global (por lo general se activa de último)
    
// Configurando el ADC
    ANSEL = 0b01;
    TRISA = 0b01;
    ADCON0bits.ADCS = 0b10;
    ADCON0bits.CHS = 0;
    __delay_ms (1);
    ADCON1bits.ADFM = 1;
    ADCON1bits.VCFG0 = 0;
    ADCON1bits.VCFG1 = 0;
    ADCON0bits.ADON = 1;
    
    ADIF = 0;                   // Limpiar la bandera

    

}