////////////////////////////////////////////////////////
//  USART_Basics
//
//  Date      : 26.11.2021
//  Author    : Christian Diekmann
//  Chip      : ATmega328P
//
///////////////////////////////////////////////////////

/*
   Features:
   ~ Transmit a string/character over UART
   ~ Receive a character over UART
   ~ Enable builtin LED on transmission error
*/

static char *message[] = {"Hello from Eindhoven! "};

void InitUART(){
  UCSR0B |= (1<<RXEN0) | (1<<TXEN0);            //enable Rx and Tx
  UCSR0B &= (~(1<<UCSZ02));                     //combined with the UCSZ[1:0] bits in UCSR0C sets the data lenght to 8bits
  
  UCSR0C &= (~(1<<UMSEL00)) & (~(1<<UMSEL01));  //set mode to Asynchronous
  UCSR0C &= (1<<UPM00) | (1<<UPM01);            //enable odd parity mode
  UCSR0C &= (~(1<<USBS0));                      //1 stop-bit when transmitting
  UCSR0C |= (1<<UCSZ00) | (1<<UCSZ01);          //8bits data lenght
  
  UBRR0 = 103;                                  //baudrate = 9600 bps with 16MHz clock
}

void TransmitChar(char c){
  while(!(UCSR0A & (1<<UDRE0))); //ensure transmit buffer is empty(ready)
  UDR0 = c;
}

unsigned char ReceiveChar(){
  while(!(UCSR0A & (1<<RXC0))); //while receive buffer is not full
  CheckError();
  return UDR0;
}

void TransmitString(char str[]){
  int i = 0;
  char ch;
  while(ch != '\0'){
        ch = str[i];
        TransmitChar(ch);
        i++;
  }
}

void CheckError(){ //toggles LED if the parity or frame error flags are set 
  if(UCSR0A & (1<<UPE0) || UCSR0A & (1<<FE0)){
    digitalWrite(LED_BUILTIN, HIGH);
  }
  else{
    digitalWrite(LED_BUILTIN, LOW);
  }
}

void setup() {
  InitUART();
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() { 
  TransmitString(*message);
  char test = ReceiveChar(); // waits here until bits are received
  delay(50);
  TransmitChar(test);
}
