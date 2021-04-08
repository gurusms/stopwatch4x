#include <Arduino.h>
#include <MsTimer2.h>
#include <Button.h>
#include <LiquidCrystal_I2C.h> // Подключение библиотеки
#include <math.h>
// LiquidCrystal_I2C lcd(0x3F,16,2); // Указываем I2C адрес (наиболее распространенное значение), а также параметры экрана (в случае LCD 1602 - 2 строки по 16 символов в каждой 
// LiquidCrystal_I2C lcd(0x27,16,2); // Указываем I2C адрес (наиболее распространенное значение), а также параметры экрана (в случае LCD 1602 - 2 строки по 16 символов в каждой 
LiquidCrystal_I2C lcd(0x27,20,4); // Указываем I2C адрес (наиболее распространенное значение), а также параметры экрана (в случае LCD 1602 - 2 строки по 16 символов в каждой 

// объявляем переменные
#define K1 2  // кнопка 1-го секундомера
#define K2 3  // кнопка 2-го секундомера
#define K3 4  // кнопка 3-го секундомера
#define K4 5  // кнопка 4-го секундомера
#define SS 8 // кнопка стоп-старт
#define ZZ 9  // на зуммер (сигнал)
//#define LED 13 // кнопка 4-го секундомера подключена к выводу 13
#define FS tone(ZZ,1000,100); // короткий сигнал
#define LS tone(ZZ,1000,500); // длинный сигнал


/*unsigned long curentTime;  // текущее значение времени
unsigned long prevTime;    // предыдущее значение времени*/
float h, m, s, ms; // часы, минуты, секунды, миллисекунды
// unsigned int h, m, s, ms; // часы, минуты, секунды, миллисекунды
unsigned long start, finished, elapsed;
unsigned long over;
boolean k1state = false;   // создаем глобальную переменную k1state
boolean k2state = false;   // создаем глобальную переменную k2state
boolean k3state = false;   // создаем глобальную переменную k1state
boolean k4state = false;   // создаем глобальную переменную k2state
byte ssstate = 1;   // создаем глобальную переменную k2state

Button k1(K1, 15);  // создание объекта - кнопка
Button k2(K2, 15);  // создание объекта - кнопка
Button k3(K3, 15);  // создание объекта - кнопка
Button k4(K4, 15);  // создание объекта - кнопка
Button ss(SS, 15);  // создание объекта - кнопка

// обработчик прерывания 
void  timerInterupt() {
  k1.scanState();  // вызов метода ожидания стабильного состояния для кнопки  
  k2.scanState();  // вызов метода ожидания стабильного состояния для кнопки  
  k3.scanState();  // вызов метода ожидания стабильного состояния для кнопки  
  k4.scanState();  // вызов метода ожидания стабильного состояния для кнопки  
  ss.scanState();  // вызов метода ожидания стабильного состояния для кнопки  
}

void setup() {
  Serial.begin(115200);
  MsTimer2::set(2, timerInterupt); // задаем период прерывания по таймеру 2 мс 
  MsTimer2::start();              // разрешаем прерывание по таймеру
// -----
 pinMode(K1, INPUT);  // определяем 1 секундомер (кнопка) как вход
 pinMode(K2, INPUT);  // определяем 2 секундомер (кнопка) как вход
 pinMode(K3, INPUT);  // определяем 3 секундомер (кнопка) как вход
 pinMode(K4, INPUT);  // определяем 4 секундомер (кнопка) как вход
 pinMode(SS, INPUT);  // определяем  стоп-старт как вход
 pinMode(ZZ, OUTPUT); // определяем  зуммер (сигнал) как выход
 //pinMode(LED, OUTPUT); // определяем светодиод как выход
  lcd.init();                      // Инициализация дисплея  
  lcd.backlight();                 // Подключение подсветки
  lcd.setCursor(0,0);              // Установка курсора в начало первой строки
  lcd.print("  StopWatch ver 0.1");
  delay (3000);
  lcd.clear();
  lcd.setCursor(0,0);              // Установка курсора в начало первой строки
  LS
}

// фукция вывода на экран
void lcdout(bool ks, unsigned char col, unsigned char row){
if ( ks == false ) {
if (m < 1) lcd.setCursor(col+3, row);
else lcd.setCursor(col, row);
if (h > 0) {lcd.print(h, 0); lcd.print(":");}
if (m > 0) {
  if (m < 10) lcd.print("0");
  lcd.print(m, 0); lcd.print(":");
  }
if (s < 10) lcd.print("0");
lcd.print(s, 0);
// lcd.print(":");
// if (ms < 10) lcd.print("0");
lcd.print(".");
// if (ms < 10) lcd.print("0"); // когда считаем десятками мс ноль всегда.
// if (ms<10) lcd.print(ms, 0); // удаляем второй знак "0"
lcd.print(ms, 0);
}
}


void loop() {
switch (ssstate)  {
case 0 :      
    // считаем время
    finished = millis(); // сохраняет время остановки, чтобы вычислить прошедшее время.
    elapsed = finished - start;
    // elapsed = elapsed * 10; // укскоряем в 10 раз время !!!!! для отладки !!!!
    h = int(elapsed / 3600000);
    over = elapsed % 3600000;
    m = int(over / 60000);
    over = over % 60000;
    s = int(over / 1000);
    // over = over / 10 ; // отсекаем ноль у миллисекунд (делаем счет десятками без нуля)
    // ms = (over / 10) % 10;
    ms = over % 1000;
    // ms = round (ms/100); для округления ms до одного знака после точки
    // ms = elapsed;
    // s = ms /1000;
    // m = s/60;
    // h = m/60;
    break;
case 1 :
     // делаем остановку таймеров
     // если уже все таймеры остановлены
     if (k1state & k2state & k3state & k4state ) ssstate = 2;
      // здесь двигаем время если есть часы
      /*lcd.clear();        // Очистка дисплея  
      lcd.setCursor(0, 0);
      lcd.print("        00:00           00:00   ");*/
     break;
case 2 :
     // фаза перед стартом
     // мигаем экраном для предупреждения перед стартом
     delay (300);
     lcd.noBacklight();
     delay (300);
     lcd.backlight();
     break;
case 3 :
      // обнуляем счетчики, состояние кнопок и Стартуем
      ssstate = 0;
      k1state = false;
      k2state = false;
      k3state = false;
      k4state = false;
      elapsed = 0;
      // Сигнал перед стартом
      delay(800);
      FS
      delay(800);
      LS
      lcd.clear();
      m = 0; s = 0; ms = 0;
      start = millis(); // Обнуляем счетчик времени
      Serial.print("START\n"); // Выводим в СОМ порт начало отсчета
      break;
default :
     // код выполняется если  не совпало ни одно предыдущее значение
     break;
}

// показываем результаты
// секундомер 1
lcdout (k1state,0,0);
// секундомер 2
lcdout (k2state,0,1);
// // секундомер 3
// lcdout (k3state,8,0);
// // секундомер 4
// lcdout (k4state,8,1);
// секундомер 3
lcdout (k3state,0,2);
// секундомер 4
lcdout (k4state,0,3);


delay (100);

// Обработка кнопок
  if ( ss.flagClick == true ) {
    // был клик кнопки
    ss.flagClick= false;         // сброс признака 
    ssstate++;
    FS // короткий звук
    }    
  
  if ( k1.flagClick == true ) {
    // был клик кнопки
    k1.flagClick = false;         // сброс признака 
    if ( k1state == false ) Serial.print("K1_" + String(elapsed)+"\n");
    k1state = true;
    FS // короткий звук
    }    
  
  if ( k2.flagClick == true ) {
    // был клик кнопки
    k2.flagClick = false;         // сброс признака 
    if ( k2state == false ) Serial.print("K2_" + String(elapsed)+"\n");
    k2state = true;
    FS // короткий звук
  }    

  if ( k3.flagClick == true ) {
    // был клик кнопки
    k3.flagClick = false;         // сброс признака 
    if ( k3state == false ) Serial.print("K3_" + String(elapsed)+"\n");
    k3state = true;
    FS // короткий звук
  }    

  if ( k4.flagClick == true ) {
    // был клик кнопки
    k4.flagClick = false;         // сброс признака 
    if ( k4state == false ) Serial.print("K4_" + String(elapsed)+"\n");
    k4state = true;
    FS // короткий звук
  }    
}
