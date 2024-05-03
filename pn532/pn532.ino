//Library
#include <SPI.h>
#include <Adafruit_PN532.h>
#include <string.h>

//Pin config
#define PN532_SCK  (13)
#define PN532_MOSI (11)
#define PN532_SS   (10)
#define PN532_MISO (12)

//initialize
Adafruit_PN532 nfc(PN532_SCK, PN532_MISO, PN532_MOSI, PN532_SS);

//data
char * url = "Sambar Powder";
uint8_t ndefprefix = NDEF_URIPREFIX_HTTP_WWWDOT;

void setup(void) {
  Serial.begin(115200);
  while (!Serial) delay(10);

  Serial.println("NFC Reader Started!");

  nfc.begin();

  uint32_t versiondata = nfc.getFirmwareVersion();
  if (! versiondata) {
    Serial.print("Didn't find PN53x board");
    while (1);
  }
  // Got ok data, print it out!
  Serial.print("Found NFC Reader"); Serial.println((versiondata>>24) & 0xFF, HEX);
  Serial.print("Firmware ver. "); Serial.print((versiondata>>16) & 0xFF, DEC);
  Serial.print('.'); Serial.println((versiondata>>8) & 0xFF, DEC);
}