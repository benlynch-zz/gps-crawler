#include <LiquidCrystal.h>
#include <Servo.h> 
#include <SoftwareSerial.h>
#include <TinyGPS.h>

TinyGPS gps;
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

// Create a Servo object for each servo
Servo servo1;
Servo motor1;

// User input for servo and position
int userInput[3];    // raw input from serial buffer, 3 bytes
int startbyte;       // start byte, begin reading input
int servo;           // which servo to pulse?
int pos;             // servo angle 0-180
int i;               // iterator

// define some values used by the panel and buttons
#define btnNONE   0
#define btnRIGHT  1
#define btnUP     2
#define btnDOWN   3
#define btnLEFT   4
#define btnSELECT 5

int state       = 0;
static FILE lcdout = {0} ;      

void setup() 
{ 
  lcd.begin(16, 2);
  lcd.clear();
  lcd.print("Initializing...");
  
  Serial1.begin(9600);  // GSP serial comm
  Serial.begin(57600);  // XBee wireless comm

  servo1.attach(22);
  motor1.attach(28);    // ESC for crawler
  
  fdev_setup_stream (&lcdout, lcd_putchar, NULL, _FDEV_SETUP_WRITE);

  byte power[8] = {     // custom character for display
    B11111,
    B11111,
    B11111,
    B11111,
    B11111,
    B11111,
    B11111,
  };
  
  lcd.createChar(0, power);
  lcd.setCursor(0, 1);
  lcd.print(millis()/1000);
  lcd.print("s elapsed");
} 

void loop() 
{
      switch (state) {
        case 0:
          delay(1000);
          state++;
          break;
        case 1:
          menu_root();
          break;
        case 2:
          menu_arm();
          break;
        case 3:
          menu_searching();
          break;
        case 4:
          menu_ask_manual();
          break;
        case 5:
          menu_lat_lon();
          break;
        case 6:
          menu_sats();
          break;
        case 7:
          menu_time();
          break;
        case 8:
          menu_manual();
          break;
        case 9:
          menu_go_waypoint();
          break;
        case 10:
          menu_next_waypoint();
          break;
        case 11:
          menu_moving();
          break;
      }
}


void menu_root()
{
  lcd.clear();
  lcd.print("Welcome.");
  delay(1000);
  state = 2;
}

void menu_arm()
{
  boolean done = false;
  lcd.clear();
  lcd.print("Arm ESC?");
  delay(500);      // delay to prevent a single button click
                   // from jumping through multiple menus
  while (! done)
  {
    if (get_button() == btnSELECT) {  
      lcd.setCursor(0, 1);
      arm();
      lcd.print("done.");
    } 
    if (get_button() == btnDOWN) {  
      state = 3;
      done = true;
    } 
  }
}

void menu_searching()
{
  boolean done = false;
  lcd.clear();
  lcd.print("Searching for");
  lcd.setCursor(1, 1);
  lcd.print("satellites...");
  unsigned long start = millis();

  delay(500);
  while (! done)
  {
    if (gps.satellites() != 255)
    {
      state = 5;
      done = true;
    }

    lcd_blink(start,millis(),11,1);  //3 blinking dots on lcd 
      
    int butt = get_button();
    if (butt == btnRIGHT) {  
      state = 5;
      done = true;
    } 
    if (butt == btnUP) {  
      state = 2;
      done = true;
    } 
    if (butt == btnDOWN) {  
      state = 4;
      done = true;
    } 
  }
}



void menu_ask_manual()
{
  boolean done = false;
  lcd.clear();
  lcd.print("Enter Manual");
  lcd.setCursor(1, 1);
  lcd.print(" Mode?");
  
  delay(500);
  while (! done)
  {
    int butt = get_button();
    if (butt == btnSELECT) {  
      state = 8;
      done = true;
    } 
    if (butt == btnDOWN) {  
      state = 3;
      done = true;
    } 
    if (butt == btnUP) {  
      state = 3;
      done = true;
    } 
    if (butt == btnLEFT) {  
      state = 3;
      done = true;
    } 
    if (butt == btnRIGHT) {  
      state = 3;
      done = true;
    } 
  }
}


void menu_lat_lon()
{
    boolean done = false;
    float lat, lon;
    unsigned long age;
    char buf[32];

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Lat: ********");
    lcd.setCursor(0, 1);
    lcd.print("Lon: ********");
    delay(5000);
    
    unsigned long start = millis();
    while (millis() - start < 2000)
    {
      if (feedgps()){
        gps.f_get_position(&lat, &lon, &age);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Lat:  ");
        ftoa(buf, lat);
        lcd.print(buf);
        lcd.setCursor(0, 1);
        lcd.print("Lon: ");
        ftoa(buf, lon);
        lcd.print(buf);
      }
   }
}

void menu_sats()
{
    boolean done = false;
    lcd.setCursor(0, 0);
    lcd.print("Satellites: ");
    lcd.print(gps.satellites());
    lcd.setCursor(0, 1);
    lcd.print("HDOP: ");
    lcd.print(gps.hdop());
    delay(500);
}

void menu_time()
{
  boolean done = false;
  int year;
  byte month, day, hour, minute, second, hundredths;
  unsigned long age;
  char sz[32];
  gps.crack_datetime(&year, &month, &day, &hour, &minute, &second, &hundredths, &age);
  sprintf(sz, " %02d:%02d:%02d ", hour, minute, second);
  lcd.print(sz);
  while (! done)
  {
    if (get_button() == btnSELECT) {
    }
  }
}

void menu_manual()
{
  boolean done = false;

  lcd.clear();
  lcd.print("MANUAL MODE!");
  lcd.setCursor(0, 1);
  lcd.print("SPD:");
  lcd.write((byte)0);
  lcd.write((byte)0);
  lcd.write((byte)0);
  lcd.print("   DIR:RT");
  delay(500);  

  int svals[11] = {5,10,15,20,25,30,35,40,45,50,55};
  int tvals[11] = {5,10,15,20,25,30,35,40,45,50,55};

  int steer     = 6;
  int throttle  = 6;
  int nsteer    = 6;
  int nthrottle = 6;

  while (! done)
  {
    if (Serial.available() > 2) 
    {
      startbyte = Serial.read();
      if (startbyte == 255)
      {
        nsteer    = Serial.read();
        nthrottle = Serial.read();
      }  
    }  
    if (nsteer != steer)
    {
      if (nsteer > 0 && nsteer < 11)
      {
        steer = nsteer;
        servo1.write(svals[steer]);
      } 
    }
    if (nthrottle != throttle)
    {
      if (nthrottle > 0 && nthrottle < 11)
      {
        throttle = nthrottle;
        motor1.write(svals[throttle]);
      } 
    }
    int butt = get_button();
    if (butt == btnLEFT | butt == btnRIGHT | butt == btnUP | butt == btnDOWN | butt == btnSELECT ){  
      state = 4;
      done = true;
    } 
  }
}

void menu_go_waypoint()
{
  boolean done = false;
    while (! done)
  {
    if (get_button() == btnSELECT) {
    }
  }
}

void menu_next_waypoint()
{
  boolean done = false;
    while (! done)
  {
    if (get_button() == btnSELECT) {
    }
  }
}

void menu_moving()
{
  boolean done = false;
  while (! done)
  {
    if (get_button() == btnSELECT) {
    }
  }
}

// Arm the electronic speed control
void arm()
{
  int speed;
  motor1.write(5);
  delay(5000);  
}

// read the buttons
int get_button()
{
  int key_in = analogRead(0);      // read the value from the sensor 
  // my buttons when read are centered at these valies: 
  //  RT 0, UP 142, DWN 328, LFT 503, SEL 741
  // we add approx 50 to those values and check to see if we are close
   
  if (key_in > 1000) return btnNONE; // We make this the 1st option for speed reasons since it will be the most likely result
  if (key_in < 50)   return btnRIGHT;  
  if (key_in < 195)  return btnUP; 
  if (key_in < 380)  return btnDOWN; 
  if (key_in < 555)  return btnLEFT; 
  if (key_in < 790)  return btnSELECT;   
  return btnNONE;  // when all others fail, return this...
}

static bool feedgps()
{
  while (Serial1.available())
  {
      if (gps.encode(Serial1.read()))
      return true;
  }
  return false;
}

// LCD character writer
static int lcd_putchar(char ch, FILE* stream)
{
    lcd.write(ch) ;
    return (0) ;
}

static char * ftoa (char *buf, float val )
{
  char *ret = buf;
  int i;
  long whole = (long) val;
  itoa (whole, buf, 10);
  while (*buf != '\0') buf++;
  *buf++ = '.';
  long decimal = abs((long)((val - whole) * 100000));
  for (int i=0; i<5; ++i) {
    if (decimal < pow(10,i))
    {
      *buf++='0';
    }
  }
  itoa(decimal, buf, 10);
  return ret;
}

void lcd_blink ( unsigned long start, unsigned long now, int y, int x )
{
  unsigned long seconds;
  seconds = ( now - start ) / 1000;
  if ( ( (now - start) % 1000 ) < 50 )  // only update during the first 20th of a second
  {
    if ( seconds % 3 == 0 )
    {
      lcd.setCursor(y, x);
      lcd.print(" ");
      lcd.setCursor(y+2, x);
      lcd.print(".");
    }
    if ( seconds % 3 == 1 )
    {
      lcd.setCursor(y, x);
      lcd.print(".");
      lcd.setCursor(y+1, x);
      lcd.print(" ");
    }
    if ( seconds % 3 == 2 )
    {
      lcd.setCursor(y+1, x);
      lcd.print(".");
      lcd.setCursor(y+2, x);
      lcd.print(" ");
    }
  }
}
