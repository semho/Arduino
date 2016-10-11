#define pot A7
long previousMillis = 0;

int Port8 = 8;
int Port9 = 9;
int Port10 = 10;
int Port11 = 11;

void setup()
{
pinMode(Port8, OUTPUT);
pinMode(Port9, OUTPUT);
pinMode(Port10, OUTPUT);
pinMode(Port11, OUTPUT);

digitalWrite(Port8, LOW);
digitalWrite(Port9, LOW);
digitalWrite(Port10, LOW);
digitalWrite(Port11  , LOW);
  
// устанавливаем соединение с портом
 Serial.begin(9600); 
// пин с потенциометром - вход

pinMode(pot, INPUT);

}

void loop()
{

// объявляем переменную x

int x;

// считываем напряжение с потенциометра:

// будет получено число от 0 до 1023

// делим его на 4, получится число в диапозоне

// 0-255 (дробная часть будет отброшена)

x = analogRead(pot) / 4;

if (millis() - previousMillis > 2000) {
   //Запоминаем время первого срабатывания
   previousMillis = millis();

int y = map(x, 16, 80, 5, 1);
Serial.println(y);
   //Поправочный коэффициент
   int k=1;
   if ( y == 5) {
     digitalWrite(Port8, HIGH);
     digitalWrite(Port9, HIGH);
     digitalWrite(Port10, HIGH);
     digitalWrite(Port11, HIGH);
   } else if (y==4) {
     digitalWrite(Port8, LOW);
     digitalWrite(Port9, HIGH);
     digitalWrite(Port10, HIGH);
     digitalWrite(Port11, HIGH);
   } else if (y==3) {
     digitalWrite(Port8, LOW);
     digitalWrite(Port9, LOW);
     digitalWrite(Port10, HIGH);
     digitalWrite(Port11, HIGH);
   } else if (y==2) {
     digitalWrite(Port8, LOW);
     digitalWrite(Port9, LOW);
     digitalWrite(Port10, LOW);
     digitalWrite(Port11, HIGH);
   } else {
     digitalWrite(Port8, LOW);
     digitalWrite(Port9, LOW);
     digitalWrite(Port10, LOW);
     digitalWrite(Port11, LOW);
     if (digitalRead(Port11) == LOW){
       digitalWrite(Port11, HIGH);
       delay(200);
     }
     if (digitalRead(Port11) == HIGH){
       digitalWrite(Port11, LOW);
       delay(3000);
     }
   }
  
  }
  
 
}
