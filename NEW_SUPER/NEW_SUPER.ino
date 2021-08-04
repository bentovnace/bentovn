#include <Wire.h>
#include <SPI.h>
#include <MFRC522.h>
  #include <LiquidCrystal_I2C.h>
#include <Keypad.h>
LiquidCrystal_I2C lcd(0x27,16,2);

const byte ROWS = 4; //Khai báo là 4 hàng
const byte COLS = 3; //Khai báo là 3 cột
// khai báo các phím có trên Keypad

char keys[ROWS][COLS] = {
  {'#','0','*'},
  {'9','8','7'},
  {'6','5','4'},
  {'3','2','1'}
};
// Khai báo mật khẩu Code của keypad để mở khóa 
char key_code[3];
char password[3]={'3','2','1'};
int lock = A0;

const int buzzp =  A1 ;
const int buzzn =  A2 ;
// Khai báo chân cắm cho keypad
unsigned int k=0;
byte rowPins[ROWS] = {5, 4, 3, 2}; //Kết nối các chân ở các hàng theo các chân kết nối với Arduino
byte colPins[COLS] = {8, 7, 6}; //Kết nối các chân ở các cột theo các chân kết nối với Arduino
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

//Khai báo Địa chỉ thẻ UID đã mã hóa dữ liệu dạng HEX
int i=0;
int card1[4]{115,173,27,04};
int card2[4]{0,0,0,0};
int card3[4]{0,0,0,0};
int card4[4]{0,0,0,0};

constexpr uint8_t RST_PIN = 7;     // kết nối RST_PIN trên RFID đến chân digital số 7 của board Arduino  Atmega 2560
constexpr uint8_t SS_PIN = 53;     // Kết nối chân SSA đến chân số 53 của board Arduino  Atmega 2560

MFRC522 rfid(SS_PIN, RST_PIN); 
MFRC522::MIFARE_Key key; 
byte nuidPICC[4];


//-----------------------------------------------------------------------------------------------------------------------------------------
//Khai báo các chân cắm và quy định số kí tự trên màn hình LCD

LiquidCrystal_I2C lcd1(0x26,16,2 );
// Khai báo chân kết nối của cảm biến hồng ngoại
#define in 22 // cảm biến hồng ngoại phát hiện số người vào nhà kết nối với chân digital 22 trên ATmega 2560
#define out 23 // cảm biến hồng ngoại phát hiện số người ra khỏi nhà  kết nối với chân digital 22 trên ATmega 2560
#define relay 9 // chân kết nối để kích điện cho relay hoạt động khi số người ở trong nhà lớn hơn 0
// Bắt đầu đếm từ số 0
int count=0;
// Đối với trường hợp vào nhà 
void IN()
{
 
  // Số đếm sẽ tăng thêm giá trị (= 1)
    count++;
    lcd1.clear(); 
    lcd1.print("So nguoi trong phong:");  // Hiện thị số người đang có trong phòng
    lcd1.setCursor(0,1);
    lcd1.print(count); 
    delay(1000);
}
// Đối với trường hợp ra khỏi nhà
void OUT()
{ 
  // Số đếm sẽ giảm giá trị (= - 1)
  count--;
    lcd1.clear();
    lcd1.print("So nguoi trong phong:");
    lcd1.setCursor(0,1);                    // Hiện thị số người đang có trong phòng
    lcd1.print(count);
    delay(1000);
}
//-----------------------------------------------------------------------------------------------------------------------------------------
void setup() {
  // Mở cổng kết nối với board ATmega 2560
  Serial.begin(9600);
  // Kết nối đến các thiết bị như khóa còi báo động màn hình .......
  pinMode(lock, OUTPUT);
  pinMode(buzzp, OUTPUT);
  pinMode(buzzn, OUTPUT);
  digitalWrite(lock, HIGH); 
  
  SPI.begin(); 
  rfid.PCD_Init(); 
 
  lcd.init();                      
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("  RFID & KEYPAD ");
  lcd.setCursor(0,1);                   //Hiển thị ra màn hình " RFID & KEYPAD Lock Project "
  lcd.print("  Lock Project  "); 
  lcd.clear();   
  // Thiết lập mật khẩu
  for (byte i = 0; i < 6; i++) {key.keyByte[i] = 0xFF;}
  Serial.println(F("Sử dụng thẻ để quét "));
  Serial.print(F("Sử dụng mật khẩu :"));   // Hiện thị dòng chữ ra màn hình 
  lcd.setCursor(0, 0);
  lcd.print("Quet the cua ban");
  lcd.setCursor(0, 1);
  lcd.print("Nhap mat khau");
}

//-----------------------------------------------------------------------------------------------------------------------------------------

lcd1.begin(16,2);
  lcd1.backlight();
  lcd1.print("THPT Mac Dinh Chi");
  delay(2000);
  pinMode(in, INPUT);
  pinMode(out, INPUT);
  pinMode(relay, OUTPUT);
  lcd1.clear();
  lcd1.print("So nguoi trong phong:");
  lcd1.setCursor(0,1);
  lcd1.print(count);
//----------------------------------------------------------------------------------------------------------------------------------------
void loop() {
  
  char key = keypad.getKey();
  
    if(key != NO_KEY){
    
      
      
     key_code[i++]=key;
     k=i;
    delay(200);
    digitalWrite(lock, HIGH);
     }
     
 if(k==3){
   // Trường hợp có thể vào nhà 
   if(!strncmp(password,key_code,3)){
   lcd.clear() ; lcd.setCursor(0, 0);lcd.print("Co the vao nha");
   digitalWrite(buzzp, HIGH);digitalWrite(buzzn, LOW);delay(100);digitalWrite(buzzp, LOW);digitalWrite(buzzn, LOW);
   digitalWrite(lock, LOW);
   delay(2000);digitalWrite(lock, HIGH); 
   delay(1000);    
    i=k=0;  
  lcd.setCursor(0, 0);
  lcd.print("Quet the cua ban");
  lcd.setCursor(0, 1);
  lcd.print("Nhap mat khau");
   }
 }
  
  if ( ! rfid.PICC_IsNewCardPresent())
    return;

 // Xác minh xem NUID đã được đọc chưa 
  if ( ! rfid.PICC_ReadCardSerial())
    return;

  Serial.print(F("PICC type: "));
  MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
  Serial.println(rfid.PICC_GetTypeName(piccType));

  
// Lưu trữ NUID vào mảng nuidPICC 
    for (byte i = 0; i < 4; i++) {
    nuidPICC[i] = rfid.uid.uidByte[i];
    Serial.println(nuidPICC[i]);
    }

     if(card1[0] == nuidPICC[0] &&card1[1] == nuidPICC[1] && card1[2] == nuidPICC[2] && card1[3] == nuidPICC[3])
 {
 
  lcd.clear() ; lcd.setCursor(0, 0);lcd.print("Co the vao nha");
  digitalWrite(buzzp, HIGH);digitalWrite(buzzn, LOW);delay(100);digitalWrite(buzzp, LOW);digitalWrite(buzzn, LOW);
  delay(1000);
  digitalWrite(lock, LOW);
  delay(2000);
  digitalWrite(lock, HIGH); 
   lcd.setCursor(0, 0);
  lcd.print("Quet the cua ban");
  lcd.setCursor(0, 1);
  lcd.print("Nhap mat khau");   
 }

 else if(card2[0] == nuidPICC[0] &&card2[1] == nuidPICC[1] && card2[2] == nuidPICC[2] && card2[3] == nuidPICC[3])
 {
 
  lcd.clear() ; lcd.setCursor(0, 0);lcd.print("Co the vao nha");
  digitalWrite(buzzp, HIGH);digitalWrite(buzzn, LOW);delay(100);digitalWrite(buzzp, LOW);digitalWrite(buzzn, LOW);
  delay(1000);
  digitalWrite(lock, LOW);
  delay(2000);
  digitalWrite(lock, HIGH); 
   lcd.setCursor(0, 0);
  lcd.print("Quet the cua ban");
  lcd.setCursor(0, 1);
  lcd.print("Nhap mat khau");   
    
 }
else if(card3[0] == nuidPICC[0] &&card3[1] == nuidPICC[1] && card3[2] == nuidPICC[2] && card3[3] == nuidPICC[3])
 {
  
  lcd.clear() ; lcd.setCursor(0, 0);lcd.print("Co the vao nha");
  digitalWrite(buzzp, HIGH);digitalWrite(buzzn, LOW);delay(100);digitalWrite(buzzp, LOW);digitalWrite(buzzn, LOW);
  delay(1000);
  digitalWrite(lock, LOW);
  delay(2000);
  digitalWrite(lock, HIGH); 
   lcd.setCursor(0, 0);
  lcd.print("Quet the cua ban");
  lcd.setCursor(0, 1);
  lcd.print("Nhap mat khau");   
    
 }
 else if(card4[0] == nuidPICC[0] &&card4[1] == nuidPICC[1] && card4[2] == nuidPICC[2] && card4[3] == nuidPICC[3])
 {
  
  lcd.clear() ; lcd.setCursor(0, 0);lcd.print("Co the vao nha");
  digitalWrite(buzzp, HIGH);digitalWrite(buzzn, LOW);delay(100);digitalWrite(buzzp, LOW);digitalWrite(buzzn, LOW);
  delay(1000);
  digitalWrite(lock, LOW);
  delay(2000);
  digitalWrite(lock, HIGH); 
   lcd.setCursor(0, 0);
  lcd.print("Quet the cua ban");
  lcd.setCursor(0, 1);
  lcd.print("Nhap mat khau");   
  }
  //-----------------------------------------------------------------------------------------------------------------------------------------
   
  if(digitalRead(in))
  IN();
  if(digitalRead(out))
  OUT();
  
  if(count<=0)
  {
    lcd1.clear();
    digitalWrite(relay, LOW);
    lcd1.clear();
    lcd1.print("Khong ai trong phong");
    lcd1.setCursor(0,1);
    lcd1.print("Tat Den");
    delay(200);
  }
  
  else
    digitalWrite(relay, HIGH);
  
  }
  
