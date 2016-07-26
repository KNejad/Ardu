#include <SPI.h>
#include <WiFi.h>

char ssid[] = "ote123"; //  your network SSID (name)
char pass[] = "password";    // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;            // your network key Index number (needed only for WEP)

int status = WL_IDLE_STATUS;

char server[] = "www.knejad.co.uk";    // name address for server (using DNS)

String response = "";


// Initialize the Ethernet client library
// with the IP address and port of the server
// that you want to connect to (port 80 is default for HTTP):
WiFiClient client;

void setup() {
  //Initialize serial
  Serial.begin(9600);


  // check for the presence of the shield:
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
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
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
    delay(10000);
  }
  Serial.println("Connected to wifi");

  makeGet();
}

void loop() {
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
      if ( instruction == 'F') {
        Serial.print("Moving Forwards:");
      } else {
        if ( instruction == 'L') {
          Serial.print("Turning Left:");
        } else {
          if ( instruction == 'R') {
            Serial.print("Turning Right:");
          } else {
            if ( instruction == 'B') {
              Serial.print("Moving Backwards:");
            }
          }
        }
      }

      command = command.substring(1);
      int cut = command.indexOf(";");
      int amount =  command.substring(0, cut).toInt();
      Serial.println(amount);
      command = command.substring(cut + 1);
      if (command.length() < 1) {
        done = true;
      }
    }
    //do nothing for 20 seconds then start over
    delay(20000);
    makeGet();
  }
}

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

