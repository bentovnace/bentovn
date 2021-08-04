#include <Wire.h>
#include <SPI.h>
#include <MFRC522.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,16,2);


int lock =A0;

const int buzzp =  A1;
const int buzzn =  A2;




int i=0;
int card1[4]{115,173,27,04};
int card2[4]{227,64,209,3};
int card3[4]{87,63,24,178};
int card4[4]{28,45,77,23};

constexpr uint8_t RST_PIN = 10;     // Configurable, see typical pin layout above
constexpr uint8_t SS_PIN = 53;     // Configurable, see typical pin layout above

MFRC522 rfid(SS_PIN, RST_PIN); // Instance of the class
MFRC522::MIFARE_Key key; 
byte nuidPICC[4];

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(lock, OUTPUT);
  pinMode(buzzp, OUTPUT);
  pinMode(buzzn, OUTPUT);
  digitalWrite(lock, HIGH); 
  
  SPI.begin(); // Init SPI bus
  rfid.PCD_Init(); // Init MFRC522 
 
  lcd.init();                      
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("  RFID  ");
  lcd.setCursor(0,1);
  lcd.print("  Lock Project  "); 
  lcd.clear();   
  for (byte i = 0; i < 6; i++) {key.keyByte[i] = 0xFF;}
  Serial.println("Hello.");
  Serial.print("Code từ RFID");  
  lcd.setCursor(0, 0);
  lcd.print("THPT MacDinhChi");
  lcd.setCursor(0, 1);
  lcd.print("Scan Your Card");
}

void loop() {
   
 

  
  if ( ! rfid.PICC_IsNewCardPresent())
    return;

  // Verify if the NUID has been readed
  if ( ! rfid.PICC_ReadCardSerial())
    return;

  Serial.print(F("Loai PICC: "));
  MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
  Serial.println(rfid.PICC_GetTypeName(piccType));

   // Store NUID into nuidPICC array
    for (byte i = 0; i < 4; i++) {
    nuidPICC[i] = rfid.uid.uidByte[i];
    Serial.println(nuidPICC[i]);
    }

     if(card1[0] == nuidPICC[0] &&card1[1] == nuidPICC[1] && card1[2] == nuidPICC[2] && card1[3] == nuidPICC[3])
 {
 // digitalWrite(buzzp, HIGH);digitalWrite(buzzn, LOW);delay(100);digitalWrite(buzzp, LOW);digitalWrite(buzzn, LOW);
  lcd.clear() ; lcd.setCursor(0, 0);lcd.print("Xin Chao Mung!");
  digitalWrite(buzzp, HIGH);digitalWrite(buzzn, LOW);delay(100);digitalWrite(buzzp, LOW);digitalWrite(buzzn, LOW);
  delay(1000);
  digitalWrite(lock, LOW);
  delay(2000);
  digitalWrite(lock, HIGH); 
   lcd.setCursor(0, 0);
  lcd.print("THPT MacDinhChi");
  lcd.setCursor(0, 1);
  lcd.print("Scan Your Card");   
 }

 else if(card2[0] == nuidPICC[0] &&card2[1] == nuidPICC[1] && card2[2] == nuidPICC[2] && card2[3] == nuidPICC[3])
 {
  //digitalWrite(buzzp, HIGH);digitalWrite(buzzn, LOW);delay(100);digitalWrite(buzzp, LOW);digitalWrite(buzzn, LOW);
  lcd.clear() ; lcd.setCursor(0, 0);lcd.print("Xin Chao Mung!");
  digitalWrite(buzzp, HIGH);digitalWrite(buzzn, LOW);delay(100);digitalWrite(buzzp, LOW);digitalWrite(buzzn, LOW);
  delay(1000);
  digitalWrite(lock, LOW);
  delay(2000);
  digitalWrite(lock, HIGH); 
   lcd.setCursor(0, 0);
  lcd.print("THPT MacDinhChi");
  lcd.setCursor(0, 1);
  lcd.print("Scan Your Card");   
    
 }
else if(card3[0] == nuidPICC[0] &&card3[1] == nuidPICC[1] && card3[2] == nuidPICC[2] && card3[3] == nuidPICC[3])
 {
  //digitalWrite(buzzp, HIGH);digitalWrite(buzzn, LOW);delay(100);digitalWrite(buzzp, LOW);digitalWrite(buzzn, LOW);
  lcd.clear() ; lcd.setCursor(0, 0);lcd.print("Xin Chao Mung!");
  digitalWrite(buzzp, HIGH);digitalWrite(buzzn, LOW);delay(100);digitalWrite(buzzp, LOW);digitalWrite(buzzn, LOW);
  delay(1000);
  digitalWrite(lock, LOW);
  delay(2000);
  digitalWrite(lock, HIGH); 
   lcd.setCursor(0, 0);
  lcd.print("THPT MacDinhChi");
  lcd.setCursor(0, 1);
  lcd.print("Scan Your Card");   
    
 }
 else if(card4[0] == nuidPICC[0] &&card4[1] == nuidPICC[1] && card4[2] == nuidPICC[2] && card4[3] == nuidPICC[3])
 {
  //digitalWrite(buzzp, HIGH);digitalWrite(buzzn, LOW);delay(100);digitalWrite(buzzp, LOW);digitalWrite(buzzn, LOW);
  lcd.clear() ; lcd.setCursor(0, 0);lcd.print("Xin Chao Mung!");
  digitalWrite(buzzp, HIGH);digitalWrite(buzzn, LOW);delay(100);digitalWrite(buzzp, LOW);digitalWrite(buzzn, LOW);
  delay(1000);
  digitalWrite(lock, LOW);
  delay(2000);
  digitalWrite(lock, HIGH); 
   lcd.setCursor(0, 0);
  lcd.print("THPT MacDinhChi");
  lcd.setCursor(0, 1);
  lcd.print("Scan Your Card");   
  }
  
  }
  
