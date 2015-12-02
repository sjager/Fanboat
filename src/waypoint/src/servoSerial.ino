#include <Servo.h>

Servo servo1;
Servo servo2;
int val;
int ang;

void setup()
{
  servo1.attach(3);
  servo2.attach(5);
  Serial.begin(9600);
}

void loop() 
{ 
  while (Serial.available() > 0) {
    val = Serial.parseInt();
    val = constrain(val, 0, 360);
    ang = val/2;
  }
  servo1.write(ang);
  if((val%2) == 0) {
    servo2.write(ang);
  } else {
    servo2.write(ang+1);
  }
  delay(15); 
  Serial.println(val, DEC);
} 
