#include <Keypad.h>

#include <LiquidCrystal_I2C.h>

#include <MFRC522.h> // Include of the RC522 Library
#include "FastLED.h" // Include of the FastLED library
#include <SPI.h> // Used for communication via SPI with the Module

// WS2812 RGB Stick connection


#define greenLED 29 // Pin 5 connected green LED
#define NUM_LEDS 8 // # of WS2812 LEDs on stick
CRGB leds[NUM_LEDS]; // FastLED Library Init
#define redLED 28 // red LED connected to pin 4
#define SDAPIN 11 // RFID Module SDA Pin is connected to the UNO 10 Pin
#define RESETPIN 10 // RFID Module RST Pin is connected to the UNO 8 Pin
#define relay 30
#define Buzzer 31 // Pin 3 connected to + pin of the Buzzer
#include <Keypad.h>
#include <Wire.h> 
#include <LiquidCrystal.h>
LiquidCrystal lcd(32,34,42,40,38,36);
#define Password_Lenght 5 // Give enough room for six chars + NULL char


char Data[Password_Lenght]; // 6 is the number of chars it can hold + the null char = 7
char Master[Password_Lenght] = "6297"; 
byte data_count = 0, master_count = 0;
bool Pass_is_good;
char customKey;

const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

byte rowPins[ROWS] = {
  9,8,7,6}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {
 5,4,3,2}; //connect to the column pinouts of the keypad

Keypad customKeypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS); //initialize an instance of class NewKeypad 

byte FoundTag; // Variable used to check if Tag was found
byte ReadTag; // Variable used to store anti-collision value to read Tag information
byte TagData[MAX_LEN]; // Variable used to store Full Tag Data
byte TagSerialNumber[5]; // Variable used to store only Tag Serial Number
byte GoodTagSerialNumber[5] = {0x34, 0xD1, 0xD8, 0x2B}; 
byte orGoodTagSerialNumber[5] = {0x56, 0x2, 0x6B, 0xAC, }; // The Tag Serial number we are looking for

MFRC522 nfc(SDAPIN, RESETPIN); // Init of the library using the UNO pins declared above
int Con=90
;

void setup() {
  pinMode(redLED,OUTPUT);
  pinMode(greenLED,OUTPUT);
  pinMode(22, OUTPUT);
  pinMode(relay,OUTPUT);
  analogWrite(12,Con);
 lcd.begin(16, 2);
pinMode(Buzzer, OUTPUT); // Set buzzer pin to an Output pin
digitalWrite(Buzzer, LOW); // Buzzer Off at startup
digitalWrite(relay, LOW);
SPI.begin();
Serial.begin(115200);
digitalWrite(22,HIGH);

// Start to find an RFID Module
Serial.println("Looking for RFID Reader");
nfc.begin();
byte version = nfc.getFirmwareVersion(); // Variable to store Firmware version of the Module

// If can't find an RFID Module 
if (! version) { 
Serial.print("Didn't find RC522 board.");
while(1); //Wait until a RFID Module is found
}

// If found, print the information about the RFID Module
Serial.print("Found chip RC522 ");
Serial.print("Firmware version: 0x");
Serial.println(version, HEX);
Serial.println();
}

void loop() {

// Light up starting LED's
lcd.setCursor(0,0);
  lcd.print("Enter Password");
  
String GoodTag="False"; // Variable used to confirm good Tag Detected

// Check to see if a Tag was detected
// If yes, then the variable FoundTag will contain "MI_OK"
FoundTag = nfc.requestTag(MF1_REQIDL, TagData);

if (FoundTag == MI_OK) {
delay(200);

// Get anti-collision value to properly read information from the Tag
ReadTag = nfc.antiCollision(TagData);
memcpy(TagSerialNumber, TagData, 4); // Write the Tag information in the TagSerialNumber variable

Serial.println("Tag detected.");
Serial.print("Serial Number: ");
for (int i = 0; i < 4; i++) { // Loop to print serial number to serial monitor
Serial.print(TagSerialNumber[i], HEX);
Serial.print(", ");
}
Serial.println("");
Serial.println();


// Check if detected Tag has the right Serial number we are looking for 
for(int i=0; i < 4; i++){
if (GoodTagSerialNumber[i] != TagSerialNumber[i]) {
break; // if not equal, then break out of the "for" loop
}
if (i == 3) { // if we made it to 4 loops then the Tag Serial numbers are matching
GoodTag="TRUE";
} 
}
for(int i=0; i < 4; i++){
if (orGoodTagSerialNumber[i] != TagSerialNumber[i]) {
break; // if not equal, then break out of the "for" loop
}
if (i == 3) { // if we made it to 4 loops then the Tag Serial numbers are matching
GoodTag="TRUE";
} 
}
if (GoodTag == "TRUE"){
lcd.clear();
lcd.print("Access Granted!!");
lcd.setCursor(0,1);
lcd.print("Welcome, Brent");
Serial.println("Success!!!!!!!");
Serial.println();
digitalWrite(relay,HIGH);
digitalWrite(greenLED,HIGH);
for (int y = 0; y < 3; y++){
digitalWrite (Buzzer, HIGH) ;// Buzzer On
delay (50) ;// Delay 1ms 
digitalWrite (Buzzer, LOW) ;// Buzzer Off
delay (50) ;// delay 1ms
}
delay(2750);
digitalWrite(greenLED,LOW);
  lcd.clear();

digitalWrite(relay,LOW);

}
else {
  lcd.clear();
lcd.print("ACCESS DENIED!");
Serial.println("TAG NOT ACCEPTED...... :(");
Serial.println();
digitalWrite(redLED,HIGH);
FastLED.show();
for (int y = 0; y < 3; y++){
digitalWrite (Buzzer, HIGH) ;// Buzzer On
delay (300) ;// Delay 1ms 
digitalWrite (Buzzer, LOW) ;// Buzzer Off
delay (400) ;// delay 1ms
}
digitalWrite(redLED,LOW);
delay(500); 
  lcd.clear();

}
}
lcd.setCursor(0,0);
  lcd.print("Enter Password");
  
  customKey = customKeypad.getKey();
  if (customKey) // makes sure a key is actually pressed, equal to (customKey != NO_KEY)
  {
    Data[data_count] = customKey; // store char into data array
    lcd.setCursor(data_count,1); // move cursor to show each new char
    lcd.print(Data[data_count]); // print char at said cursor
    data_count++; // increment data array by 1 to store new char, also keep track of the number of chars entered
  }

  if(data_count == Password_Lenght-1) // if the array index is equal to the number of expected chars, compare data to master
  {
    lcd.clear();
    lcd.setCursor(0, 0);
