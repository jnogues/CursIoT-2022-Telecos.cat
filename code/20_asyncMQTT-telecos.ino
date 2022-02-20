//20_asyncMQTT-telecos.ino
//curs hivern 2022 telecos.cat
//jnogues@gmail.com
//asyncmqtt



#include <ESP8266WiFi.h>
#include <Ticker.h>
#include <AsyncMqttClient.h>
#include <Blynk.h>
BlynkTimer taskScheduler;//organitzador de tasques

#define led_blau 16
#define WIFI_SSID     "xxxxxxxxxxxx"
#define WIFI_PASSWORD "xxxxxxxxxxxx"

#define MQTT_HOST IPAddress(5, 196, 88, 155)
#define MQTT_PORT 1883

#define TOPIC1  "/telecos/jaume/uT"         //Per publicar dades
#define TOPIC2  "/telecos/jaume/boot"       //Per publicar dades
#define TOPIC3  "/telecos/jaume/outputs"    //Per publicar dades
#define TOPIC4  "/telecos/jaume/a0"         //Per publicar dades
#define TOPIC5  "/telecos/jaume/inputs"     //Per publicar dades
#define TOPIC10 "/telecos/jaume/commands"   //Per rebre comandaments

AsyncMqttClient mqttClient;
Ticker mqttReconnectTimer;

WiFiEventHandler wifiConnectHandler;
WiFiEventHandler wifiDisconnectHandler;
Ticker wifiReconnectTimer;

boolean stateGPIO16=0;
boolean stateGPIO0=0;
boolean prevstateGPIO0=0;
//boolean stateGPIO2=0;     //GPIO2 is used for wifi stat led
//boolean prevstateGPIO2=0;
boolean stateGPIO13=0;
boolean prevstateGPIO13=0;
boolean stateGPIO12=0;
boolean prevstateGPIO12=0;
boolean stateGPIO14=0;
boolean prevstateGPIO14=0;

void setup() {
  Serial.begin(115200);
  delay(500);
  Serial.println(" ");
  Serial.println(" ");
  Serial.println("[boot] telecos.cat");
  Serial.println("[boot] 20_asyncMQTT-telecos.ino");
  
  pinMode(0, OUTPUT);
  digitalWrite(0,HIGH);
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
  

  taskScheduler.setInterval(500L,   toggleLed);            //tasca 1
  taskScheduler.setInterval(10000L, publishUpTime);        //tasca 2
  taskScheduler.setInterval(5000L,  publishA0);            //tasca 3
  taskScheduler.setInterval(300L,   checkInputsOutputs);   //tasca 4
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



//********************** Callbacks i Funcions ************************
void connectToWifi() {
  Serial.print("[WIFI] Connecting to Wi-Fi...");
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
  uint16_t packetIdSub = mqttClient.subscribe(TOPIC10, 0);
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
  //Serial.print("[MQTT]   qos: ");
  //Serial.println(properties.qos);
  //Serial.print("  dup: ");
  //Serial.println(properties.dup);
  //Serial.print("[MQTT]   retain: ");
  //Serial.println(properties.retain);
  //Serial.print("[MQTT]   len: ");
  //Serial.println(len);
  //Serial.print("[MQTT]   index: ");
  //Serial.println(index);
  //Serial.print("[MQTT]   total: ");
  //Serial.println(total);
  
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
