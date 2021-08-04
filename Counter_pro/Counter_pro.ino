
//Khai báo thư viện cho các dự án 

#include <ESP8266HTTPClient.h>           
#include <Wire.h>                 
#include <LiquidCrystal_I2C.h>    
LiquidCrystal_I2C lcd(0x27,16,2);
#include "json_parser.h"
#include "WifiConnect.h"
#include <Blynk.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
// -------------------------------------------------------
#define BLYNK_PRINT Serial    
#include <SPI.h>
#include <SimpleTimer.h>
#include <DHT.h>
 //------------------------------------------------------------

#define DHTPIN 2 //Thiết lập chân đọc dữ liệu cảm biến là chân digital 8 (D8)trên Esp 8266     
#define DHTTYPE DHT11     // Cài đặt loại cảm biến là  DHT 11


DHT dht(DHTPIN, DHTTYPE);
SimpleTimer timer;







//-------------------------------------------------------------

//Khai báo kết nôi tới wifi và kết nối Blynk
 char auth[] = "ecvqWSNGw-7edDuq4b-luN_uCbqbkxJu"; // Mã Token để kết nối đến dự án đã làm trên Blynk       
char ssid[] = "Luan";                      // Tên Wifi dùng làm Wifi kết nối với Esp 8266 để kết nối mạng
char pass[] = "79797979";                 // Mật Khẩu Wifi kết nối mạng 
 
#define inSensor 14 // Khai báo chân D5 là chân kết nối với IR sensor in ( cảm biến hồng ngoại vào ).
#define outSensor 12 // Khai báo chân D6 là chân kêt nối với IR sensor out ( Cảm biến hồng ngoại ra ).
 
int inStatus; // Khai báo biến giá trị số người vào .
int outStatus; // Khai báo biến giá trị số người ra .
 
int countin = 0; // Đặt giá trị biến đếm số người vào là 0 
int countout = 0; // Đặt giá trị biến đếm số người ra là 0
 
int in;   //Biến giá trị số người trong phòng 
int out;  // Biến giá trị số người đã rời phòng
int now;
 
#define relay 0 //Chân D3 là chân cấp xung cho relay hoạt động 
WidgetLED light(V25); //Viral pin 25 trên Blynk dùng để hiện thị trạng thái hoạt động của relay



 //-----------------------------------------------------------
//Thiết lập các kết nối 
#define s2ms(second) (second*1000)
unsigned long long  prev_millis(0);

#define country_code "Vietnam"//Định nghĩa country_code = Việt nam 

//Kết nối các tín hiệu 
int interval = s2ms(60); //Trả về thời gian lúc chương trình bắt đầu hoạt động
unsigned long long PreviousMillis = 0; //Trước 
unsigned long long CurrentMillis = interval;//Hiện tại
bool bFirstKickMillis = false; //

extern bool bGotIpFlag; //
//Xây dựng mã URL kết nối tới Server chứa dữ liệu , thông tin về tình hình Covid 19 
static String build_url_from_country(String country)
{
  String url = "http://coronavirus-19-api.herokuapp.com/countries/";
  url = url + country;   // ulr = http://coronavirus-19-api.herokuapp.com/countries/Vietnam  Tìm kiếm địa chỉ tình hình Covid Việt nam theo ULR này 
  return url;
}

void setup(void)
{ 
//----------------------------------------------------------------
dht.begin(); //Cài đặt gửi dữ liệu cảm biến
timer.setInterval(1000L, sendSensor);


//---------------------------------------------------------------

 Serial.begin(115200); //Mở cổng kết nối với Esp 8266 tại 115200.
  Blynk.begin(auth, ssid, pass);
  delay(1000);  // đợi 1s để kết nối 
  // khai báo đầu ra cho các thiết bị 
  pinMode(inSensor, INPUT);
  pinMode(outSensor, INPUT);
  pinMode(relay, OUTPUT);
  digitalWrite(relay, HIGH);


  //--------------------------------------------------------------

  //Hiển thị ra màn hình chào 
  lcd.init();       
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Covid-19 Watch");
   lcd.setCursor(0, 1);
  lcd.print("Case In VietNam");
//Kết nói đến Server 
  #if defined JSON_DEBUG
    Serial.begin(9600);
  #endif
  
  JSON_LOG("Connecting...");
  
  vConnWifiNetworkViaSdk();
}

void loop()
{


  //------------------------------------------------------------------------------


//Đọc giá trị các cảm biến hồng ngoại
   Blynk.run(); // Chạy trên Blynk
   timer.run(); 

 //-------------------------------------------------------------------  
  inStatus =  digitalRead(inSensor); //Đọc giá trị cảm biến hồng ngoại In
  outStatus = digitalRead(outSensor);//Đọc giá trị cảm biến hồng ngoại OUT
  if (inStatus == 0)//Nếu cảm biên IR sensor in hoạt động thì biến Countin tăng thêm 1 
  {
    in = countin++;
  }
 
  if (outStatus == 0) //Nếu cảm biên IR sensor out hoạt động thì biến Countout tăng thêm 1
  {
    out = countout++;
  }
 //Giá trị now là giá trị chỉ số người hiện tại (số người hiện tại = số người vào trừ số người ra )
  now = in - out;
 //Thực hiện các hành động
  if (now <= 0)
  {           // Nếu giá trị của biến now bé hơn hoặc = 0 thì không kích xung cho relay hoạt động
    digitalWrite(relay, HIGH);
    light.off(); //Hiện thị trên Blynk đèn tắt
   
  }
  else //Ngược lại
  {//Kích xung cho relay 
    digitalWrite(relay, LOW);
    light.on(); // hiện thị trên Blynk đèn bật
 
  }
  Blynk.virtualWrite(V7, in); //Nút hiện thí số lần vào nhà trên Blynk
  Blynk.virtualWrite(V8, out); //Nút hiện thị số lần ra khỏi nhà trên Blynk
  Blynk.virtualWrite(V9, now); //Nút hiện thị tổng số người hiện tại
  delay(1000);


  //-------------------------------------------------
  if(bGotIpFlag) bGotIp(); //lấy địa chỉ IP
  
  if(bFirstKickMillis) CurrentMillis = millis();
  
  if (!bGotIpFlag && CurrentMillis - PreviousMillis >= interval) 
  {
    if(!bFirstKickMillis) CurrentMillis = 0;
    
    bFirstKickMillis = true;
    
    PreviousMillis = CurrentMillis;
    
    HTTPClient http; //Kết nối với địa chỉ HTTP đã cập  nhật ULR.
    http.begin(build_url_from_country(country_code));
    
    int httpCode = http.GET(); 
  
    if(httpCode > 0) 
    {
      String payload = http.getString();
       
      char* JsonArray = (char *)malloc(payload.length() + 1);
      if (!JsonArray) JSON_LOG("upssss fuck");
      
      payload.toCharArray(JsonArray, payload.length() + 1);
      
      JSON_LOG(JsonArray);
      //Tìm kiếm và đọc các giá trị về tình hình covid 19 
      if (json_validate(JsonArray))
      {
        int confirmed = (int)get_json_value(JsonArray, "cases", INT);
        int deaths = (int)get_json_value(JsonArray, "deaths", INT);
        int recovered = (int)get_json_value(JsonArray, "recovered", INT);
      
        JSON_LOG(confirmed);
        JSON_LOG(deaths);
        JSON_LOG(recovered);



        
        //Hiển thị ra màn hình số ca nhiễm 
        lcd.clear();
        lcd.print("Nhiem");
        lcd.setCursor(7,0);
        lcd.print("Chet");
        lcd.setCursor(12,0);
        lcd.print("Khoi");
        lcd.setCursor(0, 1);
        lcd.print(confirmed);
        lcd.setCursor(7, 1);
        lcd.print(deaths);
        lcd.setCursor(12, 1);
        lcd.print(recovered);
        
      }
      
      free(JsonArray);
    }
    
    http.end(); //Kết thúc phiên làm việc với HTTP 
  }
}

void sendSensor()
{
  float h = dht.readHumidity(); //đọc giá trị độ ẩm
  float t = dht.readTemperature(); // đọc giá trị nhiệt độ

  if (isnan(h) || isnan(t)) {
    Serial.println("Không thể đọc dữ liệu cảm biến");
    return;
  }
  
  Blynk.virtualWrite(V5, h);  //Thiết lập nút hiển thị độ ẩm trên Blynk (Viral pin 5)
  Blynk.virtualWrite(V6, t);  //Thiết lập nút hiển thị nhiệt độ trên Blynk (Viral pin 6)
}
