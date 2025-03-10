#include <htc.h>

#define _XTAL_FREQ 20000000  // 20MHz Crystal Frequency
#define rs RC0
#define rw RC1
#define en RC2

// Function Declarations
void ser_int();
void tx(unsigned char);
void send(unsigned char *);
void lcd_init();
void cmd(unsigned char);
void dat(unsigned char);
void show(unsigned char *);
void monitor(unsigned char, unsigned char *);
void clrscr();
void adc();

__CONFIG(FOSC_HS & WDTE_OFF & PWRTE_OFF & CP_OFF & BOREN_ON & LVP_OFF & CPD_OFF & WRT_OFF & DEBUG_OFF);

void main() {
    // Pin Configurations
    TRISC6 = 0;  // TX (Output)
    TRISC7 = 1;  // RX (Input)
    TRISB = 0x00; // PORTB as output for LCD
    TRISC0 = TRISC1 = TRISC2 = 0; // RS, RW, EN as outputs
    TRISA0 = 1;  // Set RA0 as ADC input

    ser_int();  // Initialize USART
    lcd_init(); // Initialize LCD
    
    show("ADC Value:");
    
    while(1) {
        cmd(0x8C);  // Move cursor to ADC value position
        show("    ");  // Clear previous value
        cmd(0x8C);  // Reset cursor position
        
        adc();  // Read and display ADC value
        __delay_ms(500);  // Wait before next ADC read
    }
}

// ADC Function
void adc() {
    unsigned int adcval;
    
    ADCON1 = 0xC0;  // Right justified, Vref = Vdd
    ADCON0 = 0x85;  // ADC ON, Fosc/64, Select AN0
    
    __delay_ms(2);  // Stabilization delay
    
    GO_nDONE = 1;  // Start conversion
    while(GO_nDONE);  // Wait for completion
    
    adcval = ((ADRESH << 8) | ADRESL);  // Store 10-bit ADC result
    
    // Convert ADC value to displayable digits
    dat((adcval / 1000) + '0');
    dat(((adcval / 100) % 10) + '0');
    dat(((adcval / 10) % 10) + '0');
    dat((adcval % 10) + '0');
    
    // Send ADC value via USART
    tx((adcval / 1000) + '0');
    tx(((adcval / 100) % 10) + '0');
    tx(((adcval / 10) % 10) + '0');
    tx((adcval % 10) + '0');
    tx('\n'); // New line
}

// USART Initialization
void ser_int() {
    TXSTA = 0b00100100;  // BRGH=1, TXEN=1, Async mode
    RCSTA = 0b10010000;  // Serial Port enabled, 8-bit reception
    SPBRG = 129;  // For 20MHz, use SPBRG = 129 (9600 baud)
}

// Transmit Character
void tx(unsigned char a) {
    while (!TXIF);  // Wait for TX buffer to be empty
    TXREG = a;
}

// Send String via USART
void send(unsigned char *s) {
    while (*s) {
        tx(*s++);
    }
}

// LCD Initialization
void lcd_init() { 
    __delay_ms(20);  // Wait for LCD to power up
    cmd(0x38); // 8-bit mode, 2 lines, 5x7 matrix
    cmd(0x0C); // Display ON, Cursor OFF
    cmd(0x06); // Auto-increment cursor
    cmd(0x80); // Move cursor to first line
}

// Send Command to LCD
void cmd(unsigned char a) {
    PORTB = a;
    rs = 0;  // Command mode
    rw = 0;  // Write mode
    en = 1;
    __delay_ms(2);
    en = 0;
}

// Send Data to LCD
void dat(unsigned char b) {
    PORTB = b;
    rs = 1;  // Data mode
    rw = 0;  // Write mode
    en = 1;
    __delay_ms(2);
    en = 0;
}

// Display String on LCD
void show(unsigned char *s) {
    while(*s) {
        dat(*s++);
    }
}

// Print Text on LCD at Specific Line
void monitor(unsigned char line, unsigned char *text) {
    if(line == 0)
        cmd(0x80);  // Move to first line
    else
        cmd(0xC0);  // Move to second line

    show(text);
}

// Clear LCD Screen
void clrscr() {
    cmd(0x01);
    __delay_ms(2);
}
