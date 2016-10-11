// IMPORTANT: Adafruit_ILI9341_8bit_AS LIBRARY MUST BE SPECIFICALLY
// CONFIGURED FOR EITHER THE TFT SHIELD OR THE BREAKOUT BOARD.
// DEFAULT IS THE UNO SHIELD
// SEE RELEVANT COMMENTS IN Adafruit_ILI9341_8bit_AS.h FOR SETUP.

#define DEBUG
#include <Adafruit_GFX_AS.h>    // Core graphics library
#include <Adafruit_ILI9341_8bit_AS.h> // Hardware-specific library

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
#define	BLACK   0xFFD3
#define	BLUE    0xFA00
#define	RED     0x3F33
#define	GREEN   0xC33A
#define YELLOW  0x3133
#define WHITE   0x0000


Adafruit_ILI9341_8bit_AS tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);
// If using the shield, all control and data lines are fixed, and
// a simpler declaration can optionally be used:
// Adafruit_ILI9341_8bit_AS tft;

void setup(void) {
 #ifdef DEBUG
  Serial.begin(9600);
  Serial.println("TFT LCD test");
  Serial.print("TFT size is ");
  Serial.print(tft.width());
  Serial.print("x");
  Serial.println(tft.height());
#endif // DEBUG

  tft.reset();
  delay(10);
  tft.begin(0x9341);

#ifdef DEBUG
  Serial.println("Benchmark                Time (microseconds)");

  Serial.print("Screen fill              ");
  Serial.println(testFillScreen());
  delay(500);

  Serial.println("Done!");
#endif // DEBUG
}

void loop(void) {
 
  testFillScreen();
 
}

unsigned long testFillScreen() {
  unsigned long start = micros();
  tft.fillScreen(BLACK);
  tft.fillScreen(RED);
  tft.fillScreen(GREEN);
  tft.fillScreen(BLUE);
  tft.fillScreen(BLACK);
  return micros() - start;
}
