//Khai báo thư viện servo
#include <Servo.h>
  //Tạo biến servo
 Servo myservo;
 //khai báo các biến dữ liệu và chân cắm
 const int ldrpin = A0; //Chân đọc dữ liệu của cảm biến ánh sáng là chân analog số 0
 const int servopin = 5
 ; //Chân kích xung để hoạt động servo là chân digital 8 trên board arduino

 int ldrvalue; //Khai báo kiểu dữ liệu cho cảm biến ánh sáng
 int goc = 0; //Khai báo kiểu dữ liệu cho biến góc ( chỉ góc quay của Servo )

 void setup()
 {
  Serial.begin(9600); // Mở cổng kết bối với aruduino
  
  myservo.attach ( servopin ); // Cổng kết nối của servo
 }
 void loop ()
{
  ldrvalue = analogRead(ldrpin); // Đọc dữ liệu của cảm biến ánh sáng ở chân analog 0 
  goc = map ( ldrvalue, 0, 1023, 0, 180); //Giới hạn giá trị của cảm biến ánh sáng nằm trong khoảng từ 180 - 1023
  Serial.println(goc);
  myservo.write(goc);
  delay (15);
}
