
#include <MsTimer2.h> //подключаем таймер прерывания

#define Hollpin 2   // датчик Холла
long kmh = 0;
long k = 60000;     // количество импульсов на километр пути
volatile long i = 0;// количество импульсов
 
void setup()   {
  
  digitalWrite(Hollpin, HIGH);
  attachInterrupt(0, tImpuls, RISING);
  MsTimer2::set(500, flash);
  MsTimer2::start();
}
 

void loop() {
}
void flash() {
  kmh = i *2 * 3600 / k; // количество импульсов умножаем на 2 т.к. таймер в пол секунды. 3600 - перевод в часы
 
  i = 0;
}
void tImpuls (){
  i++;
}
