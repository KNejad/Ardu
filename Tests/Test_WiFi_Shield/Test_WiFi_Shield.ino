#include <WiFi.h>
#include <SPI.h>


// Initialize the client library
WiFiClient client;

char ssid[] = "ote123";     //  your network SSID (name) 
char pass[] = "2421087610f";  // your network password
int status = WL_IDLE_STATUS;     // the Wifi radio's status


char server[] = "www.knejad.co.uk";

void setup(){
  Serial.begin(9600); 

  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present"); 
    while(true);
  } 

  // attempt to connect to Wifi network:
  while ( status != WL_CONNECTED) { 
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network:    
    status = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
    delay(10000);
  }


  // you're connected now, so print out the data:
  Serial.println("You're connected to the network");
  
  
  
   if (client.connect(server, 80)) {
    Serial.println("connected");
    // Make a HTTP request:
    client.println("GET /ardu-server/ardu_compact.txt");
    client.println("Host: www.knejad.co.uk");
    client.println("Connection: close");
    client.println();
  } else {
    // if you didn't get a connection to the server:
    Serial.println("connection to server failed");
  }
}

void loop(){
  // if there are incoming bytes available
  // from the server, read them and print them:
  if (client.available()) {
    char c = client.read();
    Serial.print(c);
  }

  // if the server's disconnected, stop the client:
  if (!client.connected()) {
    Serial.println();
    Serial.println("disconnecting.");
    client.stop();

    // do nothing forevermore:
    while (true);
  }
}


