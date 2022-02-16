//_2022_01_04_ex13-OTA-telecos.ino
//curs tardor 2022 telecos.cat
//jnogues@gmail.com
//pseudo-multitasking i blynk
//OverTheAir flashing

//------------------ LLIBRERIES --------------------------------------------------------------------
#define BLYNK_PRINT Serial //Activo missatges de blynk per debug
#include <ESP8266WiFi.h> //Llibreria WIFI
#include <BlynkSimpleEsp8266.h> //Llibreria Blynk app
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
BlynkTimer taskScheduler;//organitzador de tasques
//--------------------------------------------------------------------------------------------------

//------------------ CONFIGURACIÓ ------------------------------------------------------------------
char auth[] = "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"; //token (super-password) obtingut a l'app del mòbil
char ssid[] = "xxxxxxxxxxxxxx"; //xarxa wifi
char pass[] = "xxxxxxxxxxxxxx"; //password wifi
#define led_blau 16  //led blau de la nodeMCU 
#define STAHOSTNAME "elMeu1rOTA"
//--------------------------------------------------------------------------------------------------


//------------------ VARIABLES ---------------------------------------------------------------------
//Més info a : https://programarfacil.com/blog/arduino-blog/tipos-de-datos-en-arduino/
unsigned int valueA0=0;//valor de 0 a 65535,16 bits
unsigned int limitA0=512;
//--------------------------------------------------------------------------------------------------

/*------------------recordatori de pins virtuals utilitzats-----------------------------------------
V0    upTime
V1    valueA0
V3    led virtual estat SORTIDA gpio0, cal afegir un interruptor virtual assignat a gpio0
V4    led virtual estat SORTIDA gpio2, cal afegir un interruptor virtual assignat a gpio2
V5    led virtual estat SORTIDA gpio13, el pulsador físic gpio12 commuta el seu estat
V6    Widget de text que indica finestra OBERTA/TANCADA, entrada gpio14
V10   led virtual alarma A0

--------------------------------------------------------------------------------------------------*/

boolean stateGPIO16=0;
boolean stateGPIO0=0;
boolean prevstateGPIO0=0;
boolean stateGPIO2=0;
boolean prevstateGPIO2=0;
boolean stateGPIO13=0;
boolean prevstateGPIO13=0;
boolean stateGPIO12=0;
boolean prevstateGPIO12=0;
boolean stateGPIO14=0;
boolean prevstateGPIO14=0;

void setup() 
{
  Serial.begin(115200);
  delay(500);
  Serial.println(" ");
  Serial.println("telecos.cat");
  Serial.println("_2022_01_04_ex13-OTA-telecos.ino");
  
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

  taskScheduler.setInterval(500L,  toggleLed);            //tasca 1
  taskScheduler.setInterval(5000L, upTime);               //tasca 2
  taskScheduler.setInterval(5001L, analogA0);             //tasca 3
  taskScheduler.setInterval(300L,  checkInputsOutputs);   //tasca 4

  // Servidor de Blynk
  // IP del servidor privat de blynk del @rprimTech
  Blynk.begin(auth, ssid, pass, IPAddress(5,196,88,155), 8080); 
  ArduinoOTA.setHostname(STAHOSTNAME);
  ArduinoOTA.begin();
  
}

void loop() 
{
   if (Blynk.connected())
    {
      Blynk.run(); //Aquí fa la màgia Blynk
    }

   taskScheduler.run();
   ArduinoOTA.handle();
   
          
}


//***********************TASQUES***********************

//tasca 1
void toggleLed()
{
  digitalWrite(led_blau, !digitalRead(led_blau));
}

//tasca 2
void upTime()
{
  //Serial.print("[upT] ");
  //Serial.println(millis());
  Serial.printf("[upT] %u \n", millis()/1000);//fa el mateix
  Blynk.virtualWrite(V0, millis()/1000);
}

//tasca 3
void analogA0()
{
  valueA0=analogRead(A0);
  Serial.printf("[ANG] %d \n", valueA0);
  Blynk.virtualWrite(V1, valueA0);
  if (valueA0 >= limitA0)
  {
    Blynk.virtualWrite(V10, 255);
  }
  else
  {
    Blynk.virtualWrite(V10, 0);
  }
}

//tasca 4
void checkInputsOutputs()
{  
  //GPIO0
  stateGPIO0=digitalRead(0);
  if (stateGPIO0!=prevstateGPIO0)
  {
    if (stateGPIO0==0) Blynk.virtualWrite(V3,255);
    if (stateGPIO0==1) Blynk.virtualWrite(V3,0);
  }
  prevstateGPIO0=stateGPIO0;
  
  //GPIO2
  stateGPIO2=digitalRead(2);
  if (stateGPIO2!=prevstateGPIO2)
  {
    if (stateGPIO2==0) Blynk.virtualWrite(V4,255);
    if (stateGPIO2==1) Blynk.virtualWrite(V4,0);
  }
  prevstateGPIO2=stateGPIO2;

  //Polsador GPIO12 a sortida GPIO13, pulsador físic que commuta GPIO13 i visualització a l'app
  stateGPIO12=digitalRead(12);
  if (prevstateGPIO12==0 && stateGPIO12==1)
  {
    bool temp=digitalRead(13);
    if (temp){ digitalWrite(13,LOW); Blynk.virtualWrite(V5,0);}
    if (!temp){ digitalWrite(13,HIGH); Blynk.virtualWrite(V5,255);}
    prevstateGPIO12=1;
  }
  if (prevstateGPIO12==1 && stateGPIO12==0)
  {
    prevstateGPIO12=0;
  }
 
  stateGPIO13=digitalRead(13);
  if (stateGPIO13!=prevstateGPIO13)
  {
    if (stateGPIO13==0) Blynk.virtualWrite(V5,0);
    if (stateGPIO13==1) Blynk.virtualWrite(V5,255);
  }
  prevstateGPIO13=stateGPIO13;

  //***************************Sensor finestra oberta/tancada***************************** 
  stateGPIO14=digitalRead(14);
  if (stateGPIO14!=prevstateGPIO14)
  {
    if (stateGPIO14==0)  Blynk.virtualWrite(V6,"TANCADA");
    if (stateGPIO14==1) 
      {
        Blynk.notify("[NOTF] Finestra OBERTA!!");//Max. 1 notificació cada 5 segons 
        Blynk.virtualWrite(V6,"OBERTA"); 
       }
   }
  prevstateGPIO14=stateGPIO14;
 //*************************************************************************************
  
}
