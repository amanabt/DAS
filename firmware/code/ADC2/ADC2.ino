//#define SELPIN 10 //Selection Pin 
#define DATAOUT 11//MOSI 
#define DATAIN  12//MISO 
#define SPICLOCK  13//Clock 
int readvalue; 
const int conv_pin = 10;

void setup(){ 
 //set pin modes 
// pinMode(SELPIN, OUTPUT); 
 pinMode(DATAOUT, OUTPUT); 
 pinMode(DATAIN, INPUT); 
 pinMode(SPICLOCK, OUTPUT);
 pinMode(conv_pin, OUTPUT);
 //disable device to start with 
// digitalWrite(SELPIN,HIGH); 
 digitalWrite(DATAOUT,LOW); 
 digitalWrite(SPICLOCK,LOW); 
 digitalWrite (conv_pin, HIGH);
 Serial.begin(9600); 
} 

int read_adc(int channel){
  unsigned int adcvalue = 0;
//  byte commandbits = B11000000; //command bits - start, mode, chn (3), dont care (3)

  //allow channel selection
//  commandbits|=((channel-1)<<3);
  digitalWrite (conv_pin, HIGH);
  delayMicroseconds(2.2);
  digitalWrite (conv_pin, LOW);

//  digitalWrite(SELPIN,LOW); //Select adc
  // setup bits to be written
//  for (int i=7; i>=3; i--){
//    digitalWrite(DATAOUT,commandbits&1<<i);
//    //cycle clock
//    digitalWrite(SPICLOCK,HIGH);
//    digitalWrite(SPICLOCK,LOW);    
//  }

//  digitalWrite(SPICLOCK,HIGH);    //ignores 2 null bits
//  digitalWrite(SPICLOCK,LOW);
//  digitalWrite(SPICLOCK,HIGH);  
//  digitalWrite(SPICLOCK,LOW);

  //read bits from adc
  for (int i=16; i>0; i--){
    adcvalue+=digitalRead(DATAIN)<<i;
    //cycle clock
    digitalWrite(SPICLOCK,HIGH);
    digitalWrite(SPICLOCK,LOW);
  }
//  digitalWrite(SELPIN, HIGH); //turn off device
  return adcvalue;
}


void loop() { 
 readvalue = read_adc(1); 
 Serial.print(readvalue,DEC);
 Serial.print("\n"); 
 delayMicroseconds(250); 
} 
