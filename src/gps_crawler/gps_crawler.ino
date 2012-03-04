#include <LiquidCrystal.h>
#include <Servo.h> 
#include <SoftwareSerial.h>

SoftwareSerial gps(12, 2);
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

// Create a Servo object for each servo
//Servo servo1;
//Servo motor1;

// User input for servo and position
int userInput[3];    // raw input from serial buffer, 3 bytes
int startbyte;       // start byte, begin reading input
int servo;           // which servo to pulse?
int pos;             // servo angle 0-180
int i;               // iterator

// define some values used by the panel and buttons
int lcd_key     = 0;
int adc_key_in  = 0;
#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5


void setup() 
{ 
  lcd.begin(16, 2);
  lcd.clear();
  lcd.print("Initializing...");
 
  gps.begin(4800);
  Serial.begin(19200);

//  servo1.attach(52);
//  motor1.attach(53);
  
//  arm();

  lcd.setCursor(0, 1);
  lcd.print(millis()/1000);
  lcd.print("s elapsed");
  

} 




void loop() 
{ 
  // Wait for serial input (min 3 bytes in buffer)
  
  
  
    if (gps.available())
//    Serial.write(gps.read());
     lcd.print(gps.read());
//    if (Serial.available())
//    gps.write(Serial.read());
  
  
//  if (Serial.available() > 2) {
    // Read the first byte
//    startbyte = Serial.read();
    // If it's really the startbyte (255) ...
//    if (startbyte == 255) {
      // ... then get the next two bytes
//      for (i=0;i<2;i++) {
//        userInput[i] = Serial.read();
//      }
      // First byte = servo to move?
//      servo = userInput[0];
      // Second byte = which position?
//      pos = userInput[1];
      // Packet error checking and recovery
//      if (pos == 255) { servo = 255; }

      // Assign new position to appropriate servo
//      switch (servo) {
//        case 1:
//          servo1.write(pos);    // move servo1 to 'pos'
//          break;
//        case 2:
//          motor1.write(pos);
//          break;
//      }

//    }
//  }

}


void arm()
{
  int speed;
//  motor1.write(5);
  for(i = 1; i <= 10; i+= 1)
  {
    speed = 55 + 50 * pow(-1,i);
//    motor1.write(speed);
    delay(1000);
  }    
}


void lcd_status()
{
  
}


// read the buttons
int read_LCD_buttons()
{
 adc_key_in = analogRead(0);      // read the value from the sensor 
 // my buttons when read are centered at these valies: 0, 144, 329, 504, 741
 // we add approx 50 to those values and check to see if we are close
 if (adc_key_in > 1000) return btnNONE; // We make this the 1st option for speed reasons since it will be the most likely result
 if (adc_key_in < 50)   return btnRIGHT;  
 if (adc_key_in < 195)  return btnUP; 
 if (adc_key_in < 380)  return btnDOWN; 
 if (adc_key_in < 555)  return btnLEFT; 
 if (adc_key_in < 790)  return btnSELECT;   
 return btnNONE;  // when all others fail, return this...
}



