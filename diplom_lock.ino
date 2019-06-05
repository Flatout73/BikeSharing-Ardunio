#include <TroykaGPS.h>
#include <SoftwareSerial.h>

SoftwareSerial BTSerial(6, 3);

SoftwareSerial GPSSerial(7, 2);

GPS gps(GPSSerial);

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

String id = "0xFFE0";

void setup() {

  pinMode(solenoid, OUTPUT);
  pinMode(switchReed, INPUT);
  
  Serial.begin(9600);
  
  GPSSerial.begin(115200);
  Serial.println("GPS init is OK on speed 115200");
  GPSSerial.write("$PMTK251,9600*17\r\n");
  GPSSerial.end();
  GPSSerial.begin(9600);
  Serial.print("GPS init is OK on speed 9600\n");
  
  BTSerial.begin(9600);

  while(!Serial || !BTSerial);
    Serial.println("AT commands: okay");
}

int counter = 30000;
void sendGPS() {
        // если пришли данные с gps-модуля
  if (counter > 30000 && gps.available()) {
    counter = 0;
    // считываем данные и парсим
    gps.readParsing();
    // проверяем состояние GPS-модуля
    switch(gps.getState()) {
      // всё OK
      case GPS_OK:
        Serial.println("GPS is OK");
        // выводим координаты широты и долготы
        // 1. в градусах, минутах и секундах
        // 2. градусах в виде десятичной дроби
        Serial.println("GPS Coordinates: ");
        gps.getLatitudeBase60(latitudeBase60, MAX_SIZE_MASS);
        gps.getLongitudeBase60(longitudeBase60, MAX_SIZE_MASS);
        Serial.print("Latitude\t");
        Serial.print(latitudeBase60);
        Serial.print("\t\t");
        Serial.println(gps.getLatitudeBase10(), 6);
        Serial.print("Longitude\t");
        Serial.print(longitudeBase60);
        Serial.print("\t\t");
        Serial.println(gps.getLongitudeBase10(), 6);
        // выводим текущее время
        Serial.print("Time: ");
        gps.getTime(strTime, MAX_SIZE_MASS);
        gps.getDate(strDate, MAX_SIZE_MASS);
        Serial.write(strTime);
        Serial.println();
        // выводим текущую дату
        Serial.print("Date: ");
        Serial.write(strDate);
        Serial.println("\r\n");

//        String coord = String("COORDINATES: ");
//        coord.concat(gps.getLatitudeBase10());
//        coord.concat(":");
//        coord.concat(gps.getLongitudeBase10());
//        char array[100];
//        coord.toCharArray(array, 100);
//        BTSerial.print(coord);
//        BTSerial.write(coor);

       BTSerial.print("COORDINATES: ");
       BTSerial.print(gps.getLatitudeBase10(), DEC);
       BTSerial.print("\n");
       BTSerial.print(gps.getLongitudeBase10(), DEC);

        break;
      // ошибка данных
      case GPS_ERROR_DATA:
        Serial.println("GPS error data");
        break;
      // нет соединение со спутниками
      case GPS_ERROR_SAT:
 //       Serial.println("GPS no connect to satellites!!!");
        break;
    }
  } else {
    counter++;
  }
}

void loop() {

  BTSerial.listen();
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

    sendGPS();
}
