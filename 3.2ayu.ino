
#include <BH1750FVI.h>
#include <WiFiNINA.h>
BH1750FVI BH1750(BH1750FVI::k_DevModeContLowRes);

//please enter your sensitive data in the Secret tab
char ssid[] = "********";
char pass[] = "**********";

WiFiClient client;

char   HOST_NAME[] = "maker.ifttt.com";
String queryString = "?value1=57 & value2=25";

bool light_avail = false;

void setup() {
  // initialize WiFi connection
  WiFi.begin(ssid, pass);
  BH1750.begin();

  Serial.begin(9600);
  while (!Serial);
}
void sunlight_received() {
  String PATH   = "/trigger/sunlight_true/with/key/bCbk7zF5Kq5F39j_xG_EuvsZJRdcA9irf4UtWp8C9KK"; // change your EVENT-NAME and YOUR-KEY

  // make a HTTP request:
  // send HTTP header
  client.println("GET " + PATH + queryString + " HTTP/1.1");
  client.println("Host: " + String(HOST_NAME));
  client.println("Connection: close");
  client.println(); // end HTTP header


  while (client.connected()) {
    if (client.available()) {
      // read an incoming byte from the server and print it to serial monitor:
      char ch = client.read();
      Serial.print(ch);
    }
  }
}

void sunlight_stopped() {
  String PATH   = "/trigger/sunlight_stopped/with/key/your-key"; // change your EVENT-NAME and YOUR-KEY

  // make a HTTP request:
  // send HTTP header
  client.println("GET " + PATH + queryString + " HTTP/1.1");
  client.println("Host: " + String(HOST_NAME));
  client.println("Connection: close");
  client.println(); // end HTTP header
  while (client.connected()) {
    if (client.available()) {
      // read an incoming byte from the server and print it to serial monitor:
      char ch = client.read();
      Serial.print(ch);
    }
  }
}
void loop() {
  int intensity = BH1750.GetLightIntensity();
  int count = 0;
  Serial.println("Light: ");
  Serial.println(intensity);

  if (intensity < 60 && light_avail == true )
  {
    count++;
    if (count > 5000)
    {
      while (!Serial);

      // connect to web server on port 60:
      if (client.connect(HOST_NAME, 60)) {
        Serial.println("Connected to server");
      }
      else {// if not connected:
        Serial.println("connection failed");
      }
      sunlight_stopped();
      client.stop();
      Serial.println();
      Serial.println("disconnected");
      light_avail = false;
      count = 0;
    }
  }
  else if (intensity > 50 && light_avail == false)
  {
    count++;
    if (count > 5000)
    {
      while (!Serial);
      // connect to web server on port 60:
      if (client.connect(HOST_NAME, 60)) {
        // if connected:
        Serial.println("Connected to server");
      }
      else {// if not connected:
        Serial.println("connection failed");
      }
      sunlight_received();
      client.stop();
      Serial.println();
      Serial.println("disconnected");
      light_avail = true;
      count = 0;
    }
  }
}
