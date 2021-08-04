
//Nhà thông minh sử dụng công nghệ IOT
 // Khai báo các thư viện cho ESP 8266
#define BLYNK_PRINT Serial            
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <ESP8266mDNS.h>  
#include <WiFiUdp.h>  
#include <ArduinoOTA.h>  
#include <SPI.h>
#include <SimpleTimer.h>
#include <DHT.h>
#include <Adafruit_Sensor.h>

BlynkTimer timer;
// cài Đặt trạng thái relay khi nút tắt mở
void checkPhysicalButton();

int relay1State = LOW;
int pushButton1State = HIGH;

int relay2State = LOW;
int pushButton2State = HIGH;

int relay3State = LOW;
int pushButton3State = HIGH;

int relay4State = LOW;
int pushButton4State = HIGH;

#define AUTH "2NKAS5m0VJruvrPsdEABKly5CZi5Nv4M"  // Mã TOKEN kết nối với Server BLYNK
#define WIFI_SSID "Luan"                   //Tên Wifi kết nối với ESP 8266
#define WIFI_PASS "79797979"                   //Mật khẩu wifi kết nối với ESP 8266

#define SERVER "blynk-cloud.com "             // Khai báo địa chỉ máy chủ BLYNK để kết nối với app BLynk trên android
#define PORT 8442
// Đinh nghĩa các chân cắm
#define RELAY_PIN_1      12   //D6
#define RELAY_PIN_2      16   //D0
#define RELAY_PIN_3       4   //D2
#define RELAY_PIN_4       5   //D1 

#define PUSH_BUTTON_1     2   //D4
#define PUSH_BUTTON_2    14   //D5
#define PUSH_BUTTON_3     0   //D3
#define PUSH_BUTTON_4     1   //TX
//Định nghĩa các nút nhấn trên BLYNK
#define VPIN_BUTTON_1    V12 
#define VPIN_BUTTON_2    V13
#define VPIN_BUTTON_3    V14
#define VPIN_BUTTON_4    V15  

#define OTA_HOSTNAME "Home_Automation" //Địa chỉ truy cập


BLYNK_CONNECTED() {

  // Cập nhật nút trên server

  Blynk.syncVirtual(VPIN_BUTTON_1);
  Blynk.syncVirtual(VPIN_BUTTON_2);
   Blynk.syncVirtual(VPIN_BUTTON_3);
  Blynk.syncVirtual(VPIN_BUTTON_4);

}

// Khi nhấn nút trên ứng dụng BLYNK chuyển trạng thái của các Relay

BLYNK_WRITE(VPIN_BUTTON_1) {
  relay1State = param.asInt();
  digitalWrite(RELAY_PIN_1, relay1State);
}

BLYNK_WRITE(VPIN_BUTTON_2) {
  relay2State = param.asInt();
  digitalWrite(RELAY_PIN_2, relay2State);
}
BLYNK_WRITE(VPIN_BUTTON_3) {
  relay3State = param.asInt();
  digitalWrite(RELAY_PIN_3, relay3State);
}
BLYNK_WRITE(VPIN_BUTTON_4) {
  relay4State = param.asInt();
  digitalWrite(RELAY_PIN_4, relay4State);
}

void checkPhysicalButton()
{
  if (digitalRead(PUSH_BUTTON_1) == LOW) {
    
    if (pushButton1State != LOW) {

      // Tùy chỉnh bật tắt relay 1 
      relay1State = !relay1State;
      digitalWrite(RELAY_PIN_1, relay1State);

      // cập nhật nút nhấn 1 
      Blynk.virtualWrite(VPIN_BUTTON_1, relay1State);
    }
    pushButton1State = LOW; // Bật relay 1
  } else {
    pushButton1State = HIGH; // Tắt relay 1
  }

  if (digitalRead(PUSH_BUTTON_2) == LOW) {
    if (pushButton2State != LOW) {

      // Tùy chỉnh bật tắt relay 2
      relay2State = !relay2State;
      digitalWrite(RELAY_PIN_2, relay2State);

      // Cập nhật nút nhấn 2
      Blynk.virtualWrite(VPIN_BUTTON_2, relay2State);
    }
    pushButton2State = LOW; // Bật relay 2
  } else {
    pushButton2State = HIGH; // Tắt relay 2
  }

  if (digitalRead(PUSH_BUTTON_3) == LOW) {
    if (pushButton3State != LOW) {

      // Tùy chỉnh bật tắt relay 3
      relay3State = !relay3State;
      digitalWrite(RELAY_PIN_3, relay3State);

      // Cập nhật nút nhấn
      Blynk.virtualWrite(VPIN_BUTTON_3, relay3State);
    }
    pushButton3State = LOW; // Bật relay 3
  } else {
    pushButton3State = HIGH; // Tắt relay 3
  }

  if (digitalRead(PUSH_BUTTON_4) == LOW) {
  
    if (pushButton4State != LOW) {

      // Tùy chỉnh trạng thái bật tắt relay 4
      relay4State = !relay4State;
      digitalWrite(RELAY_PIN_4, relay4State);

      // Cập nhật nút nhấn 4
      Blynk.virtualWrite(VPIN_BUTTON_4, relay4State);
    }
    pushButton4State = LOW; // Bật relay 4
  } else {
    pushButton4State = HIGH; // Tắt relay 4
  }
}
//---------------------------------------------------------------------------------------
#define DHTPIN 15
#define DHTTYPE DHT11  
DHT dht(DHTPIN, DHTTYPE);
SimpleTimer timer;
void sendSensor()
{
  float h = dht.readHumidity();
  float t = dht.readTemperature(); 

  if (isnan(h) || isnan(t)) {
    Serial.println("That bai khi doc gia tri cam bien");
    return;
  }
  Blynk.virtualWrite(V5, h);  //V5
  Blynk.virtualWrite(V6, t);  //V6 
} 

//-----------------------------------------------------------------------------------------------

void setup()
{
 // Thiết lập các nút
  Serial.begin(115200);
  Blynk.begin(AUTH, WIFI_SSID, WIFI_PASS,"blynk-cloud.com", 8442);
   //--------------------------------------------------------------------------------------------------------------
  dht.begin();
timer.setInterval(1000L, sendSensor);
//----------------------------------------------------------------------------------------------------------------
  ArduinoOTA.setHostname(OTA_HOSTNAME);  // Thiết lập tên nhận dạng riêng 
  ArduinoOTA.begin();  

  pinMode(RELAY_PIN_1, OUTPUT);
  pinMode(PUSH_BUTTON_1, INPUT_PULLUP);
  digitalWrite(RELAY_PIN_1, relay1State);


  pinMode(RELAY_PIN_2, OUTPUT);
  pinMode(PUSH_BUTTON_2, INPUT_PULLUP);
  digitalWrite(RELAY_PIN_2, relay2State);


  pinMode(RELAY_PIN_3, OUTPUT);
  pinMode(PUSH_BUTTON_3, INPUT_PULLUP);
  digitalWrite(RELAY_PIN_3, relay3State);


  pinMode(RELAY_PIN_4, OUTPUT);
  pinMode(PUSH_BUTTON_4, INPUT_PULLUP);
  digitalWrite(RELAY_PIN_4, relay4State);

  // Thiết lập một hàm được gọi cứ sau 100 mili giây 
  timer.setInterval(500L, checkPhysicalButton);
}

void loop()
{
  Blynk.run();
  ArduinoOTA.handle();  //  OTA
  timer.run();
}
