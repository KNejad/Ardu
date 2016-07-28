//distance sensors
#define LeftUltrasonicTrig  46
#define LeftUltrasonicEcho  47
#define FrontUltrasonicTrig 48
#define FrontUltrasonicEcho 49
#define RightUltrasonicTrig 38
#define RightUltrasonicEcho 39


//polulu beacons
#define NorthIRBeacon 24
#define EastIRBeacon 26
#define SouthIRBeacon 28
#define WestIRBeacon 30

//the 5 buttons on the lcd shield used for changing mode
#define AnalogButton A0

//Power and E pins of polulu sensor
#define PoluluPower 32
#define PoluluE 33

// Arduino Wire library is required if I2Cdev I2CDEV_ARDUINO_WIRE implementation
// is used in I2Cdev.h (Compass)
#include "Wire.h"

// I2Cdev and HMC5883L must be installed as libraries, or else the .cpp/.h files
// for both classes must be in the include path of your project
#include "I2Cdev.h"
#include "HMC5883L.h"

// class default I2C address is 0x1E
// specific I2C addresses may be passed as a parameter here
// this device only supports one I2C address (0x1E)
HMC5883L mag;

//to use for the raw data of the compass
int16_t mx, my, mz;


//For wifi mode
#include <SPI.h>
#include <WiFi.h>

char ssid[] = "ssid"; //  your network SSID (name)
char pass[] = "pass";    // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;            // your network key Index number (needed only for WEP)

int status = WL_IDLE_STATUS;

char server[] = "www.knejad.co.uk";    // name address for server (using DNS)

//response from the server
String response = "";

boolean WiFiReady = false;

// Initialize the Ethernet client library
// with the IP address and port of the server
// that you want to connect to (port 80 is default for HTTP):
WiFiClient client;



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
boolean WiFiMode = false;

//distance where it turns
int DangerDistance = 10;

void setup() {

  Serial.begin(9600);
  //first thing it does is stop just incase the servos where still moving from before
  Stop();

  //join I2C bus
  Wire.begin();
  //initialize devise
  mag.initialize();

  LeftWheel.attach(42);
  RightWheel.attach(43);

  //pinMode(AnalogButton, INPUT);

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
      WiFiMode = false;
    }
    if (Character == 'F') {
      Stop();
      lcd.clear();
      lcd.println("Follow Mode");
      delay(1000);
      FollowMode = true;
      ExploreMode = false;
      BluetoothMode = false;
      WiFiMode = false;
    }
    if (Character == 'E') {
      Stop();
      lcd.clear();
      lcd.println("Explore Mode");
      delay(1000);
      FollowMode = false;
      ExploreMode = true;
      BluetoothMode = false;
      WiFiMode = false;
    }
    if (Character == 'W') {
      Stop();
      lcd.clear();
      lcd.println("WiFi Mode");
      delay(1000);
      FollowMode = false;
      ExploreMode = false;
      BluetoothMode = false;
      WiFiMode = true;
    }
  }


  if ( analogRead(AnalogButton) < 40  ) {

    Stop();
    lcd.clear();
    lcd.println("Explore Mode");
    delay(1000);
    ExploreMode = true;
    FollowMode = false;
    BluetoothMode = false;
    WiFiMode = false;

  }
  if (100 < analogRead(AnalogButton) && analogRead(AnalogButton) < 260  ) {
    Stop();
    lcd.clear();
    lcd.println("Follow Mode");
    delay(1000);
    ExploreMode = false;
    FollowMode = true;
    BluetoothMode = false;
    WiFiMode = false;

  }
  if (300 < analogRead(AnalogButton) && analogRead(AnalogButton) < 500  ) {
    Stop();
    lcd.clear();
    lcd.println("Bluetooth Mode");
    delay(1000);
    ExploreMode = false;
    FollowMode = false;
    BluetoothMode = true;
    WiFiMode = false;

  }
  if (530 < analogRead(AnalogButton) && analogRead(AnalogButton) < 700  ) {

    Stop();
    lcd.clear();
    lcd.println("WiFi Mode");
    delay(1000);
    ExploreMode = false;
    FollowMode = false;
    BluetoothMode = false;
    WiFiMode = true;
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
  if (WiFiMode) {
    digitalWrite(PoluluPower , LOW);
    digitalWrite(PoluluE , LOW);
    if (!WiFiReady) {
      WiFiReady = true;
      // check for the presence of the shield:
      if (WiFi.status() == WL_NO_SHIELD) {
        Serial.println("WiFi shield not present");
        lcd.setCursor(0, 0);
        lcd.print("                ");
        lcd.setCursor(0, 0);
        lcd.print("WiFi shield not present");
        // don't continue:
        while (true);
      }

      String fv = WiFi.firmwareVersion();
      if (fv != "1.1.0") {
        Serial.println("Please upgrade the firmware");
      }

      // attempt to connect to Wifi network:
      while (status != WL_CONNECTED) {
        Serial.print("Attempting to connect to SSID: ");
        Serial.println(ssid);
        lcd.setCursor(0, 0);
        lcd.print("                ");
        lcd.setCursor(0, 0);
        lcd.print("Attempting to connect to SSID" + String(ssid));

        // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
        status = WiFi.begin(ssid, pass);

        // wait 10 seconds for connection:
        delay(10000);
      }
      Serial.println("Connected to wifi");
      lcd.setCursor(0, 0);
      lcd.print("                ");
      lcd.setCursor(0, 0);
      lcd.print("Connected to WiFi");
      makeGet();
    }
    InWiFiMode();
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
      Turn(0.1, true);
      lcd.clear();
      lcd.println("Left");
    }
    if (Character == 'R') {
      Turn(0.1, false);
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
        Turn(90, true); //turn left 90 degrees
      else
        Turn(90, false); //turn right 90 degrees
    }
    if (Left < 30) Turn(45, false);
    if (Right < 30)Turn(45, true);
  }
}

void InWiFiMode() {
  // if there are incoming bytes available
  // from the server, read them and print them:
  while (client.available()) {
    char c = client.read();
    response += c;
  }

  // if the server's disconnected, stop the client:
  if (!client.connected()) {
    Serial.println();
    Serial.println("disconnecting from server.");
    client.stop();

    int from = response.indexOf("BEGIN") + 5;
    String command = response.substring(from);
    boolean done = false;



    while (!done) {

      char instruction = command.charAt(0);
      command = command.substring(1);
      int cut = command.indexOf(";");
      int amount =  command.substring(0, cut).toInt();
      command = command.substring(cut + 1);

      if ( instruction == 'F') {
        MoveForward();
        delay(amount);
      } else {
        if ( instruction == 'L') {
          Turn(amount, true);
        } else {
          if ( instruction == 'R') {
            Turn(amount, false);
          } else {
            if ( instruction == 'B') {
              MoveBackwards();
              delay(amount);
            }
          }
        }
      }


      if (command.length() < 1) {
        done = true;
      }
    }
    //do nothing for 20 seconds then start over
    delay(20000);
    makeGet();
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
void Turn(float Degrees, boolean TurnLeft) {
  float DegreesNow, DegreesDifference, DegreesTo;
  //Serial.print("Turning...");
  //if TurnLeft is true turns left else turns right
  if (TurnLeft) {
    DegreesNow = comp(); //take the heading from comp()
    DegreesDifference = DegreesNow - Degrees; //find if the degreesTO is negative

    if (DegreesDifference < 0) {
      DegreesTo = 360 + DegreesDifference; //if it is negative make it positive
    } else {
      DegreesTo = DegreesDifference; //if not keep it as is
    }
    LeftWheel.write(0);
    RightWheel.write(0);
    while (DegreesNow >= DegreesTo) { // while you are not where you should be turn

      DegreesNow = comp();
    }
    Stop();
  } else { // if to turn right
    DegreesNow = comp(); //take the heading from comp()
    DegreesDifference = DegreesNow + Degrees; //find if the degreesTO is over 360
    if (DegreesDifference > 360) {
      DegreesTo = DegreesDifference - 360; //if it is make between 0-360
    } else {
      DegreesTo = DegreesDifference; // if its under 360 leave it as is
    }
    LeftWheel.write(180);
    RightWheel.write(180);
    while (DegreesNow <= DegreesTo) { // while you are not where you should be turn
      DegreesNow = comp();
    }
    Stop();
    Serial.println(" Stop");
  }


  /* while (Time < TimeTillFinish) {
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
    } */
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
/*-----------------------------(Compass)-------------------------------------------*/
//it returns the heading in degrees
float comp() {
  //int RoundHeading;
  // read raw heading measurements from device
  mag.getHeading(&mx, &my, &mz);

  // To calculate heading in degrees. 0 degree indicates North
  float heading = atan2(my, mx);
  if (heading < 0)
    heading += 2 * M_PI;
  heading = heading * 180 / M_PI;
  //RoundHeading = (int) heading + 0.5;
  return heading;
}


/*-----------------------------(WiFi)-------------------------------------------*/

void makeGet() {
  Serial.println("\nStarting connection to server...");
  // if you get a connection, report back via serial:
  if (client.connect(server, 80)) {
    Serial.println("connected to server");
    // Make a HTTP request:
    client.println("GET /ardu-server/compact.php HTTP/1.1");
    client.println("Host: www.knejad.co.uk");
    client.println("Connection: close\r\n\r\n");
  }
  response = "";
}


