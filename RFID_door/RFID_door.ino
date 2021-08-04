//Khai báo thư viện RFID
#include <SPI.h>

#include <MFRC522.h>
 
#define SS_PIN 10 //khai báo chân kết nối với chân ss của RFID
#define RST_PIN 9 // Khai báo chân kết nối vơi chân RST của RFID
#define LED_G 5 //Định nghĩa chân cấp nguồn cho Led xanh
#define LED_R 4 //Định nghĩa chân cấp nguồn cho led đỏ(trắng)
#define RELAY 3 //Chân cấp nguồn cho relay
#define BUZZER 2 //Chân cấp nguồn cho còi
#define ACCESS_DELAY 2000 //Truy cập hợp lệ
#define DENIED_DELAY 1000 //Truy cập không hợp lệ
MFRC522 mfrc522(SS_PIN, RST_PIN);   //Tạo  RFID MFRC522
void setup() 
{
  Serial.begin(9600);   // Bắt đầu tạo giao tiếp ở cổng 9600
  SPI.begin();          // mở cổng SPI
  mfrc522.PCD_Init();   // Giao tiếp với RFID
  //Thiết lập đầu ra cho các thiết bị
  pinMode(LED_G, OUTPUT); 
  pinMode(LED_R, OUTPUT);
  pinMode(RELAY, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  noTone(BUZZER);
  digitalWrite(RELAY, HIGH);
  Serial.println("Đặt thẻ để quét mã...");
  Serial.println();

}
void loop() 
{
  // Thẻ mới
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  // Chọn thẻ
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
  //Đọc mã UID của thẻ
  Serial.print("UID tag :");
  String content= "";
  byte letter;
  //In ra mã thẻ theo bộ chữ cái HEXA
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.print("Message : ");
  content.toUpperCase();
  if (content.substring(1) == "73 AD 1B 04") //Nhập mã UID của thẻ được chọn để làm thẻ truy cập
  { 
    // Hiện thị kết quả ra màn hình
    Serial.println("Truy cập thành công");
    Serial.println();
    // cấp nguồn cho relay chạy đòng thồi đèn led xanh sáng
    delay(500);
    digitalWrite(RELAY, LOW);
    digitalWrite(LED_G, HIGH);
    delay(ACCESS_DELAY);
    digitalWrite(RELAY, HIGH);
    digitalWrite(LED_G, LOW);
  }
 
 else   {
  //Hiện thị ra màn hình
    Serial.println("Truy cập bị từ chối");
  //Cấp nguồn cho led Đỏ và còi chạy, Toàn bộ thiết bị ngưng hoạt động
    digitalWrite(LED_R, HIGH);
    tone(BUZZER, 300);
    delay(DENIED_DELAY);
    digitalWrite(LED_R, LOW);
    noTone(BUZZER);
  }
}
