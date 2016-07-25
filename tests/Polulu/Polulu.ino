#define NorthIRBeacon A2
#define EastIRBeacon A3
#define SouthIRBeacon A4
#define WestIRBeacon A5

void setup() {
  Serial.begin(9600);
   //pololu IR Beacon
  pinMode(NorthIRBeacon, INPUT);
  pinMode(EastIRBeacon, INPUT);
  pinMode(SouthIRBeacon, INPUT);
  pinMode(WestIRBeacon, INPUT);

}

void loop() {
  int North = digitalRead(NorthIRBeacon);
  int East = digitalRead(EastIRBeacon);
  int South = digitalRead(SouthIRBeacon);
  int West = digitalRead(WestIRBeacon);
  
  Serial.print("North: ");
  Serial.print(North);
  Serial.print("    East: ");
  Serial.print(East);
  Serial.print("    South: ");
  Serial.print(South);
  Serial.print("    West: ");
  Serial.println(West);
  delay(500);

}


