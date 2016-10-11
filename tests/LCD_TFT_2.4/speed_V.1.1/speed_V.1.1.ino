// IMPORTANT: Adafruit_TFTLCD LIBRARY MUST BE SPECIFICALLY
// CONFIGURED FOR EITHER THE TFT SHIELD OR THE BREAKOUT BOARD.
// SEE RELEVANT COMMENTS IN Adafruit_TFTLCD.h FOR SETUP.

#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_TFTLCD.h> // Hardware-specific library

// The control pins for the LCD can be assigned to any digital or
// analog pins...but we'll use the analog pins as this allows us to
// double up the pins with the touch screen (see the TFT paint example).
#define LCD_CS A3 // Chip Select goes to Analog 3
#define LCD_CD A2 // Command/Data goes to Analog 2
#define LCD_WR A1 // LCD Write goes to Analog 1
#define LCD_RD A0 // LCD Read goes to Analog 0

#define LCD_RESET A4 // Can alternately just connect to Arduino's reset pin

// When using the BREAKOUT BOARD only, use these 8 data lines to the LCD:
// For the Arduino Uno, Duemilanove, Diecimila, etc.:
//   D0 connects to digital pin 8  (Notice these are
//   D1 connects to digital pin 9   NOT in order!)
//   D2 connects to digital pin 2
//   D3 connects to digital pin 3
//   D4 connects to digital pin 4
//   D5 connects to digital pin 5
//   D6 connects to digital pin 6
//   D7 connects to digital pin 7
// For the Arduino Mega, use digital pins 22 through 29
// (on the 2-row header at the end of the board).

// Assign human-readable names to some common 16-bit color values:
#define	BLACK   0xFFFF
#define	BLUE    0xFA00
#define	RED     0x3F33
#define	GREEN   0xC33A
#define CYAN    0xF800
#define MAGENTA 0x07E0
#define YELLOW  0x3133
#define WHITE   0x0000
#define rotation 1

Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);
// If using the shield, all control and data lines are fixed, and
// a simpler declaration can optionally be used:
// Adafruit_TFTLCD tft;
long previousMillis = 0;   // здесь будет храниться время последнего изменения состояния светодиода 
long interval = 1000;      // интервал мигания в миллисекундах
long pred;
void setup(void) {
  Serial.begin(9600);
  Serial.println(F("TFT LCD test"));

#ifdef USE_ADAFRUIT_SHIELD_PINOUT
  Serial.println(F("Using Adafruit 2.8\" TFT Arduino Shield Pinout"));
#else
  Serial.println(F("Using Adafruit 2.8\" TFT Breakout Board Pinout"));
#endif

  Serial.print("TFT size is "); Serial.print(tft.width()); Serial.print("x"); Serial.println(tft.height());

  tft.reset();
  delay(500);
  tft.begin(0x9341);

  //настройки экрана
 
  tft.setRotation(rotation);  //поворот
  tft.setCursor(80, 80);      //координаты
  tft.setTextColor(RED);      //цвет текста   
  tft.setTextSize(10);        //размер текста
  tft.fillScreen(BLACK);      //заливка
}

void loop(void) {
  long X;
  for (long X = 192; X < 212; X ++){
      if (X < 10){
        tft.setCursor(140,80);  
        Text(X);  
      }
      if ((X > 9) && (X < 100)) {
        tft.setCursor(80, 80);              
        Text(X);                      
      }   
      if (X >= 100){
        tft.setCursor(20, 80); 
        Text(X);
      }
      pred = X;
  }

  X = 208;
         if (X < 10){
        tft.setCursor(140,80);  
        Text(X);  
        }
        if ((X > 9) && (X < 100)) {
          tft.setCursor(80, 80);              
          Text(X);                      
        }   
        if (X >= 100){
          tft.setCursor(20, 80); 
          Text(X);
        }
       
     
}
unsigned long Text(long X) {
  
  if (X < pred){
    tft.fillScreen(BLACK);
  }
  
  if (X%10==0){
    tft.fillRect(80, 80, 55, 80, BLACK);
  }
  if (X%100==0){
    tft.fillRect(20, 80, 55, 80, BLACK);
  }
  if (X == 100){
    tft.fillRect(80, 80, 55, 80, BLACK);
  }

  
  if (X%100==99){
    tft.fillRect(20, 80, 55, 80, BLACK);
  }
  if (X%10==9){
    tft.fillRect(80, 80, 55, 80, BLACK);
  }
 

  

  tft.print(X); 
  tft.print("Km");
  delay(100); 
  tft.fillRect(135, 80, 55, 80, BLACK);
}


