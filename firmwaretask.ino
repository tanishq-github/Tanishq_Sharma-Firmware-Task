#include <Arduino_BuiltIn.h>  // included neccessary....
#include <EEPROM.h>           // Libraries........
#define F_CPU 16000000UL      // Macro for CPU Clock frequency in HZ.

void uart_init(unsigned long baud);            // UART Initialising function for given baud rate.
void eeprom_Write_byte(int addr, char* data);  // Function to write 8-Byte on given address.
uint8_t eeprom_Read_byte(int addr);            // Function to get 8-Byte data from given address.
template<class T>
void tx_char(T data);       // Function to transmit a character by UART.
char uart_rx();             // Function to receive a chcarcter by UART.
void tx_string(char* str);  // Function to transfer a String recieved on UART.
const int m = 100;          // Constant for maximum array length.
uint16_t ubrr = 0;          // Variable initialzed for UBRR register.
char c;                     // Variable for character value received during TX/RX.
char arr[m];                // Character array is initialzed.
int rx_index = 0;           // Array index value initialzed to 0.
int flag = 0;
float count = 0;

void setup() {
  // put your setup code here, to run once:
  uart_init(9600);           // UART is initialzed here.
  TIMSK1 &= ~(1 << OCIE1A);  // Disable interrupts during setup
  TCCR1A = 0;                // Timer1 control register A
  TCCR1B = 0;                // Timer1 control register B
  TCNT1 = 0;                 // Initialize counter value to 0
  OCR1A = 312499;            // Compare match register for 5 seconds (16MHz / (256 * 2Hz) - 1)
  TCCR1B |= (1 << WGM12);    // CTC mode
  TCCR1B |= (1 << CS12);     // 256 prescaler
  TIMSK1 |= (1 << OCIE1A);   // Enable Timer1 compare match A interrupt
  TIMSK1 |= (1 << OCIE1A);   // Enable interrupts
}

void loop() {
  // put your main code here, to run repeatedly:

  do {              // Do-while loop starts here........
    c = uart_rx();  // Polling for incoming data, when data available its stored in given variable.
    ++count;
    if (rx_index >= (m - 1) || c == '\r') {  // Condition for checking if character is next line or index exceeds maximum value.
      arr[rx_index] = '\0';                  // NULL character is stored in last index.

      rx_index = 0;  // Index variable is initialzed to 0 for starting for next incoming string.

      break;              // Loop is breaked to do further processing.
    } else {              // Else condition........
      arr[rx_index] = c;  // Character detected is stored in array on given index value.

      rx_index++;  // Index value is incremented.
    }
  } while (!(UCSR0A & (1 << RXC0)));  // Condition for checking incoming data.


  for (int i = 0; i < strlen(arr); i++) {  // Loop started for every character received for writing it to EEPROM starting address 0 and retreving it and transmitting back to Serial monitor to print.
    eeprom_Write_byte(i, arr);             // Writing to given address of EEPROM.
    arr[i] = 0;                            // Array index set to 0 for avoiding discreprancy.
    arr[i] = eeprom_Read_byte(i);          // Value read from given EEPROM adress.
  }
  tx_string(arr);  // Whole received string is transmitted and printed over Serial monitor.
}


void uart_init(unsigned long baud) {
  ubrr = (((F_CPU / (baud * 16UL))) - 1);
  UBRR0H = (uint8_t)(ubrr >> 8);
  UBRR0L = (uint8_t)ubrr;
  UCSR0B |= (1 << RXEN0) | (1 << TXEN0);
  UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

void eeprom_Write_byte(int addr, char* data) {
  while (EECR & (1 << EEPE))  // Waiting for completion of previous write operation.
    ;

  EEAR = (uint16_t)addr;  // Address is given to EEAR register.
  EEDR = *(data + addr);  // Data is written on EEDR register.

  EECR |= (1 << EEMPE);  // Writing operation is initiated.
  EECR |= (1 << EEPE);
}
uint8_t eeprom_Read_byte(int addr) {
  while (EECR & (1 << EEPE))  // Waiting for completion of previous write operation.
    ;
  EEAR = addr;          // Address is given to EEAR register.
  EECR |= (1 << EERE);  // Starting EEPROM read function.
  return EEDR;          // Value is returned from given address.
}

template<class T>
void tx_char(T data) {
  while (!(UCSR0A & (1 << UDRE0)))  // Waiting for previous transmission.
    ;

  UDR0 = data;  // UART data buffer is loaded for transmission.
}
void tx_string(char* str) {
  while (*str) {            // Loop condition until NULL character is found
    tx_char<char>(*str++);  // Transmitting character by character by UART.
  }
}
char uart_rx() {

  while (!(UCSR0A & (1 << RXC0)))  // Waiting for previous receive.
    ;
  while (!(UCSR0A & (1 << UDRE0)))  // Waiting for previous transmission.
    ;


  return UDR0;  // Received data is returned from UART data buffer.
}
ISR(TIMER1_COMPA_vect) {
  ++flag;
  if (flag == 5) {
    float rate = (count / 5) * 8;  // Calculating rate in 5 second intervals.
    tx_char<char>('\r');           // Next line.
    tx_string("Byte per second :");
    Serial.print(rate);  // Rate printed on Serial Monitor
    tx_char<char>('\n');
    count = 0;  // Counter initialzed to 0.
    flag = 0;
  }
}
