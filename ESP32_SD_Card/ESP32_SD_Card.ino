// Simple ESP32 data logging example

// Libraries for SD card
#include <SPI.h>
#include "FS.h"
#include "SD.h"

// Timer variables
unsigned long lastTime = 0;
unsigned long timerDelay = 10000;

// Variables to hold sensor readings
float temp;
int hum, ldr;

const char* fileName = "/data.txt";
String dataMessage;

// Initialize SD card
void initSDCard() {
  if (!SD.begin(5)) {
    Serial.println("Card Mount Failed");
    return;
  }
  uint8_t cardType = SD.cardType();

  if (cardType == CARD_NONE) {
    Serial.println("No SD card attached");
    return;
  }
  Serial.print("SD Card Type: ");
  if (cardType == CARD_MMC) {
    Serial.println("MMC");
  } else if (cardType == CARD_SD) {
    Serial.println("SDSC");
  } else if (cardType == CARD_SDHC) {
    Serial.println("SDHC");
  } else {
    Serial.println("UNKNOWN");
  }
  uint64_t cardSize = SD.cardSize() / (1024 * 1024);
  Serial.printf("SD Card Size: %lluMB\n", cardSize);
}

// Write to the SD card
void writeFile(fs::FS& fs, const char* path, const char* message) {
  Serial.printf("Writing file: %s\n", path);

  File file = fs.open(path, FILE_WRITE);
  if (!file) {
    Serial.println("Failed to open file for writing");
    return;
  }
  if (file.print(message)) {
    Serial.println("File written");
  } else {
    Serial.println("Write failed");
  }
  file.close();
}

// Append data to the SD card
void appendFile(fs::FS& fs, const char* path, const char* message) {
  Serial.printf("Appending to file: %s\n", path);

  File file = fs.open(path, FILE_APPEND);
  if (!file) {
    Serial.println("Failed to open file for appending");
    return;
  }
  if (file.print(message)) {
    Serial.println("Message appended");
  } else {
    Serial.println("Append failed");
  }
  file.close();
}

void setup() {
  Serial.begin(115200);

  initSDCard();

  // If the data.txt file doesn't exist
  // Create a file on the SD card and write the data labels
  File file = SD.open(fileName);
  if (!file) {
    Serial.println("File doesn't exist");
    Serial.println("Creating file...");
    writeFile(SD, fileName, "Temperature, Humidity, LDR \r\n");
  } else {
    Serial.println("File already exists");
  }
  file.close();
}

void loop() {
  if ((millis() - lastTime) > timerDelay) {
    //Get sensor readings
    temp = float(random(0, 50)) / 3.33;  // Divide to get different float values
    hum = random(20, 100);               // humidity random range
    ldr = random(0, 1023);               // ldr random range

    //Concatenate all info separated by commas
    dataMessage = String(temp) + "," + String(hum) + "," + String(ldr) + "\r\n";
    Serial.print("Saving data: ");
    Serial.println(dataMessage);

    //Append the data to file
    appendFile(SD, fileName, dataMessage.c_str());

    lastTime = millis();
  }
}