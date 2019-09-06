  //used libraries
  #include <SD.h>
  #include <Wire.h>
  #include <Adafruit_BMP085.h>
  #include <MPU6050.h>


 
  #define CS 4   //SD pin 

     MPU6050 mpu;
     Adafruit_BMP085 bmp;


    File temp_file;

    
    double groundPressusre, temperature;
    String temp_string;
    String accelx, accely, accelz;
    
    // Timers
    unsigned long timer = 0;
    float timeStep = 1;


void setup() {
  Serial.begin(115200);
  
  Serial.println("Initializing...");
  while(!bmp.begin(3))
  {
    Serial.println("BMP180 init fail");
    delay(500);
  }
  
  while(!SD.begin(CS))
  {
    Serial.println("SD init fail");
    delay(500);
  }
    
   while(!mpu.begin())
  {
    Serial.println("MPU 6050 init fail");
    delay(500);
  }
   
   Serial.println("Init succeed...");
   SD.remove("temp.txt");
   delay(5000);
  groundPressusre = bmp.readPressure();
  
}

void loop() {
  timer = millis();
  
  //Vector rawGyro = mpu.readRawGyro();
  Vector normGyro = mpu.readNormalizeGyro();
 //Vector rawAccel = mpu.readRawAccel();
  Vector normAccel = mpu.readNormalizeAccel();
  
  
  

  temp_string = String(millis()) + "ms, " + String(bmp.readTemperature()) + "*C, " + String(bmp.readPressure()) + "Pa, " + String(bmp.readAltitude(groundPressusre)) + "m"; 
  accelx = String(millis()) + "ms, X = " + String(normGyro.XAxis) + "dps, " + String(normAccel.XAxis) + "m/s2"; 
  accely = String(millis()) + "ms, Y = " + String(normGyro.YAxis) + "dps, " + String(normAccel.YAxis) + "m/s2";
  accelz = String(millis()) + "ms, Z = " + String(normGyro.ZAxis) + "dps, " + String(normAccel.ZAxis) + "m/s2";

  delay(50);
  
 temp_file = SD.open("temp.txt",FILE_WRITE);

  if (temp_file)
  {
    temp_file.println(temp_string);
    temp_file.println(accelx);
    temp_file.println(accely);
    temp_file.println(accelz);
    temp_file.close();
    Serial.println(temp_string);
    Serial.println(accelx);
    Serial.println(accely);
    Serial.println(accelz);
  }
  else
  {
    Serial.println("SD operation error...");
  }
  
  temp_string = "";
  accelx = "";
  accely = "";
  accelz = "";
  
  delay(50);
}
