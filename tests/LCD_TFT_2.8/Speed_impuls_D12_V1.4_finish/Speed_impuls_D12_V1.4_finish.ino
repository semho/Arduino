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
#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF
#define rotation 1


Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);
// If using the shield, all control and data lines are fixed, and
// a simpler declaration can optionally be used:
// Adafruit_TFTLCD tft;

const int impuls = 12;           // порт, читающий импульсы

void setup(void) {
pinMode(impuls, INPUT); 
  
  Serial.begin(9600);
  Serial.println(F("TFT LCD test"));

#ifdef USE_ADAFRUIT_SHIELD_PINOUT
  Serial.println(F("Using Adafruit 2.8\" TFT Arduino Shield Pinout"));
#else
  Serial.println(F("Using Adafruit 2.8\" TFT Breakout Board Pinout"));
#endif

  Serial.print("TFT size is "); Serial.print(tft.width()); Serial.print("x"); Serial.println(tft.height());
 

  tft.reset();
  delay(10);
  
  tft.begin(0x9341);

  //настройки экрана
  tft.fillScreen(BLACK);      //заливка
  tft.setRotation(rotation);  //поворот
  tft.setCursor(0, 0);        //координаты
  tft.setTextColor(RED);      //цвет текста   
  tft.setTextSize(10);        //размер текста
 
}
long previousMillis = 0;     // здесь будет храниться время последнего изменения состояния светодиода 
long interval = 1000;        // интервал мигания в миллисекундах
long i = 0;                  // кол-во импульсов за промежуток времени
long k = 2500;               //коэффициент импульсов спидометра
long kmh = 0;                //километры в час
long pred;                   //запоминает предыдущее значение X
int sign = 0;
String predSpeedo;           //строка для запоминания предыдущего значения X

void loop(void) {
      if((digitalRead(impuls) == HIGH) && (sign == 0) ){
         i++;
         sign = 1;
      }      
      if(digitalRead(impuls) == LOW ){
        sign = 0;
      }
      Serial.println(i);
      if (millis() - previousMillis > interval) {
      previousMillis = millis(); 
        //kmh = i/k;
        kmh = i*3600/k; 
        long X = kmh;
        if (X!=pred){ //-постоянно обновляем единицы, если предыдущее значение не равно настоящему
          tft.fillRect(135, 80, 55, 80, BLACK);
        }
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
        pred = X;               // запоминаем значение X
        predSpeedo = String(X); // запоминаем значение X в строке
        i = 0;    
      }
     
}

unsigned long Text(long X) {
  
 /* if (X < pred){ //Если предыдущее значение X больше текущего, обновляем экран
    tft.fillRect(20, 80, 110, 80, BLACK);
  }if (((X%10==0) || (X%10==1) || (X%10==2) || (X%10==3) || (X%10==4) || (X%10==5) || (X%10==6) || (X%10==7) || (X%10==8) || (X%10==9)) && (X!=pred)){ //ужасная конструкция работает, но мигают десятые при обновлении 
    tft.fillRect(80, 80, 55, 80, BLACK);
  }
  if ((X%100==0) && (X!=pred)){
    tft.fillRect(20, 80, 55, 80, BLACK);
  }
  if ((X == 100) && (X!=pred)){
    tft.fillRect(80, 80, 55, 80, BLACK);
  }
  if (X%100==99){//если знычение уменьшается обновляем сотни
    tft.fillRect(20, 80, 55, 80, BLACK);
  }
  if (X%10==9){//если знычение уменьшается обновляем десятки
    tft.fillRect(80, 80, 55, 80, BLACK);
  }*/
  
  /*-------------Работаем со строками---------------*/
  String speedo = String(X);                              //преобразуем число в строку

  if(speedo.length() == 2){                               //проверяем длину строки в 2 символа
    String speedo2 = speedo.substring(0,1);               //запоминаем первое число строки
    String predSpeedo2 = predSpeedo.substring(0,1);       //декларируем строку для первого числа из предыдущих показаний
    if(speedo2.toInt() != predSpeedo2.toInt()){           //если они не равны, то:
      tft.fillRect(80, 80, 55, 80, BLACK);
    }
  } else if(speedo.length() == 3){                        //проверяем длину строки в 3 символа
    String speedo3_1 = speedo.substring(0,1);             //запоминаем первое число строки
    String speedo3_2 = speedo.substring(1,2);             //запоминаем второе число строки

    String predSpeedo3_1 = predSpeedo.substring(0,1);     //декларируем строку для первого числа из предыдущих показаний
    String predSpeedo3_2 = predSpeedo.substring(1,2);     //декларируем строку для второго числа из предыдущих показаний
    
    if(speedo3_1.toInt() != predSpeedo3_1.toInt()){       //если они не равны, то:
      tft.fillRect(20, 80, 55, 80, BLACK);
    }
    if(speedo3_2.toInt() != predSpeedo3_2.toInt()){       //если они не равны, то:
      tft.fillRect(80, 80, 55, 80, BLACK);
    }
  } else if(speedo.length() == 1){
    tft.fillRect(20, 80, 110, 80, BLACK);
  }

  /*-------------конец преобразования строк---------------*/
 
  tft.print(X); 
  
  tft.setTextSize(5);         //размер текста
  tft.setCursor(200,115);   
  tft.print("Km/h");
  
  tft.setTextSize(10);        //размер текста
}
