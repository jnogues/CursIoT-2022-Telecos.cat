//22_asyncMQTT-DS18B20-telecos.ino
//curs hivern 2022 telecos.cat
//jnogues@gmail.com
//asyncmqtt

#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <Ticker.h>
#include <AsyncMqttClient.h>
#include <Blynk.h>
BlynkTimer taskScheduler;//organitzador de tasques
#include <OneWire.h>
#include <DallasTemperature.h>

#define led_blau 16
#define WIFI_SSID     "********"
#define WIFI_PASSWORD "********"

#define MQTT_HOST IPAddress(5, 196, 88, 155)
#define MQTT_PORT 1883

#define IDdevice "rebedor"
#define TOPIC1   "/telecos/jaume/rebedor/uT"         //Per publicar dades
#define TOPIC2   "/telecos/jaume/rebedor/boot"       //Per publicar dades
#define TOPIC3   "/telecos/jaume/rebedor/outputs"    //Per publicar dades
#define TOPIC4   "/telecos/jaume/rebedor/a0"         //Per publicar dades
#define TOPIC5   "/telecos/jaume/rebedor/inputs"     //Per publicar dades
#define TOPIC6   "/telecos/jaume/rebedor/state"      //Per publicar dades
#define TOPIC7   "/telecos/jaume/rebedor/ds18b20"    //Per publicar dades
#define TOPIC10  "/telecos/jaume/rebedor/commands"   //Per rebre comandaments
#define TOPIC11  "/telecos/jaume/rebedor/pwm0"       //Per rebre comandaments

AsyncMqttClient mqttClient;
Ticker mqttReconnectTimer;

WiFiEventHandler wifiConnectHandler;
WiFiEventHandler wifiDisconnectHandler;
Ticker wifiReconnectTimer;

boolean stateGPIO16=0;
//boolean stateGPIO0=0;
//boolean prevstateGPIO0=0;
//boolean stateGPIO2=0;     //GPIO2 is used for wifi stat led
//boolean prevstateGPIO2=0;
boolean stateGPIO13=0;
boolean prevstateGPIO13=0;
boolean stateGPIO12=0;
boolean prevstateGPIO12=0;
boolean stateGPIO14=0;
boolean prevstateGPIO14=0;
int valuePWM0=0;

// Data wire is plugged into port 10 on the Arduino
#define ONE_WIRE_BUS 10
// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);
// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);
float temperatureDS = 0.0;

void setup() {
  Serial.begin(115200);
  delay(500);
  Serial.println(" ");
  Serial.println(" ");
  Serial.println("[boot] telecos.cat");
  Serial.println("[boot] MQTT-telecos.ino");
  
  //pinMode(0, OUTPUT);
  //digitalWrite(0,HIGH);
  pinMode(2, OUTPUT);
  digitalWrite(2,HIGH);
  pinMode(13, OUTPUT);
  digitalWrite(13,LOW);
  pinMode(15, OUTPUT);
  digitalWrite(15,LOW);
  pinMode(4, INPUT);
  pinMode(5, INPUT);
  pinMode(12, INPUT);
  pinMode(14, INPUT);
  pinMode(led_blau, OUTPUT);

  // Start up the library DS18B20
  sensors.begin();

  // Configurem CallBacks MQTT
  wifiConnectHandler = WiFi.onStationModeGotIP(onWifiConnect);
  wifiDisconnectHandler = WiFi.onStationModeDisconnected(onWifiDisconnect);
  mqttClient.onConnect(onMqttConnect);
  mqttClient.onDisconnect(onMqttDisconnect);
  mqttClient.onSubscribe(onMqttSubscribe);
  mqttClient.onUnsubscribe(onMqttUnsubscribe);
  mqttClient.onMessage(onMqttMessage);
  mqttClient.onPublish(onMqttPublish);
  mqttClient.setServer(MQTT_HOST, MQTT_PORT);

  // Connectem al Wifi
  connectToWifi();
  

  taskScheduler.setInterval(500L,   toggleLed);             //tasca 1
  taskScheduler.setInterval(10000L, publishUpTime);         //tasca 2
  //taskScheduler.setInterval(5000L,  publishA0);           //tasca 3
  //taskScheduler.setInterval(300L,   checkInputsOutputs);  //tasca 4
  taskScheduler.setInterval(15000L, publishState);          //tasca 5
  taskScheduler.setInterval(11000L, publishDS18B20);        //tasca 6
}

void loop() 
{
  taskScheduler.run();
}

//***********************TASQUES***********************

// tasca 1
void toggleLed()
{
  digitalWrite(led_blau, !digitalRead(led_blau));
}

// tasca 2
void publishUpTime()
{
  //unsigned long upTime = millis()/1000; //seconds
  //String payload = String(upTime);
  //mqttClient.publish(TOPIC1, 0, true, (char*)payload.c_str()); //QOS 0
  PublishMqtt(TOPIC1, millis()/1000);
}

// tasca 3
void publishA0()
{
  unsigned int analogA0=analogRead(A0);
  Serial.print("[ANG0] ");
  Serial.println(analogA0);
  PublishMqtt(TOPIC4, analogA0);
}

// tasca 4
void checkInputsOutputs()
{
  // input GPIO12
  stateGPIO12=digitalRead(12);
  if (stateGPIO12!=prevstateGPIO12)
  {
    if (stateGPIO12==0) mqttClient.publish(TOPIC5, 0, true, "I12off");;
    if (stateGPIO12==1) mqttClient.publish(TOPIC5, 0, true, "I12on");;
  }
  prevstateGPIO12=stateGPIO12;
}

// tasca 5
void publishState()
{
   String payload = "";
   StaticJsonDocument<300> jsonDoc;
   jsonDoc["id"]    = IDdevice;
   jsonDoc["uT"]    = millis()/1000;
   jsonDoc["A0"]    = analogRead(A0);
   jsonDoc["Q13"]   = digitalRead(13);
   jsonDoc["I12"]   = digitalRead(12);
   jsonDoc["PWM0"]  = valuePWM0;
   jsonDoc["DS18"]  = temperatureDS;
   jsonDoc["IP"]    = WiFi.localIP();
   serializeJson(jsonDoc, payload);
   mqttClient.publish(TOPIC6, 0, true, (char*)payload.c_str());
}

// tasca 6
void publishDS18B20()
{
  Serial.print("[DS18] Requesting temperatures...");
  sensors.requestTemperatures(); // Send the command to get temperatures
  Serial.println("DONE");
  // After we got the temperatures, we can print them here.
  // We use the function ByIndex, and as an example get the temperature from the first sensor only.
  temperatureDS = sensors.getTempCByIndex(0);

  // Check if reading was successful
  if(temperatureDS != DEVICE_DISCONNECTED_C) 
  {
    temperatureDS = int(temperatureDS * 100.0); // temperatura * 100, fora decimals
    Serial.print("[DS18]  ");
    Serial.println(temperatureDS);
    PublishMqtt(TOPIC7, temperatureDS);
  } 
  else
  {
    Serial.println("[DS18] Error: Could not read temperature data");
  }
}

//********************** Callbacks i Funcions ************************
void connectToWifi() {
  Serial.print("[WIFI] Connecting to Wi-Fi...");
  //WiFi.mode(WIFI_STA);
  //WiFi.disconnect();
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  delay(150);
  while (WiFi.status() != WL_CONNECTED) 
  { 
     delay(500);  
     Serial.print('.'); 
  }
  WiFi.setAutoConnect(true);
  WiFi.setAutoReconnect(true);
  Serial.println(" ");
}



void connectToMqtt() {
  Serial.println("[MQTT] Connecting to MQTT...");
  mqttClient.connect();
}

void onWifiConnect(const WiFiEventStationModeGotIP& event) {
  digitalWrite(2, LOW); //wifi ok, turn on led GPIO2
  Serial.println(" ");
  Serial.println("[WIFI] Connected to Wi-Fi.");
  Serial.print("[WIFI] Iniciado STA:\t");
  Serial.println(WIFI_SSID);
  Serial.print("[WIFI] IP address:\t");
  Serial.println(WiFi.localIP());
  connectToMqtt();
}

void onWifiDisconnect(const WiFiEventStationModeDisconnected& event) {
  digitalWrite(2, HIGH); //wifi ko, turn off led GPIO2
  Serial.println("[WIFI] Disconnected from Wi-Fi.");
  mqttReconnectTimer.detach(); // ensure we don't reconnect to MQTT while reconnecting to Wi-Fi
  //wifiReconnectTimer.once(2, connectToWifi);
}

void onMqttConnect(bool sessionPresent) {
  Serial.println("[MQTT] Connected to MQTT.");
  mqttClient.publish(TOPIC2, 0, true, "******* CONNECTED!! *******");
  //Serial.print("[MQTT] Session present: ");
  //Serial.println(sessionPresent);
  mqttClient.subscribe(TOPIC10, 0);
  mqttClient.subscribe(TOPIC11, 0);
  //Serial.print("[MQTT] Subscribing at QoS 0, packetId: ");
  //Serial.println(packetIdSub);
  //mqttClient.publish("/telecos/jaume/test", 0, true, "test 1");
  //Serial.println("[MQTT] Publishing at QoS 0");
  //uint16_t packetIdPub1 = mqttClient.publish("/telecos/jaume/test", 0, true, "test 2");
  //Serial.print("[MQTT] Publishing at QoS 0, packetId: ");
  //Serial.println(packetIdPub1);
  //uint16_t packetIdPub2 = mqttClient.publish("/telecos/jaume/test", 0, true, "test 3");
  //Serial.print("[MQTT] Publishing at QoS 0, packetId: ");
  //Serial.println(packetIdPub2);
}

void onMqttDisconnect(AsyncMqttClientDisconnectReason reason) {
  Serial.println("[MQTT] Disconnected from MQTT.");

  if (WiFi.isConnected()) {
    mqttReconnectTimer.once(2, connectToMqtt);
  }
}

void onMqttSubscribe(uint16_t packetId, uint8_t qos) {
  Serial.print("[MQTT] Subscribed to ");
  Serial.println(TOPIC10);
  Serial.print("[MQTT] Subscribed to ");
  Serial.println(TOPIC11);
  //Serial.print(" packetId: ");
  //Serial.println(packetId);
  //Serial.print(" qos: ");
  //Serial.println(qos);
}

void onMqttUnsubscribe(uint16_t packetId) {
  Serial.println("[MQTT] Unsubscribe acknowledged.");
  Serial.print("  packetId: ");
  Serial.println(packetId);
}

void onMqttMessage(char* topic, char* payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total) {
  Serial.print("[CMD ] Command in");
  Serial.print(" topic: ");
  Serial.print(topic);
  Serial.print(" content: ");
  String content = GetPayloadContent(payload, len);
  Serial.print(content);
  Serial.println();
  
  
  //******************Interpreto i executo comandaments************************

  //Q13
  if (content == "Q13set")
  {
    digitalWrite(13, HIGH);
    mqttClient.publish(TOPIC3, 0, true, "Q13on");
    Serial.println("[CMD ] Q13 is ON");
  }
  else if(content == "Q13reset")
  {
    digitalWrite(13, LOW);
    mqttClient.publish(TOPIC3, 0, true, "Q13off");
    Serial.println("[CMD ] Q13 is OFF");
  }
  else if(content == "Q13tog")
  {
    digitalWrite(13, !digitalRead(13));
    if (digitalRead(13) == true)
    {
      Serial.println("[CMD ] Q13 is ON");
      mqttClient.publish(TOPIC3, 0, true, "Q13on");
    }
    if (digitalRead(13) == false)
    {
      Serial.println("[CMD ] Q13 is OFF");
      mqttClient.publish(TOPIC3, 0, true, "Q13off");
    }
  }

  //comandament info?
  else if(content == "info?")
  {
    publishState();
  }

  //comandament reset
  else if(content == "rst")
  {
    ESP.restart();
  }

  //comandament pwm0
  else if (strcmp(topic, TOPIC11)==0)
  {
    valuePWM0 = content.toInt();
    if (valuePWM0 >= 255) valuePWM0 = 255;
    if (valuePWM0 <= 0)   valuePWM0 = 0;
    analogWrite(0, 255-valuePWM0); //regulacio de 255 (0%) a 0 (100%)
    Serial.println (valuePWM0);
  }
 //******************************************************************** 
}

void onMqttPublish(uint16_t packetId) {
  Serial.println("[MQTT] Publish acknowledged.");
  Serial.print("  packetId: ");
  Serial.println(packetId);
}

String GetPayloadContent(char* data, size_t len)
{
  String content = "";
  for(size_t i = 0; i < len; i++)
  {
    content.concat(data[i]);
  }
  return content;
}


void PublishMqtt(char* topic, unsigned long data)
{
  String payload = String(data);
  mqttClient.publish(topic, 0, true, (char*)payload.c_str());
}
