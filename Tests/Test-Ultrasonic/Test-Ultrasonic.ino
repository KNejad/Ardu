//description:sketch createdd to check whether the ultrasonic pins work


#define LeftUltrasonicTrig 50
#define LeftUltrasonicEcho 51
#define FrontUltrasonicTrig 48
#define FrontUltrasonicEcho 49
#define RightUltrasonicTrig 52
#define RightUltrasonicEcho 53
int Val = 76;
int Distance = 999;

void setup() {
  Serial.begin(9600);
  pinMode(FrontUltrasonicTrig, OUTPUT);
  pinMode(FrontUltrasonicEcho, INPUT);
  pinMode(LeftUltrasonicTrig, OUTPUT);
  pinMode(LeftUltrasonicEcho, INPUT);
  pinMode(RightUltrasonicTrig, OUTPUT);
  pinMode(RightUltrasonicEcho, INPUT);
}
/* ------------------------------(LOOP)------------------------------*/
// use buttons R C L
void loop() {
  if (Serial.available() > 0) {
    Val = Serial.read();
  }
  if (Val == 76) {
    Distance = LookLeft();
    Serial.print("Left Distance");
  }
  if (Val == 67){
    Distance = LookForward();
    Serial.print("Center Distance");
  }
  if (Val == 82) {
    Distance = LookRight();
    Serial.print("Right Distance");
  }
  Serial.print(": ");
  Serial.println(Distance);
  delay(250);
}

int LookForward() {
  int Centimeters = UltrasonicCheckDistance(FrontUltrasonicTrig, FrontUltrasonicEcho);
  return Centimeters;
}
int LookLeft() {
  int Centimeters = UltrasonicCheckDistance(LeftUltrasonicTrig, LeftUltrasonicEcho);
  return Centimeters;
}
int LookRight() {
  int Centimeters = UltrasonicCheckDistance(RightUltrasonicTrig, RightUltrasonicEcho);
  return Centimeters;
}

int UltrasonicCheckDistance(int Trig, int Echo) {
  delay(50); // the delay is to verify that a previous check won't interfier with this one
  long Duration;
  // The sensor is triggered by a HIGH pulse of 10 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  digitalWrite(Trig, LOW);
  delayMicroseconds(2);
  digitalWrite(Trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(Trig, LOW);
  Duration = pulseIn(Echo, HIGH);
  // convert the time into a distance
  int Centimeters = Duration / 29 / 2;
  return Centimeters;
}

