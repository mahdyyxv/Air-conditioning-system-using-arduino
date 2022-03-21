////////////////////////////////////////////// Libraries & definition & variables //////////////////////////////////////////////

#include <LiquidCrystal.h>

#define Tpin A0
#define Up 7
#define Down 6
#define motor 5

LiquidCrystal lcd(8,9,10,11,12,13);

byte default_temp = 27;
byte wanted_temp = 27;
int Temprature;

////////////////////////////////////////////// Setup //////////////////////////////////////////////

void setup() {
  
  Init_interrupt();
  lcd.begin(16,2);
  Serial.begin(9600);
  
  pinMode(Up, INPUT_PULLUP);  
  pinMode(Down, INPUT_PULLUP);
  pinMode(motor, OUTPUT);
  
  lcd.print("Air Conditioning");
  lcd.setCursor(0,1);
  lcd.print("System");
  delay(100);
  lcd.clear();
  lcdWrite(Temprature , default_temp);

}

////////////////////////////////////////////// Interrupt Service Routine //////////////////////////////////////////////

ISR(TIMER1_COMPA_vect){
  
  Temprature = round( ( analogRead(Tpin)/1024.0)*500.0 );
  if( Temprature <= wanted_temp ) digitalWrite(motor, LOW);
  else digitalWrite(motor, HIGH);

}

////////////////////////////////////////////// Loop //////////////////////////////////////////////

void loop() {
  lcdWrite(Temprature , wanted_temp);
  if( digitalRead(Up) == 0 ){
    delay(10);
    if( digitalRead(Up) == 0 ){
      wanted_temp++;
      lcd.clear();
      while( digitalRead(Up) == 0 );
    }
  }
  
  if( digitalRead(Down) == 0 ){
    delay(10);
    if( digitalRead(Down) == 0 ){
      wanted_temp--;
      lcd.clear();
      while( digitalRead(Down) == 0 );
    }
  }
}

////////////////////////////////////////////// LCD write Routine //////////////////////////////////////////////

void lcdWrite(int RoomTemp, byte Temp){
  
  lcd.setCursor(0,0);
  lcd.print("Room Temp : ");
  lcd.print(RoomTemp);
  lcd.setCursor(0,1);
  lcd.print("Wanted Temp : ");
  lcd.print(Temp);
  
}

////////////////////////////////////////////// Initialization Of Interrupt //////////////////////////////////////////////

void Init_interrupt(){

  cli();
  TCCR1A = 0x80;
  TCCR1B = 0x09;
  OCR1AH = 0x1F; 
  OCR1AL = 0x3F;
  TIMSK1 = 0x02;
  sei();

}
