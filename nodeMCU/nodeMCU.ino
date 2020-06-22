#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

ESP8266WebServer server;

char ssid[] = "FRITZ!Box 7530 QL";
char pass[] = "89308086050155047149";

 
void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
delay(500);
Serial.print("Connecting to..");
Serial.println(ssid);
delay(500);

WiFi.disconnect();
WiFi.begin(ssid,pass);

while (WiFi.status() != WL_CONNECTED)
{
  delay(500);
  Serial.println(",");
}

Serial.print ("SSID....,");
Serial.println(WiFi.SSID());
Serial.println ("Successfully connected...");

Serial.print("IP Address allotted to NodeMCU ESP");
Serial.println(WiFi.localIP());

Serial.print("MAC address of ESP...");
Serial.println(WiFi.macAddress());
WiFi.printDiag(Serial);

server.on("/",[](){server.send(200,"text/plain","Hello World");});
server.begin();
}


void loop() {
  // put your main code here, to run repeatedly:
server.handleClient();
}
