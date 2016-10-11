
/* спидометр и тахометр  */
#include <MsTimer2.h>              // подключаем таймер прерывания
#include <EEPROM.h>                // библиотека для работы внутренней памяти
#define Hollpin 2                  // датчик Холла спидометр
#define Hollpin2 3                 // датчик тахометра
long rpm = 0;
long kmh = 0;
volatile long odometr;
unsigned long odoMem;
volatile long valOdometr;
long k = 60000;                    // количество импульсов на километр пути спидометра
volatile long i = 0;               // количество импульсов спидометра
volatile long r = 0;               // количество импульсов тахометра

int Relay = 13;                    // 13 порт включает реле питания от аккумулятора
int Zajiganie = 12;                // 12 порт следит за напряжением замка зажигания


long previousMillis = 0;           // здесь будет храниться время последнего изменения состояния
long interval = 10000; 

/* датчик температуры */
#define portTemp A2
/* датчик топлива */
#define portFuel A1

/* цифровой вольтметр */
#define portVolt A0
float vout = 0.0;                  // переменная с значением выходного напряжения  
float vin = 0.0;                   // переменная с результатом измерений
float R1 = 47000.0;                // сопротивление R1
float R2 = 10000.0;                // сопротивление R2
int valueV = 0;                    // переменная хранения данных 

/* порты поворотников */
const int leftTurn = 4;
const int rightTurn = 5;
int leftState = 0;
int rightState = 0;
/* порт дальнего света */
const int distantLight = 6;
int lightState = 0;
/* порты нейтрали и датчика ножки */ 
const int portN = 7;
const int portS = 8;
int nState = 0;
int sState = 0;
/* порт давления масла */
const int portOil = 9;
int oilState = 0;

/* вспомогательные функции для вывода информации на дисплей Nextion*/
void setValue(uint32_t number, String nameIndificator){
  char buf[10] = {0};
  String cmd;

  utoa(number, buf, 10); //преобразование числа number в строку buf
  cmd += nameIndificator +".val=";
  cmd += buf;

  sendCommand(cmd.c_str()); //преобразование String в char
}
void setPic(uint32_t number, String nameIndificator){
  char buf[10] = {0};
  String cmd;

  utoa(number, buf, 10); //преобразование числа number в строку buf
  cmd += nameIndificator +".pic=";
  cmd += buf;

  sendCommand(cmd.c_str()); //преобразование String в char
}
void setTxt(String txt, String nameIndificator){
  String cmd;
  cmd += nameIndificator +".txt=";
  cmd +='"'+ txt + '"';     //текс обязательно должен быть передан в кавычках
  sendCommand(cmd.c_str()); //преобразование String в char
}
void sendCommand( const char* cmd ){
  Serial.print(cmd);
  Serial.write(0xff);
  Serial.write(0xff);
  Serial.write(0xff);
}


void setup()   {
  Serial.begin(9600);
  delay(2000);
  sendCommand( "" );
  sendCommand( "page 0" );

  pinMode(portVolt, INPUT);
  pinMode(portTemp, INPUT);
  pinMode(portFuel, INPUT);

  // инициализируем пин, подключенный к левому поворотнику, как вход
  pinMode(leftTurn, INPUT);
  // инициализируем пин, подключенный к правому поворотнику, как вход
  pinMode(rightTurn, INPUT);

  pinMode(distantLight, INPUT);

  pinMode(portN, INPUT);
  pinMode(portS, INPUT);

  pinMode(portOil, INPUT);

  pinMode(Relay, OUTPUT);   
  pinMode(Zajiganie, INPUT);  
  digitalWrite(Relay, LOW);    // при старте зажигания реле переводит питание от аккумулятора

  odoMem = EEPROM_ulong_read(0);  //считываем показание одометра из памяти
  
  digitalWrite(Hollpin, HIGH);
  digitalWrite(Hollpin2, HIGH);
  attachInterrupt(0, tImpuls, RISING);
  attachInterrupt(1, tImpulsT, RISING);
  MsTimer2::set(500, flash);
  MsTimer2::start();


}

/* функция датчика температуры */
void Temp(){
  int valTemp = analogRead(portTemp);
  int valTempDisplay = map(valTemp, 633, 85, 0, 120); 
  if (valTempDisplay < 85)                             
  {   
    valTempDisplay=120;                               
  }
  if (valTempDisplay > 633)                             
  {   
    valTempDisplay=0;                               
  }
  setValue(valTempDisplay, "Temp_n");
}
/* функция датчика топлива */
void Fuel(){
  int valFuel = analogRead(portFuel) / 4;
  int valFuelDisplay = map(valFuel, 16, 80, 100, 0);  
  if (valFuelDisplay < 16)                             
  {   
    valFuelDisplay=100;                               
  }
  if (valFuelDisplay > 80)                             
  {   
    valFuelDisplay=0;                               
  }
  setValue(valFuelDisplay, "fuel_bar");
}
/* функция цифрового вольтметра */
void Volt(){
  valueV = analogRead(portVolt);             // читаем аналоговый сигнал, с аналог. порта - 0 
  vout = (valueV * 5.0) / 1024.0;            // не забываем про внесение поправки (5.0 - это идеальный вариант,хотя все возможно)
  vin = vout / (R2/(R1+R2));                 // в переменной будет значение входного напряжения
  if (vin<0.09)                              // проверка, для получ. 0
  {   
     vin=0.0;                                // обнуляем нежелательное значение
  }
  String volts = String(vin);  
  setTxt(volts, "V");
}
/* повороты */
void Turns(){
   // считываем значения с входа кнопки
  leftState = digitalRead(leftTurn);
  // проверяем нажата ли кнопка
  // если нажата, то leftState будет HIGH:
  if (leftState == HIGH) {   
    setPic(2, "arrow_l"); 
  }
  else {
    setPic(14, "arrow_l"); 
  }
  
  rightState = digitalRead(rightTurn);
  if (rightState == HIGH) {   
    setPic(3, "arrow_r"); 
  }
  else {
    setPic(14, "arrow_r"); 
  }
}
void distLight(){
   lightState = digitalRead(distantLight);
    if (lightState == HIGH) {   
      setPic(4, "light"); 
    }
    else {
      setPic(14, "light"); 
    }
}
void N_and_S(){
  nState = digitalRead(portN);
  sState = digitalRead(portS);
  if (nState == HIGH) {   
  setPic(5, "N"); 
  }
  else {
    setPic(14, "N"); 
  }
  if (sState == HIGH) {   
  setPic(7, "S"); 
  }
  else {
    setPic(14, "S"); 
  }
}
void Oil(){
  oilState = digitalRead(portOil);
  if (oilState == HIGH) {   
  setPic(6, "Oil"); 
  }
  else {
    setPic(15, "Oil"); 
  }
}

// чтение из памяти
unsigned long EEPROM_ulong_read(int addr) {    
  byte raw[4];
  for(byte i = 0; i < 4; i++) raw[i] = EEPROM.read(addr+i);
  unsigned long &num = (unsigned long&)raw;
  return num;
}

// запись в память
void EEPROM_ulong_write(int addr, unsigned long num) {
  byte raw[4];
  (unsigned long&)raw = num;
  for(byte i = 0; i < 4; i++) EEPROM.write(addr+i, raw[i]);
}


void loop() {
  Turns();
  distLight();
  N_and_S();
  Oil();
  if (millis() - previousMillis > interval) {
      previousMillis = millis(); 
       Temp();
       Fuel();
       Volt();  
  }

  //Раскомментировать, когда будет готова обвязка
 /* int statusZajiganie = digitalRead(Zajiganie);   
  if (statusZajiganie == LOW){
   delay(500);
   if (statusZajiganie == LOW){
    
    unsigned long odo = valOdometr;
      if (odo != odoMem){
        EEPROM_ulong_write(0, odo);
      }
    
   delay(5000);
   digitalWrite(Relay, HIGH);
   }
  }*/
}

void flash() {
  /* спидометр и тахометр  */
  kmh = i *2 * 3600 / k;                     // количество импульсов умножаем на 2 т.к. таймер в пол секунды. 3600 - перевод в часы
  setValue(kmh, "spido");                    // отправляем данные о скорости в числовое поле дисплея 
  
  rpm = (r*60*2)/2;                          // умножаем на 2 т.к. таймер в пол секунды. умножаем на 60 секунд, чтобы получить кол-во оборотов в минуту.делим на 2, т.к. коленвал делает 2 импульса за один оборот
  setValue(rpm, "taho_n");                   // отправляем данные тахометра в числовое поле дисплея
  
  long bar = map(rpm, 0, 11500, 0, 100); 
  setValue(bar, "taho");                     // отправляем данные тахометра  в прогресс-бар дисплея

  /* одометр */
  odometr = odometr + i;
  valOdometr = (odometr/k) + odoMem;
  setValue(valOdometr, "odometr");  

  /*-----------*/
 
  /*задержка и обнуление импульсов*/
  delay(10);
  i = 0;
  r = 0;
  
}
void tImpuls (){
  i++;
}
void tImpulsT (){
  r++;
}
