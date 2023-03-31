// ------------------------------------------------------------------------------
// DATOS Y ENCABEZADO
//-------------------------------------------------------------------------------
    
// Archivo:                 POSTLAB_6.c
// Dispositivo:             PIC16F887
// Autor:                   GUSTABO CÓRDOVA 
// Creado:                  30 FEB, 2023
// Última modificación:     31 FEB, 2023

// ------------------------------------------------------------------------------
// CONFIGURACIÓN DEL PIC
//-------------------------------------------------------------------------------

// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT    // Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF               // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF              // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF              // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF                 // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF                // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF              // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF               // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF              // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF                // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V           // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF                // Flash Program Memory Self Write Enable bits Write protection off)


//------------------------------------------------------------------------------
// LIBRERÍAS 
//------------------------------------------------------------------------------
#include <xc.h>
#include <stdint.h>
#include <stdio.h>

//------------------------------------------------------------------------------
// DEFINE
//------------------------------------------------------------------------------
#define _XTAL_FREQ 4000000
#define _tmr0_value 200

//------------------------------------------------------------------------------
// LISTA DEL DISPLAY
//------------------------------------------------------------------------------

unsigned char display[] =
{
    0b00111111, 0b00000110, 0b01011011,0b01001111, 0b01100110, 0b01101101, 0b01111101, 0b00000111, 0b01111111, 0b01101111, //0,1,2,3,4,5,6,7,8,9
};


//------------------------------------------------------------------------------
// VARIABLES NUEVAS
//------------------------------------------------------------------------------

unsigned int unidad, decena, centena, diferencia;
unsigned int variable = 0;               

void setup(void);

//------------------------------------------------------------------------------
// INTERRUPCIONES
//------------------------------------------------------------------------------
void __interrupt() isr (void){              // Función de interrupción
    if (T0IF == 1){                         // Verifica si la bandera de interrupción del temporizador 0 es igual a 1
        PORTD++;                            // Incrementa el valor del puerto D en 1
    }
   
    if(T0IF == 1){                          // Verifica si la bandera de interrupción del temporizador 0 es igual a 1
        variable = variable*1.964;          // Multiplica "variable" por 1.964 y guarda el resultado en "variable"
        centena = variable/100;             // Calcula las centena de "variable"
        diferencia = variable%100;          // Calcula el diferencia de "variable" al dividir entre 100
        decena = diferencia/10;             // Calcula las decena del diferencia anterior
        unidad = diferencia%10;             // Calcula las unidad del diferencia anterior
        T0IF = 0;                           // Limpia la bandera de interrupción del temporizador 0 estableciéndola en 0
    }
   
    if(PIR1bits.ADIF){                     
        if(ADCON0bits.CHS == 12){           // Verificamos sea AN12 (RB0) el canal seleccionado
            PORTA = ADRESH;                 // Mostramos ADRESH en PORTA
        }
       
        else
            variable = ADRESH;
            PIR1bits.ADIF = 0;               // Limpiamos bandera de interrupción
    }
}

//------------------------------------------------------------------------------
// LOOP
//------------------------------------------------------------------------------
void main(void) {
    setup();                             // Se llama a la función setup para inicializar el microcontrolador
   
    __delay_us(50);                     // Se espera 50 microsegundos
    ADCON0bits.GO = 1;                  // Se inicia la conversión analógico-digital
   
    while(1){
       
        PORTC = display[centena];      // Salida de centena en el display en puerto C
        PORTE = 0B001;                  // Se activa bit 0 en puerto E para transistores.
        __delay_ms(10);                 // Retardo de 10 milisegundos
        PORTE = 0B000;                  // Desactivación de la selección de digitos
       
        PORTC = display[decena];       // Salida de decena en el display en puerto C
        PORTE = 0B010;                  // Se activa bit 1 en puerto E para transistores.
        __delay_ms(10);                 // Retardo de 10 milisegundos
        PORTE = 0B000;                  // Desactivación de la selección de digitos
       
        PORTC = display[unidad];      // Salida de decena en el display en puerto C
        PORTE = 0B100;                  // Se activa bit 1 en puerto E para transistores.
        __delay_ms(10);                 // Retardo de 10 milisegundos
        PORTE = 0B000;                  // Desactivación de la selección de digitos
       
if(ADCON0bits.GO == 0){                 // Verificar si la conversión ya ha terminado               
            if(ADCON0bits.CHS == 12) {  // Verificar si el canal actual es AN12(RB0)
                ADCON0bits.CHS = 10;    // Cambiar al canal AN10(RB1)
            }
            else
                ADCON0bits.CHS = 12;    // Cambiar al canal AN12(RB0))
            
            __delay_us(100);           
            ADCON0bits.GO = 1;          
        }
    }
}


//------------------------------------------------------------------------------
// CONFIGURACION
//------------------------------------------------------------------------------
void setup(void){

    ANSEL = 0;                      // Pines AN como entradas digitales
    ANSELH = 0b00000011;            // AN8, AN9 como entradas analógicas

    TRISA = 0;                      // Pines del PORTA como salidas digitales
    PORTA = 0;                      // Ponemos todos los pines del puerto A en estado bajo
    
    TRISE = 0;                      // Pines del PORTE como salidas digitales
    PORTE = 0;                      

    TRISB = 0b00000011;             // RB0 y RB1 como entradas
    PORTB = 0b00000011;             

    TRISC = 0;                      // Pines del PORTC como salidas digitales
    PORTC = 0;                      

    TRISD0 = 0;                     // PIN RD0 como salida digital
    PORTD = 0;                      


// Se configura el oscilador
    OSCCONbits.IRCF2 = 1;       // Se selecciona una frecuencia de oscilación interna de 4 MHz
    OSCCONbits.IRCF1 = 1;
    OSCCONbits.IRCF0 = 0;
    OSCCONbits.SCS = 1;         // Se selecciona el oscilador interno como fuente de reloj del sistema

// Se configura el TMR0
    OPTION_REGbits.T0CS = 0;    // Se selecciona el modo temporizador/contador del TMR0
    OPTION_REGbits.PSA = 0;
    OPTION_REGbits.PS2 = 1;     // Se selecciona un preescaler de 1:256 para el TMR0
    OPTION_REGbits.PS1 = 1;
    OPTION_REGbits.PS0 = 1;

// Se configura el ADC
    ADCON0bits.ADCS = 0b01;     // Se selecciona un clock de conversion Fosc/8 para el ADC
    ADCON1bits.VCFG0 = 0;       // Se selecciona la referencia de voltaje VSS para el pin de tierra
    ADCON1bits.VCFG1 = 0;       // Se selecciona la referencia de voltaje VDD para el pin de referencia positiva
    ADCON0bits.CHS = 12;        // Se selecciona el canal AN12 como entrada analógica
    ADCON1bits.ADFM = 0;        // Se alinea a la izquierda los bits de resultado de la conversion
    __delay_us(50);             // Se espera un tiempo de 50 microsegundos antes de encender el ADC
    ADCON0bits.ADON = 1;        // Se enciende el ADC

// Se configuraN las interrupciones
    PIR1bits.ADIF = 0;          // Se borra el flag de interrupción del ADC
    PIE1bits.ADIE = 1;          // Se habilita la interrupción del ADC
    INTCONbits.PEIE = 1;        // Se habilita la interrupción de periféricos
    INTCONbits.GIE = 1;         // Se habilita la interrupción global
    INTCONbits.T0IE = 1;        // Se habilita la interrupción del TMR0
    INTCONbits.T0IF = 0;        // Se borra el flag de interrupción del TMR0
    INTCONbits.TMR0IF = 0;      // Se borra el flag de interrupción del TMR0
    INTCONbits.TMR0IE = 1;      // Se habilita la interrupción del TMR0
    INTCONbits.RBIF = 0;        // Se borra el flag de interrupción del Puerto B

    return; 
    }