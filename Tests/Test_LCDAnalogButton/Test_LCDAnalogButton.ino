int Button =A0;

void setup() {
  pinMode(Button,INPUT);
  Serial.begin(9600);
}

void loop() {
  Serial.println(analogRead(Button));
  delay(250); 

}

