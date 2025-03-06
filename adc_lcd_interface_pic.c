#define _XTAL_FREQ 20000000
#include <xc.h>
#include <stdint.h>
#include "EUSART1.h"

#define LED RA0
#define ADC_CHANNEL 0

unsigned int BPM, Signal, IBI = 600;
unsigned char Pulse = 0;
unsigned int P = 512, T = 512, thresh = 525, amp = 100;
unsigned char firstBeat = 1, secondBeat = 0;

void init();
unsigned int ADC_Read(int);
void calculate_heart_beat(int);
void __interrupt() ISR(void);

void main() {
    init();
    while (1) {
        Signal = ADC_Read(ADC_CHANNEL);
        calculate_heart_beat(Signal);
    }
}

void init() {
    TRISA = 0x01;
    TRISC = 0x80;
    PORTA = 0x00;
    PORTC = 0x00;
    ANSEL = 0x01;
    ANSELH = 0x00;
    ADCON0 = 0x01;
    ADCON1 = 0x80;
    EUSART1_Init(9600);
    TMR0 = 0;
    OPTION_REG = 0x07;
    INTCON = 0xA0;
    GIE = 1;
    PEIE = 1;
    T0IE = 1;
}

unsigned int ADC_Read(int adcChannel) {
    ADCON0 &= 0xC5;
    ADCON0 |= adcChannel << 3;
    __delay_ms(2);
    GO_nDONE = 1;
    while (GO_nDONE);
    return ((ADRESH << 8) + ADRESL);
}

void calculate_heart_beat(int adc_value) {
    if (adc_value < thresh && Pulse == 1) {
        Pulse = 0;
        amp = P - T;
        thresh = amp / 2 + T;
        P = thresh;
        T = thresh;
    }
    if (adc_value > thresh && Pulse == 0) {
        Pulse = 1;
        IBI = TMR0;
        TMR0 = 0;
        BPM = 60000 / IBI;
        EUSART1_Write(BPM);
    }
}

void __interrupt() ISR(void) {
    if (T0IF) {
        T0IF = 0;
    }
}
