
#include <Servo.h> 
 
Servo myservoA;
Servo myservoB;
  
void setup() 
{
  myservoA.attach(8);
  myservoB.attach(9);
  Serial.begin(9600);
} 
 
void loop() 
{
    Serial.println("Start A");
    myservoA.write(180);
    delay(1000); 
    myservoA.write(0);    
    delay(1000);
    
Serial.println("Start B");
    myservoB.write(0);
    delay(1000); 
    myservoB.write(180);    
    delay(1000);
}

