// Khai báo cá thư viện cần thiết . 
#include <Wire.h> 
#include <SPI.h> 
#include <MFRC522.h>
#include <LiquidCrystal_I2C.h>






//---------------------------------------------------------------------------------------------


//Khai báo thư viện cho servo và khai báo các chân cắm 
#include <Servo.h>

Servo myservo;
int pos = 0;
int lightPin = A3;


//---------------------------------------------------------------------------------------------




LiquidCrystal_I2C lcd(0x27,16,2); // Khai báo địa chỉ kết nối với Lcd và phân loại LCD là 16 x 2.


int lock =A0; // Khai báo chân cấp nguồn cho relay để bật khóa là chân Analog 0

const int buzzp =  A1; // Khai báo chân dương của chuông với Chân analog 1
const int buzzn =  A2; // Khai báo chân âm của chuông với Chân analog 2




int i=0;
//Khai báo các mã NUID cho các thẻ đã được chọn làm chìa khóa (Mã NUID  được quét bằng một chương trình riêng )
int card1[4]{115,173,27,04}; // Thẻ trắng 
int card2[4]{81,236,204,59}; // Thẻ Xanh
int card3[4]{0,0,0,0};//Chưa cấu hình thẻ
int card4[4]{0,0,0,0};//Chưa cấu hình thẻ

constexpr uint8_t RST_PIN = 10;     //Khai báo  chân digital 10 trên board Atmega 2560  với RFID
constexpr uint8_t SS_PIN = 53;     // Khai báo chân digital 53 trên board Atmega  2560  với RFID

MFRC522 rfid(SS_PIN, RST_PIN); //Kết nối với RFID
MFRC522::MIFARE_Key key; 
byte nuidPICC[4];

void setup() {

//----------------------------------------------------------------------------------------------------



myservo.attach(9);



//--------------------------------------------------------------------------------------------------
  
  //Mở cổng kết nối 
  Serial.begin(9600);
  pinMode(lock, OUTPUT);
  pinMode(buzzp, OUTPUT);
  pinMode(buzzn, OUTPUT);
  digitalWrite(lock, HIGH); 
  
  SPI.begin(); 
  rfid.PCD_Init();  
 
  lcd.init();                      
  lcd.backlight();
  lcd.setCursor(0,0); // Hiện thị ra màn hình tiêu đề dự án ( RFID Lock Project )
  lcd.print("  RFID  ");
  lcd.setCursor(0,1);
  lcd.print("  Lock Project  "); 
  lcd.clear();   
  //Hiển thị ra màn hình chào 
  for (byte i = 0; i < 6; i++) {key.keyByte[i] = 0xFF;}
  
  Serial.println("Hello.");
  Serial.print("Code từ RFID");  
  lcd.setCursor(0, 0);
  lcd.print("THPT MacDinhChi");
  lcd.setCursor(0, 1);
  lcd.print("Scan Your Card");
}

void loop() {


   //----------------------------------------------------------------------------------------------------
  
   
  int lightLevel =analogRead(lightPin);
  lightLevel = map(lightLevel, 0, 1023, 0, 179);
  pos = constrain(lightLevel, 0, 179);

  myservo.write(pos);
  delay(100);


//-------------------------------------------------------------------------------------------------------
  
 
//TH1 Nếu là thẻ chưa được cấu hình thì không làm gì cả
  
  if ( ! rfid.PICC_IsNewCardPresent())
    return;
    

  //TH2 Thẻ đã được cấu hình mã NUID trên Board Arduino
  if ( ! rfid.PICC_ReadCardSerial())
    return;
// Đọc loại PICC bằng RFID
  Serial.print(F("Loại PICC: "));
  MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
  Serial.println(rfid.PICC_GetTypeName(piccType));

   // Lưu trữ NUID vào mảng nuidPICC 
    for (byte i = 0; i < 4; i++) {
    nuidPICC[i] = rfid.uid.uidByte[i];
    Serial.println(nuidPICC[i]);
    }
//Dưới đây là trường hợp hiển thị đối với từng thẻ.
     if(card1[0] == nuidPICC[0] &&card1[1] == nuidPICC[1] && card1[2] == nuidPICC[2] && card1[3] == nuidPICC[3])
 {
 
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
  
