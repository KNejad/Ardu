//build number:0008
//version created on the: 19/7/2016
//DESCRIPTION: fully functioning vehicle with an lcd display showing relavent info. Running on an Arduino Mega.  With a pololu sensor.


//Changes from last build:
//Replaced Uno and Due with Mega
//Polulu turns off when not used

//equipment used:
//2 servos half rotation converted to full rotation by sticking the potentiometer to 90 degrees
//3 Ultrasonic sensors in front. With a few resistors in order to work with the arduino due
//1 lcd display with a wire underneath for the buttons to work.
//1 pololu IR Beacon
//1 Arduino Mega
//1 bluetooth tranciever

//for the sharp IRsensor on the back of the arduino
//#include <SharpIR.h>
//#define BackIRSensor A0
//#define IrSensorModel 1080
//SharpIR sharp(BackIRSensor, 25, 93, IrSensorModel);

//Bluetooth:
//App: Arduino Bluetooth Controller (Estado's ltd.)
//Up: U
//Left: L
//Right: R
//Down: D
//Bluetooth Mode: B
//Follow Mode: F
//Explore Mode: E

#define LeftUltrasonicTrig  50 //was 8
#define LeftUltrasonicEcho  51 //was 9
#define FrontUltrasonicTrig 48 //was 10
#define FrontUltrasonicEcho 49 //was 11
#define RightUltrasonicTrig 52 //was 12
#define RightUltrasonicEcho 53 //was 13

#define NorthIRBeacon 24 //was A2
#define EastIRBeacon 26  //was A3
#define SouthIRBeacon 28 //was A4
#define WestIRBeacon 30  //was A5

//the 5 buttons on the lcd shield used for changing mode
#define AnalogButton A0

#define PoluluPower 32
#define PoluluE 33

#include <LiquidCrystal.h>

// initialize the lcd with the numbers of the interface pins
LiquidCrystal lcd(7, 6, 5, 4, 3, 2);

#include<Servo.h>
Servo LeftWheel;
Servo RightWheel;

//to change between explore mode and follow mode
boolean FollowMode = false;
boolean ExploreMode = true;
boolean BluetoothMode = false;

int DangerDistance = 10;

void setup() {

  //first thing it does is stop just incase the servos where still moving from before
  Stop();

  LeftWheel.attach(8);
  RightWheel.attach(9);

  pinMode(AnalogButton, INPUT);

  pinMode(FrontUltrasonicTrig, OUTPUT);
  pinMode(FrontUltrasonicEcho, INPUT);
  pinMode(LeftUltrasonicTrig, OUTPUT);
  pinMode(LeftUltrasonicEcho, INPUT);
  pinMode(RightUltrasonicTrig, OUTPUT);
  pinMode(RightUltrasonicEcho, INPUT);

  //pololu IR Beacon
  pinMode(NorthIRBeacon, INPUT);
  pinMode(EastIRBeacon, INPUT);
  pinMode(SouthIRBeacon, INPUT);
  pinMode(WestIRBeacon, INPUT);

  lcd.begin(16, 2);

  Serial.begin(9600);
}

/* ------------------------------(LOOP)------------------------------*/

void loop() {
  if (Serial.available() > 0 && !BluetoothMode) {
    char Character = Serial.read();

    if (Character == 'B') {
       Stop();
      lcd.clear();
      lcd.println("Bluetooth Mode");
      delay(1000);
      FollowMode = false;
      ExploreMode = false;
      BluetoothMode = true;
    }
    if (Character == 'F') {
       Stop();
      lcd.clear();
      lcd.println("Follow Mode");
      delay(1000);
      FollowMode = true;
      ExploreMode = false;
      BluetoothMode = false;
    }
    if (Character == 'E') {
       Stop();
      lcd.clear();
      lcd.println("Explore Mode");
      delay(1000);
      FollowMode = false;
      ExploreMode = true;
      BluetoothMode = false;
    }
  }


    if (analogRead(AnalogButton) < 40) {
      Stop();
      lcd.clear();
      lcd.println("Follow Mode");
      delay(1000);
      FollowMode = true;
      ExploreMode = false;
      BluetoothMode = false;
    }
    if (100 < analogRead(AnalogButton) && analogRead(AnalogButton) < 260  ) {
      Stop();
      lcd.clear();
      lcd.println("Explore Mode");
      delay(1000);
      FollowMode = false;
      ExploreMode = true;
      BluetoothMode = false;
    }
    if (300 < analogRead(AnalogButton) && analogRead(AnalogButton) < 500  ) {
      Stop();
      lcd.clear();
      lcd.println("Bluetooth Mode");
      delay(1000);
      FollowMode = false;
      ExploreMode = false;
      BluetoothMode = true;
    }

    if (FollowMode) {
      digitalWrite(PoluluPower , HIGH); //Turns the Polulu sensor on
      digitalWrite(PoluluE , HIGH); //Turns the Polulu E pin on
      InFollowMode();
    }
    if (ExploreMode) {
      digitalWrite(PoluluPower , LOW);
      digitalWrite(PoluluPower , LOW);
      InExploreMode();
    }
    if (BluetoothMode) {
      digitalWrite(PoluluPower , LOW);      
      digitalWrite(PoluluE , LOW);
      InBluetoothMode();
    }
  }

  /* ------------------------------(Modes)------------------------------*/

  void InBluetoothMode() {
    if (Serial.available() > 0) {
      char Character = Serial.read();

    if (Character == 'B') {
       Stop();
      lcd.clear();
      lcd.println("Bluetooth Mode");
      delay(1000);
      FollowMode = false;
      ExploreMode = false;
      BluetoothMode = true;
    }
    if (Character == 'F') {
       Stop();
      lcd.clear();
      lcd.println("Follow Mode");
      delay(1000);
      FollowMode = true;
      ExploreMode = false;
      BluetoothMode = false;
    }
    if (Character == 'E') {
       Stop();
      lcd.clear();
      lcd.println("Explore Mode");
      delay(1000);
      FollowMode = false;
      ExploreMode = true;
      BluetoothMode = false;
    }
    
      if (Character == 'L') {
        Turn(1, true);
        lcd.clear();
        lcd.println("Left");
      }
      if (Character == 'R') {
        Turn(1, false);
        lcd.clear();
        lcd.println("Right");
      }
      if (Character == 'U') {
        lcd.clear();
        lcd.println("Forwards");
        MoveForward();
      }
      if (Character == 'D') {
        lcd.clear();
        lcd.println("Backwards");
        MoveBackwards();
      }
      delay(150); //gives time for the next command to come through
    } else {
      Stop();
    }




  }

  void InFollowMode() {

    char BeaconLocation = LookForBeacon();

    if (BeaconLocation == 'N') {

      //Serial.println("NORTH");
      if (LookForward() > DangerDistance && LookLeft() > DangerDistance && LookRight() > DangerDistance) {
        lcd.clear();
        lcd.print("I'm coming");
        MoveForward();
      }
      else {
        lcd.clear();
        lcd.print("I'll try to come");
        Stop();
        if (LookForward() < DangerDistance) {
          if (LookLeft() > LookRight()) {
            Turn(1, true);

          } else {
            Turn(1, false);
          }
          if (LookForward() > DangerDistance && LookLeft() > DangerDistance && LookRight() > DangerDistance) {
            MoveForward();
            delay(500);
            Stop();
          }
        }
        else if (LookLeft() < DangerDistance) {
          Turn(1, false);
        } else {
          Turn(1, true);
        }
      }
      return;
    }

    if (BeaconLocation == 'E') {
      //Serial.println("East");
      lcd.clear();
      lcd.print("To my right");
      TurnTillNorth(false);
      return;
    }
    if (BeaconLocation == 'W') {
      //Serial.println("West");
      lcd.clear();
      lcd.print("To my left");
      TurnTillNorth(true);
      return;
    }
    if (BeaconLocation == 'S') {
      //Serial.println("South");
      lcd.clear();
      lcd.print("Behind me");
      TurnTillNorth(true);
      return;
    }
    if (BeaconLocation == 'O') {
      Serial.println("UNKNOWN");
      Stop();
      lcd.clear();
      lcd.print("Where are you?");
    }
  }

  void InExploreMode() {
    delay(100);//makes the lcd refresh smoother

    int Left = LookLeft();
    int Right = LookRight();
    int Forward = LookForward();

    PrintDistance(Left, Forward, Right);

    if (Left > DangerDistance && Forward > DangerDistance && Right > DangerDistance) {
      MoveForward();
      lcd.setCursor(0, 0);
      lcd.print("Moving forward!");
    } else {
      if (Forward < 30) {
        Stop();
        if (Left > Right)
          Turn(500, true); //turn Left 500 milliseconds
        else
          Turn(500, false); //turn right 500 milliseconds
      }
      if (Left < 30) Turn(250, false);
      if (Right < 30)Turn(250, true);
    }
  }

  /* ------------------------------(Moving)------------------------------*/
  void MoveForward() {
    //they are different directions because the servos are looking in the opposite direction
    LeftWheel.write(180);//turns clockwise constantly;
    RightWheel.write(0);//turns counter-clockwise constantly;
    //Serial.println ("--Moving--");
  }
  void MoveBackwards() {
    //they are different directions because the servos are looking in the opposite direction
    LeftWheel.write(0);//turns clockwise constantly;
    RightWheel.write(180);//turns counter-clockwise constantly;
    //Serial.println ("--Moving Backwards--");
  }

  void Stop() {
    //Serial.print("Stoping...");
    LeftWheel.write(90);//stops left wheel
    RightWheel.write(90);//stops right wheel
    //Serial.println ("Done");
  }

  //accepts the amount of time to turn for and the direction to turn (if true it turns left else right)
  void Turn(int TurningTime, boolean TurnLeft) {
    int val = 90;
    //Serial.print("Turning...");

    //if TurnLeft is true turns left else turns right
    if (TurnLeft) {
      val = 0;
    }
    else {
      val = 180;
    }
    LeftWheel.write(val);
    RightWheel.write(val);

    //delays until complete without updating lcd (only for short turns <100 ms)
    if (TurningTime < 100) {
      delay(TurningTime);
    } else {
      //waits until turn completes while updating lcd
      int Time = millis();
      int TimeTillFinish = TurningTime + Time;

      while (Time < TimeTillFinish) {
        if (TurnLeft) {
          lcd.setCursor(0, 0);
          lcd.print("                ");
          lcd.setCursor(0, 0);
          lcd.print("Turn left: " + String(TimeTillFinish - Time));
        }
        else {
          lcd.setCursor(0, 0);
          lcd.print("                ");
          lcd.setCursor(0, 0);
          lcd.print("Turn right: " + String(TimeTillFinish - Time));
        }
        Time = millis();
        int Left = LookLeft();
        int Right = LookRight();
        int Forward = LookForward();
        PrintDistance(Left, Forward, Right);
        delay(100);
      }
    }
    //Serial.println ("Done");
  }

  //accepts the Direction which it should turn in (True=Left False=Right)
  void TurnTillNorth(boolean Direction) {
    boolean North = digitalRead(NorthIRBeacon);

    int Timer = 0;
    while (North && Timer < 100) {
      North = digitalRead(NorthIRBeacon);
      Turn(1, Direction);
      Timer++;
    }
  }
  /* ------------------------------(Looking)------------------------------*/

  //returns the letter of the direction N E S W or O if it cant determine the location of the beacon
  char LookForBeacon() {

    //checks the distances 5 times to get an average of where is the best option
    int NorthCounter = 0;
    int EastCounter = 0;
    int SouthCounter = 0;
    int WestCounter = 0;
    for (int i = 0; i < 5; i++) {
      delay(50);
      if (!digitalRead(NorthIRBeacon))
        NorthCounter++;
      if (!digitalRead(EastIRBeacon))
        EastCounter++;
      if (!digitalRead(SouthIRBeacon))
        SouthCounter++;
      if (!digitalRead(WestIRBeacon))
        WestCounter++;
    }

    if (NorthCounter > EastCounter && NorthCounter > SouthCounter && NorthCounter > WestCounter)
      return 'N';
    else if ((EastCounter > NorthCounter && EastCounter > SouthCounter && EastCounter > WestCounter) ||
             (NorthCounter == EastCounter && NorthCounter != 0) ||
             (SouthCounter == EastCounter && SouthCounter != 0))
      return 'E';
    else if (SouthCounter > EastCounter && SouthCounter > NorthCounter && SouthCounter > WestCounter)
      return 'S';
    else if ((WestCounter > EastCounter && WestCounter > SouthCounter && WestCounter > NorthCounter)  ||
             (NorthCounter == WestCounter && NorthCounter != 0) ||
             (SouthCounter == WestCounter && SouthCounter != 0))
      return 'W';
    else
      return 'O';
  }

  int LookForward() {
    int Centimeters = UltrasonicCheckDistance(FrontUltrasonicTrig, FrontUltrasonicEcho);
    //Serial.print("Centimeters To obstacle:  ");
    //Serial.println(Centimeters);
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
    // delay(50); // the delay is to verify that a previous check won't interfier with this one
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

  /* ------------------------------(Printing to lcd)------------------------------*/

  void  PrintDistance(int Left, int Forward, int Right) {
    //clears the second line
    lcd.setCursor(0, 1);
    lcd.print("                ");
    lcd.setCursor(0, 1);
    String Distances = "L" + String(Left) + " C" + String(Forward) + " R" + String(Right);
    lcd.print(Distances);
  }
