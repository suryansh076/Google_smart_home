#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Servo.h>
char *ssid = "surya";
char *pass = "warmachine076";
char *broker = "io.adafruit.com";
char *username = "The_maheep";
char *keypass  = "aio_sssi39OIBCzSOMCzyVxVdNVkJUnR";
char msg[10];
char loff[] = {"OFF1"};
char moff[] = {"OFF2"};
char soff[] = {"OFF3"};
int l1 = D1;
int m1 = D2;
int buzz=D4;
int gass = A0;
int timee=0;
int gasvalue = 0;
Servo s1;
WiFiClient client1;
String receivedMessage;
PubSubClient client(client1);
void setup() {
  Serial.begin(115200);
  Serial.println("connecting");
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print("....");
    delay(500);
  }
  Serial.println("connected to network");
  pinMode(gass, OUTPUT);
  pinMode(l1, OUTPUT);
  pinMode(m1, OUTPUT);
 pinMode(buzz, OUTPUT);
  client.setServer(broker, 1883);
  client.setCallback(callback);
  timee = millis();
  s1.attach(D3);

}

void loop() {
  if (!client.connected())
  {
    if (client.connect("mqtt", username, keypass))
    {
      Serial.println("Connected to Server");
      client.subscribe("The_maheep/feeds/lighe1");
      client.subscribe("The_maheep/feeds/motor");
      client.subscribe("The_maheep/feeds/servo");
    }
  }
 if ((millis()-timee) >=3000)
 {
  Serial.println("hello");
   gasvalue=analogRead(gass);
   Serial.println(gasvalue);
  if (gasvalue < 500)
  {
    digitalWrite(buzz,0);
    client.publish("The_maheep/feeds/gass-monitor", "NO gass Leakage");
  }
  else if ((gasvalue > 600) && (gasvalue < 800))
  {
    analogWrite(buzz,150);
    client.publish("The_maheep/feeds/gass-monitor", "Low Leakage");
  }
  else if (gasvalue > 800)
  {
    analogWrite(buzz,200);
    client.publish("The_maheep/feeds/gass-monitor", "High leakage");
  }
   timee=millis();
  }
  client.loop();
  delay(500);
}

void callback(char* topic, byte* payload,  int lengths)//byte me char ki ascii value integer me aaygi to unko Type Cast kena padega to print in char Fomate
{
  for (int i = 0; i < lengths; i++)
  {
    receivedMessage = receivedMessage + char(payload[i]);
  }
  Serial.println(receivedMessage);
  if (receivedMessage == "ON1")
  {
    digitalWrite(l1,1);
    Serial.println("leight on");
  }
  else if (receivedMessage == "OFF1")
  {
    digitalWrite(l1,0);
    Serial.println("light off");
  }
  else if (receivedMessage == "ON2")
  {
    digitalWrite(m1,1);
    Serial.println("motor on");
  }
  else if (receivedMessage == "OFF2")
  {
    digitalWrite(m1,0);
    Serial.println("motor off");
  }
  else if (receivedMessage == "ON3")
  {
    s1.write(180);
    Serial.println("servo on");
  }
  else if (receivedMessage == "OFF3")
  {
    s1.write(0);
    Serial.println("servvo off");
  }

  receivedMessage = "";
  Serial.println("");
  delay(500);
}
