#include <htc.h>

#pragma config FOSC = HS
#pragma config WDTE = OFF
#pragma config PWRTE = OFF
#pragma config BOREN = OFF
#pragma config LVP = OFF
#pragma config CPD = OFF
#pragma config WRT = OFF
#pragma config CP = OFF

#define _XTAL_FREQ 20000000
#define I2C_BaudRate 100000

#define LCD_BACKLIGHT 0x08
#define LCD_COMMAND 0
#define LCD_DATA 1

unsigned char I2C_address = 0x4E;
unsigned char BackLight_State = LCD_BACKLIGHT;

// Function Prototypes
void ser_int();
void tx(unsigned char);
void send(unsigned char *);
void adc(unsigned int *);
void I2C_Master_Init(void);
void I2C_Master_Start(void);
void I2C_Master_Stop(void);
void I2C_Master_Wait(void);
void I2C_Master_Write(unsigned char data);
void IO_Expander_Write(unsigned char data);
void LCD_Write_4Bit(unsigned char nibble);
void LCD_CMD(unsigned char cmd);
void LCD_Char(char data);
void LCD_Init(void);
void LCD_String(char *str);
void LCD_Set_Cursor(unsigned char row, unsigned char col);
void monitor(unsigned char, char *);

void main() {
    unsigned int heart_rate = 0;
    unsigned long int time_elapsed = 0;

    TRISC6 = 0;
    TRISC7 = 1;
    TRISA0 = 1;

    ser_int();
    I2C_Master_Init();
    LCD_Init();

    LCD_Set_Cursor(1, 1);
    LCD_String("BPM: ");

    while(1) {
        heart_rate = 0;
        time_elapsed = 0;

        while (time_elapsed < 15000) {
            adc(&heart_rate);
            __delay_ms(10);
            time_elapsed += 10;
        }

        heart_rate *= 4;

        LCD_Set_Cursor(1, 6);  // Move to after "BPM: "
        LCD_String("    ");    // Clear old value
        LCD_Set_Cursor(1, 6);

        LCD_Char((heart_rate / 100) + '0');
        LCD_Char(((heart_rate / 10) % 10) + '0');
        LCD_Char((heart_rate % 10) + '0');

        // Send BPM via USART
        tx((heart_rate / 100) + '0');
        tx(((heart_rate / 10) % 10) + '0');
        tx((heart_rate % 10) + '0');
        tx('\n');

        if (heart_rate > 100) {
            monitor(2, "Alert!");
        }
    }
}

// ADC Function
void adc(unsigned int *beat_count) {
    static unsigned int last_value = 0;
    unsigned int adcval;

    ADCON1 = 0xC0;
    ADCON0 = 0x85;

    __delay_ms(2);
    GO_nDONE = 1;
    while (GO_nDONE);

    adcval = ((ADRESH << 8) | ADRESL);

    if (last_value < 200 && adcval > 1000) {
        (*beat_count)++;
        RB0 = 1;
    }

    last_value = adcval;
}

// USART Initialization
void ser_int() {
    TXSTA = 0b00100100;
    RCSTA = 0b10010000;
    SPBRG = 129;
}

void tx(unsigned char a) {
    while (!TXIF);
    TXREG = a;
}

void send(unsigned char *s) {
    while (*s) {
        tx(*s++);
    }
}

// I2C + LCD Functions
void I2C_Master_Init(void) {
    SSPCON = 0x28;
    SSPCON2 = 0x00;
    SSPADD = (_XTAL_FREQ / (4 * I2C_BaudRate)) - 1;
    SSPSTAT = 0x00;
    TRISC3 = 1;
    TRISC4 = 1;
}

void I2C_Master_Wait(void) {
    while ((SSPSTAT & 0x04) || (SSPCON2 & 0x1F));
}

void I2C_Master_Start(void) {
    I2C_Master_Wait();
    SEN = 1;
}

void I2C_Master_Stop(void) {
    I2C_Master_Wait();
    PEN = 1;
}

void I2C_Master_Write(unsigned char data) {
    I2C_Master_Wait();
    SSPBUF = data;
    while (!SSPIF);
    SSPIF = 0;
}

void IO_Expander_Write(unsigned char data) {
    I2C_Master_Start();
    I2C_Master_Write(I2C_address);
    I2C_Master_Write(data | BackLight_State);
    I2C_Master_Stop();
}

void LCD_Write_4Bit(unsigned char nibble) {
    IO_Expander_Write(nibble | 0x04);  // EN = 1
    __delay_us(1);
    IO_Expander_Write(nibble & ~0x04); // EN = 0
    __delay_us(50);
}

void LCD_CMD(unsigned char cmd) {
    unsigned char high = cmd & 0xF0;
    unsigned char low = (cmd << 4) & 0xF0;
    LCD_Write_4Bit(high | 0x00); // RS = 0
    LCD_Write_4Bit(low | 0x00);
}

void LCD_Char(char data) {
    unsigned char high = data & 0xF0;
    unsigned char low = (data << 4) & 0xF0;
    LCD_Write_4Bit(high | 0x01); // RS = 1
    LCD_Write_4Bit(low | 0x01);
}

void LCD_Init(void) {
    __delay_ms(50);
    LCD_CMD(0x03);
    __delay_ms(5);
    LCD_CMD(0x03);
    __delay_ms(5);
    LCD_CMD(0x03);
    __delay_ms(5);
    LCD_CMD(0x02);
    __delay_ms(1);
    LCD_CMD(0x28);
    LCD_CMD(0x0C);
    LCD_CMD(0x06);
    LCD_CMD(0x01);
    __delay_ms(2);
}

void LCD_String(char *str) {
    while (*str) {
        LCD_Char(*str++);
    }
}

void LCD_Set_Cursor(unsigned char row, unsigned char col) {
    unsigned char pos[] = {0x80, 0xC0, 0x94, 0xD4};
    LCD_CMD(pos[row - 1] + col - 1);
}

void monitor(unsigned char line, char *text) {
    LCD_Set_Cursor(line, 1);
    LCD_String("        "); // Clear line
    LCD_Set_Cursor(line, 1);
    LCD_String(text);
}
