#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

ESP8266WebServer server;

char ssid[] = "FRITZ!Box 7530 QL";
char pass[] = "89308086050155047149";

#define SENSOR  4

long currentMillis = 0;
long previousMillis = 0;
int interval = 1000;
boolean ledState = LOW;
float calibrationFactor = 4.5;
volatile byte pulseCount;
byte pulse1Sec = 0;
float flowRate;
unsigned int flowMilliLitres;
unsigned long totalMilliLitres;
int total;


void IRAM_ATTR pulseCounter()
{
  pulseCount++;
}

 
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

Serial.begin(9600);

  pinMode(SENSOR, INPUT_PULLUP);

  pulseCount = 0;
  flowRate = 0.0;
  flowMilliLitres = 0;
  totalMilliLitres = 0;
  previousMillis = 0;

 

  attachInterrupt(digitalPinToInterrupt(SENSOR), pulseCounter, FALLING);


server.on("/",[](){server.send(200,"text/plain","");});
server.begin();
}


void loop() {
  // put your main code here, to run repeatedly:
server.handleClient();


  currentMillis = millis();
  if (currentMillis - previousMillis > interval) {
    
    pulse1Sec = pulseCount;
    pulseCount = 0;

    // Because this loop may not complete in exactly 1 second intervals we calculate
    // the number of milliseconds that have passed since the last execution and use
    // that to scale the output. We also apply the calibrationFactor to scale the output
    // based on the number of pulses per second per units of measure (litres/minute in
    // this case) coming from the sensor.
    flowRate = ((1000.0 / (millis() - previousMillis)) * pulse1Sec) / calibrationFactor;
    previousMillis = millis();

    // Divide the flow rate in litres/minute by 60 to determine how many litres have
    // passed through the sensor in this 1 second interval, then multiply by 1000 to
    // convert to millilitres.
    flowMilliLitres = (flowRate / 60) * 1000;

    // Add the millilitres passed in this second to the cumulative total
    totalMilliLitres += flowMilliLitres;
    
    // Print the flow rate for this second in litres / minute
    Serial.print("Flow rate: ");
    Serial.print(int(flowRate));  // Print the integer part of the variable
    Serial.print("L/min");
    Serial.print("\t");       // Print tab space

    // Print the cumulative total of litres flowed since starting
    Serial.print("Output Liquid Quantity: ");
    Serial.print(totalMilliLitres);
    Serial.print("mL / ");
    Serial.print(totalMilliLitres / 1000);
    Serial.println("L");

  }
}
