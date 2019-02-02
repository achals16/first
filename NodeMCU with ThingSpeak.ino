#include <ESP8266WiFi.h>
#include "ThingSpeak.h"
#define led 2

#define trigger 13
#define echo 15

unsigned long startMillis;
unsigned long currentMillis = 0;

unsigned long myChannelNumber = channel_number;
const char* myWriteAPIKey = "your_api_key";

const char* ssid = "SSID";
const char* pass = "PASS";
const char* server = "api.thingspeak.com";

int ledState = 1;
int dist;
float timE = 0, distance = 0;
long interval = 10000;

WiFiClient client;

void setup()
{
  pinMode(trigger, OUTPUT);
  pinMode(echo, INPUT);
  Serial.begin(115200);
  WiFi.begin(ssid, pass);
  Serial.print("Connecting...");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.print(".");
  }
  pinMode(led, OUTPUT);
  digitalWrite(led, ledState);
  Serial.println("");
  Serial.println(WiFi.localIP());
  Serial.println("Connected");
  ThingSpeak.begin(client);

}

int ultraa()
{
  digitalWrite(trigger, LOW);
  delayMicroseconds(2);
  digitalWrite(trigger, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigger, LOW);
  delayMicroseconds(2);
  timE = pulseIn(echo, HIGH);
  distance = timE * 340 / 20000;
  return distance;
}

void loop()
{
  startMillis = millis();
  dist = ultraa();
  Serial.println(dist);
  if (startMillis - currentMillis > interval)
  {
    currentMillis = startMillis;
    //dist = ultraa();
    if (client.connect(server, 80))  //   "184.106.153.149" or api.thingspeak.com
    {
      ThingSpeak.setField(1, dist);
      ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
      //delay(10000);

      Serial.print("Distance in cm: ");
      Serial.print(distance);
      Serial.println("cm. Sent to Thingspeak.");
    }
    client.stop();
  }
  digitalWrite(led, LOW);
  delay(100);
  digitalWrite(led, HIGH);
  delay(100);
}
