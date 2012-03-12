#include <LiquidCrystal.h>
#include <Servo.h> 
#include <SoftwareSerial.h>
#include <TinyGPS.h>

TinyGPS gps;
SoftwareSerial ssp(12, 2);
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

int state       = 5;
static FILE lcdout = {0} ;      


void setup() 
{ 
  lcd.begin(16, 2);
  lcd.clear();
  lcd.print("Initializing...");
  ssp.begin(9600);
  Serial.begin(19200);
//  servo1.attach(52);
//  motor1.attach(53);
  fdev_setup_stream (&lcdout, lcd_putchar, NULL, _FDEV_SETUP_WRITE);
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
  
//  bool newdata = false;
//  unsigned long start = millis();
//  while (millis() - start < 1000)
//  {
    // spend 1 second listening to gps data and 
//    if (feedgps()) newdata = true;
//  }
//  gpsdump(gps);
//  Serial.print(ssp.read());
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


void menu_root()
{
  lcd.clear();
  lcd.print("Welcome.");
  delay(1000);
}

void menu_arm()
{
  boolean done = false;
  lcd.clear();
  lcd.print("Arm ESC?");
  while (! done)
  {
    if (get_button() == btnSELECT) {  
      lcd.setCursor(0, 1);
      lcd.print("done.");
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
  while (! done)
  {
    lcd.setCursor(11, 1);
    lcd.print(" ");
    delay(800);
    lcd.setCursor(11, 1);
    lcd.print(".");
    lcd.setCursor(12, 1);
    lcd.print(" ");
    delay(800);
    lcd.setCursor(12, 1);
    lcd.print(".");
    lcd.setCursor(13, 1);
    lcd.print(" ");
    delay(800);
    lcd.setCursor(13, 1);
    lcd.print(".");
  }
}

void menu_lat_lon()
{
    boolean done = false;
    float lat, lon;
    unsigned long age;
    char buf[32];
    unsigned long start = millis();
    while (millis() - start < 1000)
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
        lcd.print(lon);
      }
   }
}

static void menu_sats()
{
    boolean done = false;
    lcd.setCursor(0, 0);
    lcd.print("Satellites: ");
    lcd.print(gps.satellites());
    lcd.setCursor(0, 1);
    lcd.print("HDOP: ");
    lcd.print(gps.hdop());
}

static void menu_time()
{
  boolean done = false;
  while (! done)
  {
    if (get_button() == btnSELECT) {
    }
  }
}

static void menu_manual()
{
  boolean done = false;
    while (! done)
  {
    if (get_button() == btnSELECT) {
    }
  }
}

static void menu_go_waypoint()
{
  boolean done = false;
    while (! done)
  {
    if (get_button() == btnSELECT) {
    }
  }
}

static void menu_next_waypoint()
{
  boolean done = false;
    while (! done)
  {
    if (get_button() == btnSELECT) {
    }
  }
}

static void menu_moving()
{
  boolean done = false;
  while (! done)
  {
    if (get_button() == btnSELECT) {
    }
  }
}

static void gpsdump(TinyGPS &gps)
{
  float flat, flon;
  unsigned long age, date, time, chars = 0;
  unsigned short sentences = 0, failed = 0;
  static const float LONDON_LAT = 51.508131, LONDON_LON = -0.128002;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Satellites: ");
  lcd.print(gps.satellites());
  lcd.setCursor(0, 1);
  print_int(gps.satellites(), TinyGPS::GPS_INVALID_SATELLITES, 5);
  print_int(gps.hdop(), TinyGPS::GPS_INVALID_HDOP, 5);
  gps.f_get_position(&flat, &flon, &age);
  print_float(flat, TinyGPS::GPS_INVALID_F_ANGLE, 9, 5);
  print_float(flon, TinyGPS::GPS_INVALID_F_ANGLE, 10, 5);
  print_int(age, TinyGPS::GPS_INVALID_AGE, 5);
  print_date(gps);
  Serial.println();
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



// read the buttons
int get_button()
{
  int key_in = analogRead(0);      // read the value from the sensor 
  // my buttons when read are centered at these valies: 0, 144, 329, 504, 741
  // we add approx 50 to those values and check to see if we are close
  if (key_in > 1000) return btnNONE; // We make this the 1st option for speed reasons since it will be the most likely result
  if (key_in < 50)   return btnRIGHT;  
  if (key_in < 195)  return btnUP; 
  if (key_in < 380)  return btnDOWN; 
  if (key_in < 555)  return btnLEFT; 
  if (key_in < 790)  return btnSELECT;   
  return btnNONE;  // when all others fail, return this...
}




static void print_int(unsigned long val, unsigned long invalid, int len)
{
  char sz[32];
  if (val == invalid)
    strcpy(sz, "*******");
  else
    sprintf(sz, "%ld", val);
  sz[len] = 0;
  for (int i=strlen(sz); i<len; ++i)
    sz[i] = ' ';
  if (len > 0) 
    sz[len-1] = ' ';
  Serial.print(sz);
  feedgps();
}

static void print_float(float val, float invalid, int len, int prec)
{
  char sz[32];
  if (val == invalid)
  {
    strcpy(sz, "*******");
    sz[len] = 0;
        if (len > 0) 
          sz[len-1] = ' ';
    for (int i=7; i<len; ++i)
        sz[i] = ' ';
    Serial.print(sz);
  }
  else
  {
    Serial.print(val, prec);
    int vi = abs((int)val);
    int flen = prec + (val < 0.0 ? 2 : 1);
    flen += vi >= 1000 ? 4 : vi >= 100 ? 3 : vi >= 10 ? 2 : 1;
    for (int i=flen; i<len; ++i)
      Serial.print(" ");
  }
  feedgps();
}

static void print_date(TinyGPS &gps)
{
  int year;
  byte month, day, hour, minute, second, hundredths;
  unsigned long age;
  gps.crack_datetime(&year, &month, &day, &hour, &minute, &second, &hundredths, &age);
  if (age == TinyGPS::GPS_INVALID_AGE)
    Serial.print("*******    *******    ");
  else
  {
    char sz[32];
    sprintf(sz, " %02d:%02d:%02d ",
        hour, minute, second);
    lcd.print(sz);
  }
  feedgps();
}

static void print_str(const char *str, int len)
{
  int slen = strlen(str);
  for (int i=0; i<len; ++i)
    Serial.print(i<slen ? str[i] : ' ');
  feedgps();
}

static bool feedgps()
{
  while (ssp.available())
  {
    if (gps.encode(ssp.read()))
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
  long whole = (long) val;
  itoa (whole, buf, 10);
  while (*buf != '\0') buf++;
  *buf++ = '.';
  long decimal = abs((long)((val - whole) * 100000));
  itoa(decimal, buf, 10);
  return ret;
}


