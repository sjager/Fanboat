#include <Servo.h>


Servo servo1;
Servo servo2;
byte dataIn;
int val;
int ang;

void setup()
{
  servo1.attach(3);
  servo2.attach(5);
  Serial.begin(9600);
  pinMode(13, OUTPUT);
}

void loop() 
{ 
  while (Serial.available() > 0) {
    dataIn = Serial.read();
    val = int(dataIn);
    Serial.println(val, DEC);
    val = constrain(val, 0, 180);
  }
  servo1.write(val);
//  if((val%2) == 0) {
//    servo2.write(ang);
//  } else {
//    servo2.write(ang+1);
//  }
  delay(15); 
  //Serial.println(val, DEC);
} 
