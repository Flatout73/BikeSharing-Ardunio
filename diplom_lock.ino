#include <TroykaGPS.h>

#include <SoftwareSerial.h>

SoftwareSerial BTSerial(6, 3);

//SoftwareSerial GPSSerial(7, 2);

//GPS gps(GPSSerial);

#define MAX_SIZE_MASS 16
// массив для хранения текущего времени
char strTime[MAX_SIZE_MASS];
// массив для хранения текущей даты
char strDate[MAX_SIZE_MASS];
// массив для хранения широты в градусах, минутах и секундах
char latitudeBase60[MAX_SIZE_MASS];
// массив для хранения долготы в градусах, минутах и секундах
char longitudeBase60[MAX_SIZE_MASS];

int LED = 13;

int switchReed=5;

int solenoid=11;

bool isOpen = false;

void setup() {

  pinMode(solenoid, OUTPUT);
  pinMode(switchReed, INPUT);
  
  Serial.begin(9600);
  BTSerial.begin(9600);
  
  while(!Serial || !BTSerial);
  Serial.println("AT commands: okay");

//  GPSSerial.begin(115200);
//  Serial.println("GPS init is OK on speed 115200");
//  GPSSerial.write("$PMTK251,9600*17\r\n");
//  GPSSerial.end();
//  GPSSerial.begin(9600);
//  Serial.print("GPS init is OK on speed 9600\n");
}

void loop() {
//  delay(500);
//  digitalWrite(LED, LOW);
//  delay(500);
//  digitalWrite(LED, HIGH);

//  digitalWrite(solenoid, HIGH);
//  delay(2000);
//  digitalWrite(solenoid, LOW);
//  delay(1000);

  
  if(BTSerial.available()) {
    Serial.println("BT avaliable");
    String command = BTSerial.readString();
    Serial.println(command);
    if (command == "OPENLOCK") {
        digitalWrite(solenoid, HIGH);
        delay(2000);
        digitalWrite(solenoid, LOW);
        delay(10);
    }
  } else {
     if (digitalRead(switchReed)==HIGH) {
      if (isOpen) {
          BTSerial.write("CLOSED");
          isOpen = false;
      }
     } else {
          BTSerial.write("OPENED");
          Serial.println("Your Door is Open"); 
          isOpen = true;
     }
  }

  if(Serial.available())
    BTSerial.write(Serial.read());

      // если пришли данные с gps-модуля
//  if (gps.available()) {
//    // считываем данные и парсим
//    gps.readParsing();
//    // проверяем состояние GPS-модуля
//    switch(gps.getState()) {
//      // всё OK
//      case GPS_OK:
//        Serial.println("GPS is OK");
//        // выводим координаты широты и долготы
//        // 1. в градусах, минутах и секундах
//        // 2. градусах в виде десятичной дроби
//        Serial.println("GPS Coordinates: ");
//        gps.getLatitudeBase60(latitudeBase60, MAX_SIZE_MASS);
//        gps.getLongitudeBase60(longitudeBase60, MAX_SIZE_MASS);
//        Serial.print("Latitude\t");
//        Serial.print(latitudeBase60);
//        Serial.print("\t\t");
//        Serial.println(gps.getLatitudeBase10(), 6);
//        Serial.print("Longitude\t");
//        Serial.print(longitudeBase60);
//        Serial.print("\t\t");
//        Serial.println(gps.getLongitudeBase10(), 6);
//        // выводим количество видимых спутников
//        Serial.print("Sat: ");
//        Serial.println(gps.getSat());
//        // выводим текущую скорость
//        Serial.print("Speed: ");
//        Serial.println(gps.getSpeedKm());
//        // выводим высоту над уровнем моря
//        Serial.print("Altitude: ");
//        Serial.println(gps.getAltitude());
//        // выводим текущее время
//        Serial.print("Time: ");
//        gps.getTime(strTime, MAX_SIZE_MASS);
//        gps.getDate(strDate, MAX_SIZE_MASS);
//        Serial.write(strTime);
//        Serial.println();
//        // выводим текущую дату
//        Serial.print("Date: ");
//        Serial.write(strDate);
//        Serial.println("\r\n");
//        // каждую переменную дату и времени можно выводить отдельно
//  /*    Serial.print(gps.getHour());
//        Serial.print(gps.getMinute());
//        Serial.print(gps.getSecond());
//        Serial.print(gps.getDay());
//        Serial.print(gps.getMonth());
//        Serial.print(gps.getYear());
//  */   
//        break;
//      // ошибка данных
//      case GPS_ERROR_DATA:
//        Serial.println("GPS error data");
//        break;
//      // нет соединение со спутниками
//      case GPS_ERROR_SAT:
//        //Serial.println("GPS no connect to satellites!!!");
//        break;
//    }
//  }
}
