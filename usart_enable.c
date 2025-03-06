//#include <pic.h>
//
//__CONFIG(FOSC_HS & WDTE_OFF & PWRTE_OFF & CP_OFF & BOREN_ON & LVP_OFF & CPD_OFF & WRT_OFF & DEBUG_OFF);
//
//#define rs RD2
//#define en RD3
//#define LED RD0
//#define R1 RB0
//#define R2 RB1
//#define R3 RB2
//#define R4 RB3
//#define C1 RB4
//#define C2 RB5
//#define C3 RB6
//#define C4 RB7
//
//void lcd_init();
//void cmd(unsigned char a);
//void dat(unsigned char b);
//void show(unsigned char *s);
//void lcd_delay();
//void EEPROM_Write(unsigned char address, unsigned char data);
//unsigned char EEPROM_Read(unsigned char address);
//
//unsigned char key();
//void keyinit();
//
//unsigned char keypad[4][4] = {
//    {'7', '8', '9', '/'},
//    {'4', '5', '6', '*'},
//    {'1', '2', '3', '-'},
//    {'C', '0', '=', '+'}
//};    
//
//unsigned char rowloc, colloc;
//char inputBuffer[7];  // Stores up to 6 digits + null terminator
//unsigned char index = 0;
//
//void patientAdd() {
//    TRISD0 = 0;
//    LED = 1;
//    TRISD = 0;
//    lcd_init();
//    keyinit();
//    
//    unsigned char b;
//    cmd(0x80);
//    show("Patient_ID: ");
//    
//    while (1) {
//        if (index < 6) {
//            cmd(0xC7 + index); // Move cursor dynamically
//            b = key();
//            
//            if (b == '=') {
//                // Store value in EEPROM and reset buffer
//                for (unsigned char i = 0; i < index; i++) {
//                    EEPROM_Write(i, inputBuffer[i]);
//                }
//                EEPROM_Write(index, '\0'); // Null-terminate in EEPROM
//                show(" Saved! ");
//                break;
//            } else if (b >= '0' && b <= '9') {
//                inputBuffer[index] = b;
//                dat(b);
//                index++;
//            }
//        }
//    }
//}
//
//void main() {
//    TRISA0 = 1;  // Set RA0 as input
//    while (1) {
//        if (RA0 == 0) { // Button press
//            patientAdd();
//        }
//    }
//}
//
//// LCD Functions
//void lcd_init() {
//    cmd(0x02);
//    cmd(0x28);
//    cmd(0x0e);
//    cmd(0x06);
//    cmd(0x80);
//}
//
//void cmd(unsigned char a) {
//    rs = 0;
//    PORTD &= 0x0F;
//    PORTD |= (a & 0xf0);
//    en = 1;
//    lcd_delay();
//    en = 0;
//    lcd_delay();
//    PORTD &= 0x0f;
//    PORTD |= (a << 4 & 0xf0);
//    en = 1;
//    lcd_delay();
//    en = 0;
//    lcd_delay();
//}
//
//void dat(unsigned char b) {
//    rs = 1;
//    PORTD &= 0x0F;
//    PORTD |= (b & 0xf0);
//    en = 1;
//    lcd_delay();
//    en = 0;
//    lcd_delay();
//    PORTD &= 0x0f;
//    PORTD |= (b << 4 & 0xf0);
//    en = 1;
//    lcd_delay();
//    en = 0;
//    lcd_delay();
//}
//
//void show(unsigned char *s) {
//    while (*s) {
//        dat(*s++);
//    }
//}
//
//void lcd_delay() {
//    unsigned int lcd_delay;
//    for (lcd_delay = 0; lcd_delay <= 1000; lcd_delay++);
//}
//
//// Keypad Functions
//void keyinit() {
//    TRISB = 0XF0;
//    OPTION_REG &= 0X7F; // ENABLE PULL-UP
//}
//
//unsigned char key() {
//    PORTB = 0X00;
//    while (C1 && C2 && C3 && C4); // Wait for key press
//
//    while (!C1 || !C2 || !C3 || !C4) {
//        R1 = 0; R2 = R3 = R4 = 1;
//        if (!C1 || !C2 || !C3 || !C4) { rowloc = 0; break; }
//        
//        R2 = 0; R1 = 1;
//        if (!C1 || !C2 || !C3 || !C4) { rowloc = 1; break; }
//        
//        R3 = 0; R2 = 1;
//        if (!C1 || !C2 || !C3 || !C4) { rowloc = 2; break; }
//        
//        R4 = 0; R3 = 1;
//        if (!C1 || !C2 || !C3 || !C4) { rowloc = 3; break; }
//    }
//
//    if (C1 == 0 && C2 != 0 && C3 != 0 && C4 != 0) colloc = 0;
//    else if (C1 != 0 && C2 == 0 && C3 != 0 && C4 != 0) colloc = 1;
//    else if (C1 != 0 && C2 != 0 && C3 == 0 && C4 != 0) colloc = 2;
//    else if (C1 != 0 && C2 != 0 && C3 != 0 && C4 == 0) colloc = 3;
//
//    while (C1 == 0 || C2 == 0 || C3 == 0 || C4 == 0); // Wait for release
//    return keypad[rowloc][colloc];
//}
//
//// EEPROM Functions
//void EEPROM_Write(unsigned char address, unsigned char data) {
//    EEADR = address;     // Set address
//    EEDATA = data;       // Set data
//    EECON1bits.EEPGD = 0; // Access EEPROM memory
//    EECON1bits.WREN = 1;  // Enable writing
//    INTCONbits.GIE = 0;   // Disable interrupts
//    
//    // Required sequence to write EEPROM
//    EECON2 = 0x55;
//    EECON2 = 0xAA;
//    EECON1bits.WR = 1;
//
//    while (EECON1bits.WR); // Wait for write to complete
//    EECON1bits.WREN = 0;  // Disable writing
//    INTCONbits.GIE = 1;   // Enable interrupts
//}
//
//unsigned char EEPROM_Read(unsigned char address) {
//    EEADR = address;      // Set address
//    EECON1bits.EEPGD = 0; // Access EEPROM memory
//    EECON1bits.RD = 1;    // Read data
//    return EEDATA;
//}

#include <htc.h>

__CONFIG(FOSC_HS & WDTE_OFF & PWRTE_OFF & CP_OFF & BOREN_ON & LVP_OFF & CPD_OFF & WRT_OFF & DEBUG_OFF);

void ser_int();
void tx(unsigned char);
unsigned char rx();
void send(unsigned char *);

void main() {
    TRISC6 = 0;  // TX (Output)
    TRISC7 = 1;  // RX (Input)
    ser_int();
    
}

void ser_int() {
    TXSTA = 0b00100100;  // BRGH=1, TXEN=1, Async mode
    RCSTA = 0b10010000;  // Serial Port enabled, 8-bit reception
    SPBRG = 129;  // For 20MHz, use SPBRG = 129 (9600 baud)
    TXIF = 0; 
    RCIF = 0;
}

void tx(unsigned char a) {
    while (!TXIF);  // Wait for TX buffer to be empty
    TXREG = a;
}

unsigned char rx() {
    while (!RCIF);  // Wait for received data
    return RCREG;
}

void send(unsigned char *s) {
    while (*s) {
        tx(*s++);
    }
    
    while(1) {
        tx(rx());  // Echo received characters
    }
    
}
