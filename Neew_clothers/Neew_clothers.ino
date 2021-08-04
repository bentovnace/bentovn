#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE); 
#define sensorPin1 7
#define sensorPin2 4

#define relay 3

int sensorState1 = 0;
int sensorState2 = 0;
int count=0;


















#define OutputDO 8 //Thiết lập chân số 8 là đầu ra của DO cảm biến mưa
int Ldr = A1; //Thiết lập A1 là chân analog của Cảm biến ánh sáng 
int ldr;
int en1= 6; //Thiết lập chân digital 6 làm chân kết nối với ena A trên L298N
int en2= 5; //Thiết lập chân digital 5 làm chân kết nối với ena B trên L298N
int in1= 2;  //Thiết lập đầu vào in1 trên L298N là chân digital 2 trên arduino
int in2= 12; //Thiết lập đầu vào in2 trên L298N là chân digital 12 trên arduino
int in3= 11; //Thiết lập đầu vào in3 trên L298N là chân digital 11 trên arduino
int in4= 10; //Thiết lập đầu vào in4 trên L298N là chân digital 10 trên arduino

int OutputAO=A0; // Thiết lập A0 là chân analog của cảm biến mưa
int Led=13; //Thiết lập chân cắm của đèn led điều khiển


int val;//Thiết lập cơ sở dữ liệu
float sensor; //Khai báo dữ liệu của cảm biến dưới dạng số thực

  void setup() {



    
pinMode (sensorPin1, INPUT_PULLUP);
  pinMode (sensorPin2, INPUT_PULLUP);
  pinMode(relay, OUTPUT);
  
  lcd.begin(16,2);
  lcd.backlight();
  lcd.setCursor(4,0);
  lcd.print("COUNTER");
   lcd.setCursor(0,1);
  lcd.print("No Visitors     ");
  delay(200);
  
    Serial.begin(9600); // Mở cổng kết bối với aruduino





   
  pinMode(en1, OUTPUT); //Xác định đầu ra cho en1 OUTPUT
  pinMode(en2, OUTPUT); //Xác định đầu ra cho en2 OUTPUT
  pinMode(in1, OUTPUT); //Xác định đầu ra cho in1 OUTPUT
  pinMode(in2, OUTPUT); //Xác định đầu ra cho in2 OUTPUT
  pinMode(in3, OUTPUT); //Xác định đầu ra cho in3 OUTPUT
  pinMode(in4, OUTPUT); //Xác định đầu ra cho in4 OUTPUT

pinMode (Led, OUTPUT) ; //Xác định đầu rá của led
pinMode (OutputDO, INPUT) ; //Xác định đầu vào OutputDO
pinMode (OutputAO, INPUT) ; //Xác định đầu vào OutputAO
pinMode (ldr, INPUT); //Xác định đầu vào của cảm biến ánh sáng 
// Bắt đầu khởi tạo giao tiếp với các cảm biến 
Serial.begin(9600);
  
}
// Đối với trường hợp mưa
void Rain(){           
  digitalWrite (Led, HIGH); // Bật led
// Cài đặt hoạt động L298N, cấp nguồn cho các chân.
  analogWrite(en1,125); 
  digitalWrite(in1,LOW);
  digitalWrite(in2,HIGH);

  analogWrite(en2,125);
  digitalWrite(in3,LOW);
  digitalWrite(in4,HIGH);

  Serial.println("Kha thi");
  
}
void Cloudy(){
  digitalWrite (Led, HIGH);

  analogWrite(en1,125);
  digitalWrite(in1,LOW);
  digitalWrite(in2,HIGH);

  analogWrite(en2,125);
  digitalWrite(in3,LOW);
  digitalWrite(in4,HIGH);

  Serial.println("Khả thi");
}
void light(){
  digitalWrite (Led, LOW);

  analogWrite(en1,125);
  digitalWrite(in1,HIGH);
  digitalWrite(in2,LOW);

  analogWrite(en2,125);
  digitalWrite(in3,HIGH);
  digitalWrite(in4,LOW);

  Serial.println("lên");
}
void death(){
  analogWrite(en1,0);
  digitalWrite(in1,LOW);
  digitalWrite(in2,LOW);

  analogWrite(en2,0);
  digitalWrite(in3,LOW);
  digitalWrite(in4,LOW);

  Serial.println("Chet");
  delay(5000);
}

void loop() {



  
sensorState1 = digitalRead(sensorPin1);
  sensorState2 = digitalRead(sensorPin2);

  if(sensorState1 == LOW){
    count++; 
    delay(500);
  }

  if(sensorState2 == LOW){
    count--; 
    delay(500);
  }
  

   if(count<=0)
  {
    digitalWrite(relay, LOW); 
    lcd.setCursor(0,1);
    lcd.print("No visitors    ");
  }
  else if (count>0 && count<10){
    digitalWrite(relay, HIGH);
    lcd.setCursor(0,1);
    lcd.print("Visitors:   ");
    lcd.setCursor(12,1);
    lcd.print(count);
    lcd.setCursor(13,1);
    lcd.print("  "); 
    
  }
  else {
    digitalWrite(relay, HIGH);
    lcd.setCursor(0,1);
    lcd.print("Visitors:   ");
    lcd.setCursor(12,1);
    lcd.print(count);  
  }
  






 
 
sensor = analogRead(OutputAO);//Đọc giá trị cảm biến ở chân AO
Serial.println(sensor); //Hiển thị kết quả đọc cảm biến trên màn hình máy tính

ldr = analogRead(A1);
Serial.print("ldr : ");
Serial.println(ldr);// Hiện thị giá trị cảm biến 
//Đọc giá trị của cảm biến ánh sáng (logig 1 và 0)
val = digitalRead (OutputDO);

//Các trường hợp
 if (ldr >= 250 )  
 {                  //Nếu giá trị cảm biến ánh sáng bé hơn hoặc bằng 250 và giá trị cảm biến mùa bé hơn hoặc bằng 500 thì đèn led sẽ sáng/
  if (sensor <=500)
    // Đây là trường hợp mưa
   
    Rain(); delay(500);
    death(); delay(5000);
   
 }
//Nếu giá trị của cảm biến ánh sáng bé hơn hoặc bằng 250  và giá trị cảm biến mưa lớn hơn 500
 else if(ldr >= 250 ){
  if (sensor > 500){          //Đây là trường hợp nhiều mây
   Cloudy(); delay(400);
   death(); delay(5000);
 }
 }
 // Nếu giá trị cảm biến ánh sáng lớn hơn hoặc bằng 250 cà giá trị cảm biến myaw lớn hơn 500 
else if(ldr <= 250 ){
  if (sensor > 500){        // Đây là trường hợp nắng 
   light(); delay(1000);
   death(); delay(5000);
 }
 }
}
