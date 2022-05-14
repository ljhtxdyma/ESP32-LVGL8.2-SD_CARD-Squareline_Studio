#include "sdcard.hpp"


#define SCK  27
#define MISO  13
#define MOSI  12
#define CS  14

/*
void pin_init(void)
{
    digitalWrite(21, HIGH); // Touch controller chip select (if used)
    digitalWrite(15, HIGH); // TFT screen chip select
    digitalWrite(14, HIGH); // SD card chips select, must use GPIO 5 (ESP32 SS)
}
*/

void sdcard_setup(void)
{
  SPIClass spi = SPIClass(VSPI);
  spi.begin(SCK, MISO, MOSI, CS);

  if (!SD.begin(CS,spi)) {
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

  Serial.println("initialisation done.");
}


void readFile(fs::FS &fs, const char * path){
    Serial.printf("Reading file: %s\n", path);

    File file = fs.open(path);
    if(!file){
        Serial.println("Failed to open file for reading");
        return;
    }

    Serial.print("Read from file: ");
    
    while(file.available()){
        Serial.write(file.read());
    }
    
    file.close();
}

