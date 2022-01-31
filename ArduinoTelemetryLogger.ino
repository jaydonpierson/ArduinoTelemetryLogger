#include <Wire.h>
#include <JY901.h>

#include <SPI.h>
#include <SD.h>


const int chipSelect = A0;
const int RXLED = 17;
const int TXLED = 30;

File file;


void setup() 
{

  pinMode(RXLED, OUTPUT);
  pinMode(TXLED, OUTPUT);
  digitalWrite(TXLED, LOW);
  digitalWrite(RXLED, LOW);
  
  Serial.begin(9600);
  Serial1.begin(115200);
  //while (!Serial); // Wait for serial
  Serial.println("Initializing");

  if (!SD.begin(chipSelect)) {
    Serial.println("SD init failed!");
    while (true) {
      digitalWrite(RXLED, HIGH);
      delay(500);
      digitalWrite(RXLED, LOW);
      delay(500);
    }
  }
  Serial.println("SD initialized");

  Serial.println("Listing files:");
  File root = SD.open("/");

  uint8_t fid = 1;
  char filename[20];
  while (true) {
    if (0 == fid) {
      fid = 1;
      sprintf(filename, "%03d.csv", fid);
      break;
    }
    sprintf(filename, "%03d.csv", fid);
    if (!SD.exists(filename)) {
      break;
    }
    fid++;
  }

  Serial.print("Using file ");
  Serial.println(filename);
  file = SD.open(filename, FILE_WRITE);
  if (!file) {
    Serial.println("Failed to open file");
    while (true) {
      digitalWrite(TXLED, HIGH);
      delay(500);
      digitalWrite(TXLED, LOW);
      delay(500);
    }
  }
  Serial.println("File opened");

  file.print("ArduinoMillis, ");
  file.print("Year, ");
  file.print("Month, ");
  file.print("Day, ");
  file.print("Hour, ");
  file.print("Minute, ");
  file.print("Second, ");
  file.print("Milisecond, ");
  
  file.print("AccX, ");
  file.print("AccY, ");
  file.print("AccZ, ");
  
  file.print("GyroX, ");
  file.print("GyroY, ");
  file.print("GyroZ, ");
  
  file.print("AngleX, ");
  file.print("AngleY, ");
  file.print("AngleZ, ");

  file.print("MagX, ");
  file.print("MagY, ");
  file.print("MagZ, ");

  file.print("FileWriteMillis");

  file.println();

  file.flush();

  Serial.println("Beginning loop");
  digitalWrite(TXLED, HIGH);
  
}

void loop() 
{

  uint32_t start_time = millis();
  file.print( start_time ); file.print(", ");
  
  // Date/Time
  file.print( JY901.stcTime.ucYear ); file.print(", ");
  file.print( JY901.stcTime.ucMonth ); file.print(", ");
  file.print( JY901.stcTime.ucDay ); file.print(", ");
  file.print( JY901.stcTime.ucHour ); file.print(", ");
  file.print( JY901.stcTime.ucMinute ); file.print(", ");
  file.print( JY901.stcTime.ucSecond ); file.print(", ");
  file.print( JY901.stcTime.usMiliSecond ); file.print(", ");

  // Accelerometer
  file.print( JY901.stcAcc.a[0] ); file.print(", ");
  file.print( JY901.stcAcc.a[1] ); file.print(", ");
  file.print( JY901.stcAcc.a[2] ); file.print(", ");

  // Gyroscope
  file.print( JY901.stcGyro.w[0] ); file.print(", ");
  file.print( JY901.stcGyro.w[1] ); file.print(", ");
  file.print( JY901.stcGyro.w[2] ); file.print(", ");
  
  // Euler Angles
  file.print( JY901.stcAngle.Angle[0]); file.print(", ");
  file.print( JY901.stcAngle.Angle[1]); file.print(", ");
  file.print( JY901.stcAngle.Angle[2]); file.print(", ");
  
  // Magnetometer
  file.print( JY901.stcMag.h[0] ); file.print(", ");
  file.print( JY901.stcMag.h[1] ); file.print(", ");
  file.print( JY901.stcMag.h[2] ); file.print(", ");

  file.flush();
  file.print( millis() - start_time ); file.print(", ");
  file.println();

  while (Serial1.available()) 
  {
    JY901.CopeSerialData(Serial1.read()); //Call JY901 data cope function
  }
  
}


void printDirectory(File dir, int numTabs) {
  while (true) {

    File entry =  dir.openNextFile();
    if (! entry) {
      // no more files
      break;
    }
    for (uint8_t i = 0; i < numTabs; i++) {
      Serial1.print('\t');
    }
    Serial1.print(entry.name());
    if (entry.isDirectory()) {
      Serial1.println("/");
      printDirectory(entry, numTabs + 1);
    } else {
      // files have sizes, directories do not
      Serial1.print("\t\t");
      Serial1.println(entry.size(), DEC);
    }
    entry.close();
  }
}
