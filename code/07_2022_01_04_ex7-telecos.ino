//_2022_01_04_ex7-telecos.ino
//curs tardor 2022 telecos.cat
//jnogues@gmail.com
//pseudo-multitasking i blynk

//------------------ LLIBRERIES --------------------------------------------------------------------
#define BLYNK_PRINT Serial //Activo missatges de blynk per debug
#include <ESP8266WiFi.h> //Llibreria WIFI
#include <BlynkSimpleEsp8266.h> //Llibreria Blynk app
BlynkTimer taskScheduler;//organitzador de tasques
//--------------------------------------------------------------------------------------------------

//------------------ CONFIGURACIÓ ------------------------------------------------------------------
char auth[] = "IXbmwuklV9Fw0vp6_4lGyMTBqDifjs2h"; //token (super-password) obtingut a l'app del mòbil
char ssid[] = "viscalaterraP"; //xarxa wifi
char pass[] = "97A4A6F95697EB30E873"; //password wifi
#define led_blau 16  //led blau de la nodeMCU 
//--------------------------------------------------------------------------------------------------


//------------------ VARIABLES ---------------------------------------------------------------------
//Més info a : https://programarfacil.com/blog/arduino-blog/tipos-de-datos-en-arduino/
unsigned int valueA0=0;//valor de 0 a 65535,16 bits
unsigned int limitA0=512;
//--------------------------------------------------------------------------------------------------

/*------------------recordatori de pins virtuals utilitzats-----------------------------------------
V0    upTime
V1    valueA0
V10   led virtual alarma A0

--------------------------------------------------------------------------------------------------*/



void setup() 
{
  Serial.begin(115200);
  delay(500);
  Serial.println(" ");
  Serial.println("telecos.cat");
  Serial.println("_2022_01_04_ex7-telecos.ino");
  
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

  taskScheduler.setInterval(500L, toggleLed); //tasca 1
  taskScheduler.setInterval(5000L, upTime);   //tasca 2
  taskScheduler.setInterval(5001L, analogA0);   //tasca 3

  // Servidor de Blynk
  // IP del servidor privat de blynk del @rprimTech, situat a GRAVELINES (1.393 km), 
  // França (https://ca.wikipedia.org/wiki/Gravelines)
  // https://goo.gl/maps/JjtPBqaPQVjfuFYMA
  Blynk.begin(auth, ssid, pass, IPAddress(51,38,35,12), 8080); 
  
}

void loop() 
{
   taskScheduler.run();
   Blynk.run();         //Aquí fa la màgia Blynk
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
