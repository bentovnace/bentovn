#include <ESP8266HTTPClient.h> 
#include <Wire.h>                 
#include <LiquidCrystal_I2C.h>    
LiquidCrystal_I2C lcd(0x27,16,2);
#include "json_parser.h"
#include "WifiConnect.h"
#include <Blynk.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>


 //------------------------------------------------------------


 char auth[] = "ecvqWSNGw-7edDuq4b-luN_uCbqbkxJu";       
char ssid[] = "Luan";                      
char pass[] = "79797979";
 
#define inSensor 14 //D5
#define outSensor 12 //D6
 
int inStatus;
int outStatus;
 
int countin = 0;
int countout = 0;
 
int in;
int out;
int now;
 
#define relay 0 //D3
WidgetLED light(V25);



 //-----------------------------------------------------------
//Thiết lập các kết nối 
#define s2ms(second) (second*1000)
unsigned long long prev_millis(0);

#define country_code "Vietnam"


int interval = s2ms(60);
unsigned long long PreviousMillis = 0;
unsigned long long CurrentMillis = interval;
bool bFirstKickMillis = false;

extern bool bGotIpFlag;
//Xây dựng mã URL kết nối tới Server chứa dữ liệu , thông tin về tình hình Covid 19 
static String build_url_from_country(String country)
{
  String url = "http://coronavirus-19-api.herokuapp.com/countries/";
  url = url + country; // ulr = http://coronavirus-19-api.herokuapp.com/countries/Vietnam  Tìm kiếm địa chỉ tình hình Covid Việt nam theo ULR này 
  return url;
}

void setup(void)
{ 
//----------------------------------------------------------------


 Serial.begin(115200);
  Blynk.begin(auth, ssid, pass);//Truy cập dữ liệu mạng và kết nối tới Blynk
  delay(1000);  //Đợi 1 s
  
  pinMode(inSensor, INPUT);
  pinMode(outSensor, INPUT);
  pinMode(relay, OUTPUT);
  digitalWrite(relay, HIGH);

  Serial.println("Visitor Counter Demo");


  //--------------------------------------------------------------

  
  lcd.init();       
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Covid-19 Watch");
   lcd.setCursor(0, 1);
  lcd.print("Case In VietNam");

  #if defined JSON_DEBUG
    Serial.begin(9600);
  #endif
  
  JSON_LOG("Connecting...");
  
  vConnWifiNetworkViaSdk();
}

void loop()
{


  //------------------------------------------------------------------------------



   Blynk.run(); // Initiates Blynk
  inStatus =  digitalRead(inSensor);
  outStatus = digitalRead(outSensor);
  if (inStatus == 0)
  {
    in = countin++;
  }
 
  if (outStatus == 0)
  {
    out = countout++;
  }
 
  now = in - out;
 
  if (now <= 0)
  {
    digitalWrite(relay, HIGH);
    light.off();
   
  }
  else
  {
    digitalWrite(relay, LOW);
    light.on();
 
  }
  Blynk.virtualWrite(V0, relay);
  Blynk.virtualWrite(V7, in); // Visitors In
  Blynk.virtualWrite(V8, out); // Visitors Out
  Blynk.virtualWrite(V9, now); // Current Visitors
  delay(1000);


  //-------------------------------------------------
  if(bGotIpFlag) bGotIp();
  
  if(bFirstKickMillis) CurrentMillis = millis();
  
  if (!bGotIpFlag && CurrentMillis - PreviousMillis >= interval) 
  {
    if(!bFirstKickMillis) CurrentMillis = 0;
    
    bFirstKickMillis = true;
    
    PreviousMillis = CurrentMillis;
    
    HTTPClient http; 
    http.begin(build_url_from_country(country_code));
    
    int httpCode = http.GET(); 
  
    if(httpCode > 0) 
    {
      String payload = http.getString();
       
      char* JsonArray = (char *)malloc(payload.length() + 1);
      if (!JsonArray) JSON_LOG("upssss fuck");
      
      payload.toCharArray(JsonArray, payload.length() + 1);
      
      JSON_LOG(JsonArray);
      
      if (json_validate(JsonArray))
      {
        int confirmed = (int)get_json_value(JsonArray, "cases", INT);
        int deaths = (int)get_json_value(JsonArray, "deaths", INT);
        int recovered = (int)get_json_value(JsonArray, "recovered", INT);
      
        JSON_LOG(confirmed);
        JSON_LOG(deaths);
        JSON_LOG(recovered);
        
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
    
    http.end(); 
  }
}
