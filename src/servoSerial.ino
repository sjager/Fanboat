#include <Servo.h>

Servo myservo;
int val;

void setup()
{
  myservo.attach(3);
  Serial.begin(9600);
}

void loop() 
{ 
  while (Serial.available() > 0) {
    val = Serial.parseInt();
    val = constrain(val, 0, 180);
  }
  myservo.write(val); 
  delay(15); 
  Serial.println(val, DEC);
} 
