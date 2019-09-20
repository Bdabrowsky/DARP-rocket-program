//used libraries
#include <SD.h>
#include <Wire.h>
#include <Adafruit_BMP085.h>
#include <MPU6050.h>



#define CS 4   //SD pin 

MPU6050 mpu;
Adafruit_BMP085 bmp;


File logs;


double groundPressure, temperature;
String temp_string;
String x, y, z;

// Timers
unsigned long timer = 0;
float timeStep = 1;




void setup() {
  Serial.begin(115200);



  Serial.println("Initializing...");
  while (!bmp.begin(3))
  {
    Serial.println("BMP180 init fail");
    delay(500);
    buzzer(200);
  }

  while (!SD.begin(CS))
  {
    Serial.println("SD init fail");
    delay(500);
    buzzer(500);
  }

  while (!mpu.begin())
  {
    Serial.println("MPU 6050 init fail");
    delay(500);
    buzzer(800);
  }

  Serial.println("Init succeed...");
  buzzer(100);
  delay(100);
  buzzer(100);
  delay(100);

  if (!SD.exists("logs.csv"))
  {
    logs = SD.open("logs.csv", FILE_WRITE);
    logs.println(F("timestamp(ms) \t Temperature(*C) \t Pressure(Pa) \t Altitude(m) \t Xgyro \t Xacceleration(m/s2) \t Xacceleration(G) \t Ygyro \t Yacceleration(m/s2) \t Yacceleration(G) \t Zgyro \t Zacceleration(m/s2) \t Zacceleration(G)"));
    logs.close();
  }

  mpu.calibrateGyro();
  groundPressure = bmp.readPressure();

}





void loop() {
  timer = millis();

  Vector rawGyro = mpu.readRawGyro();
  Vector rawAccel = mpu.readRawAccel();
  Vector normAccel = mpu.readNormalizeAccel();




  temp_string = String(millis())  + "\t" + String(bmp.readTemperature())  + "\t" + String(bmp.readPressure())  + "\t" + String(bmp.readAltitude(groundPressure))  + "\t" ;
  x =     String(rawGyro.XAxis)  + "\t" + String(normAccel.XAxis)  + "\t" + String(normAccel.XAxis / 10) + "\t" ;
  y =     String(rawGyro.YAxis)  + "\t" + String(normAccel.YAxis)  + "\t" + String(normAccel.YAxis / 10) + "\t" ;
  z =     String(rawGyro.ZAxis)  + "\t" + String(normAccel.ZAxis)  + "\t" + String(normAccel.ZAxis / 10) ;

  delay(100);

  saveData(temp_string + x + y + z);



  Serial.println(temp_string + x + y + z);


  delay(50);
  digitalWrite(6, LOW);
}



void buzzer(int a)
{
  pinMode(6, OUTPUT);
  //digitalWrite(6, HIGH);
  delay(a);
  digitalWrite(6, LOW);
}

void saveData(String data)
{
  logs = SD.open("logs.csv", FILE_WRITE);


  if (!logs)
  {
    Serial.println("SD operation error...");
    //digitalWrite(6, HIGH);
  }
  else
  {
    logs.println(data);
    logs.close();
  }
}
