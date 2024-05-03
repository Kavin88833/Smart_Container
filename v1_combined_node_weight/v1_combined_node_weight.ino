//Library

//NFC Reader
#include <SPI.h>
#include <Adafruit_PN532.h>
#include <string.h>

//Weight Scale
#include <Arduino.h>
#include "HX711.h"

//Pin config
#define PN532_SCK  (13)
#define PN532_MOSI (11)
#define PN532_SS   (10)
#define PN532_MISO (12)

// HX711 circuit wiring
const int LOADCELL_DOUT_PIN = 2;
const int LOADCELL_SCK_PIN = 3;

const int outputPin = 51;


HX711 scale;

//initialize
Adafruit_PN532 nfc(PN532_SCK, PN532_MISO, PN532_MOSI, PN532_SS);

//**********************************************************************************************************************************************************************
// Global variables
// NFC
//**********************************************************************************************************************************************************************
// NFC Reader
uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };  // Buffer to store the returned UID
uint8_t uidLength;                        // Length of the UID (4 or 7 bytes depending on ISO14443A card type)
uint8_t dataLength;

//Read_Data
char read_data[20];
int idx = 0;

//Write_Data
char * url = "Rasam POWDER";
uint8_t ndefprefix = NDEF_URIPREFIX_HTTP_WWWDOT;

//Weight Scale
float w;
char result[20];
//**********************************************************************************************
//Weight functions
void weight()
{
  float weight;
  //Serial.print("one reading:\t");
  //Serial.print(scale.get_units(), 1);
  //Serial.print("\t| average:\t");
  Serial.println("Place the weight..");
  w = scale.get_units(10);
  String a = String(read_data);
  //Serial.println(weight, 5);
  if(w < 0.7){
    for(int t=0; t<5; t++)
    {
      digitalWrite(outputPin, HIGH);
      w = scale.get_units(10);
      String b = String(w,2);
      Serial.print("Weight:");
      Serial.println(b);
      for(int i=0; i<1; i++)
      {
        Serial1.println(a+"/"+b);
      }
      Serial.println("Data sent...");
      delay(700);
      digitalWrite(outputPin, LOW);
      if(w>0.7){
        break;
      }
      else{
        Serial.print("-");
      }
    }
  }
  else{
    digitalWrite(outputPin, HIGH);
    String b = String(w,2);
    Serial.print("Weight:");
    Serial.println(b);
    for(int i=0; i<1; i++)
    {
      Serial1.println(a+"/"+b);
    }
    Serial.println("Data sent...");
    delay(700);
    digitalWrite(outputPin, LOW);
  }
  
}


//*******************************************************************************************
// NFC functions

//functions
void erase(uint8_t *data){
  uint8_t success;
  Serial.print("Erasing previous data area ");
  for (uint8_t i = 4; i < (dataLength/4)+4; i++)
  {
    memset(data, 0, 4);
    success = nfc.ntag2xx_WritePage(i, data);
    Serial.print(".");
    if (!success)
    {
      Serial.println(" ERROR!");
      return;
    }
  }
  Serial.println(" DONE!");
}

void write(){
  uint8_t success;
  pad_string_with_spaces(*url, 20);
  Serial.print("Writing URI as NDEF Record ... ");
  success = nfc.ntag2xx_WriteNDEFURI(ndefprefix, url, dataLength);
  if (success)
  {
    Serial.println("DONE!");
  }
  else
  {
    Serial.println("ERROR! (URI length?)");
  }
}

void read(){
  uint8_t success;
  uint8_t data[32];
  char info[20];
  idx = 0;
  for (uint8_t i = 7; i < 11; i++)
  {
    success = nfc.ntag2xx_ReadPage(i, data);


    if (success)
    {
      //nfc.PrintHexChar(data, 4);
      //PrintHexChar(data, 4);
      
      for (int i = 0; i < 4; i++) {
      read_data[idx] = (char)data[i]; // Print each byte
      idx++;
    }
    }
    else
    {
      Serial.println("Unable to read the requested page!");
    }
  }
  Serial.print("Item Name: ");
  Serial.println(read_data);
  idx = 0;
}



void pad_string_with_spaces(char *str, int length) {
    int pad_length = length - strlen(str);

    // Handle strings longer than 20 characters (truncate)
    if (pad_length < 0) {
        str[length] = ' ';  // Null terminate the string at the 20th character
    } else {
        // Pad the string with spaces
        strncat(str, " ", pad_length);
    }
}

//******************************************************************************************************************************************************************************

void setup(void) {
  Serial.begin(115200);
  Serial1.begin(9600); 
  pinMode(outputPin, OUTPUT);
  //Weight scale Setup
  //************************************************************************************************************************************************
  Serial.println("Initializing the scale");

  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);

  Serial.println("Before setting up the scale:");
  Serial.print("read: \t\t");
  Serial.println(scale.read());      // print a raw reading from the ADC

  Serial.print("read average: \t\t");
  Serial.println(scale.read_average(20));   // print the average of 20 readings from the ADC

  Serial.print("get value: \t\t");
  Serial.println(scale.get_value(5));   // print the average of 5 readings from the ADC minus the tare weight (not set yet)

  Serial.print("get units: \t\t");
  Serial.println(scale.get_units(5), 1);  // print the average of 5 readings from the ADC minus tare weight (not set) divided
            // by the SCALE parameter (not set yet)
            
  scale.set_scale(-879.84);
  //scale.set_scale(-471.497);                      // this value is obtained by calibrating the scale with known weights; see the README for details
  scale.tare();               // reset the scale to 0

  Serial.println("After setting up the scale:");

  Serial.print("read: \t\t");
  Serial.println(scale.read());                 // print a raw reading from the ADC

  Serial.print("read average: \t\t");
  Serial.println(scale.read_average(20));       // print the average of 20 readings from the ADC

  Serial.print("get value: \t\t");
  Serial.println(scale.get_value(5));   // print the average of 5 readings from the ADC minus the tare weight, set with tare()

  Serial.print("get units: \t\t");
  Serial.println(scale.get_units(5), 1);        // print the average of 5 readings from the ADC minus tare weight, divided
            // by the SCALE parameter set with set_scale

  Serial.println("Readings:");
  
  //NFC Reader Setup
  //************************************************************************************************************************************************
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

  Serial.println("Waiting for a tag ...");
}

//*********************************************************************************************************************************************************************

void loop(void)
{
  Serial.println("In Loop");
  uint8_t success;

  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength);

  // It seems we found a valid ISO14443A Tag!
  if (success)
  {
    // 2.) Display some basic information about the card
    //*****************************************************************************
    //Serial.println("Found an NFC Tag");
    //Serial.print("  UID Length: ");Serial.print(uidLength, DEC);Serial.println(" bytes");
    //Serial.print("  UID Value: ");
    //nfc.PrintHex(uid, uidLength);
    //Serial.println("");

    if (uidLength != 7)
    {
      Serial.println("This doesn't seem to be an NTAG203 tag (UUID length != 7 bytes)!");
    }
    else
    {
      uint8_t data[32];

      
      // in OTP memory (page 3)
      memset(data, 0, 4);
      success = nfc.ntag2xx_ReadPage(3, data);
      if (!success)
      {
        Serial.println("Unable to read the Capability Container (page 3)");
        return;
      }
      else
      {
        
        if (!((data[0] == 0xE1) && (data[1] == 0x10)))
        {
          Serial.println("This doesn't seem to be an NDEF formatted tag.");
          Serial.println("Page 3 should start with 0xE1 0x10.");
        }
        else
        {
          // 4.) Determine and display the data area size
          dataLength = data[2]*8;
          //Serial.print("Tag is NDEF formatted. Data area size = ");
          //Serial.print(dataLength);
          //Serial.println(" bytes");
          
          // READ DATA
          read();
          weight();
          Serial.println("Back");

          //Erase the old data area
          //erase(data);

          //Write NDEF URI record
          //write();

        } // CC contents NDEF record check
      } // CC page read check
    } // UUID length check

    // Wait a bit before trying again
    Serial.flush();
  } // Start waiting for a new ISO14443A tag
  delay(1000);
}