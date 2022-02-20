//_2022_01_04_ex14-telecos.ino
//curs tardor 2022 telecos.cat
//jnogues@gmail.com
//pseudo-multitasking

//------------------ LLIBRERIES --------------------------------------------------------------------
#define BLYNK_PRINT Serial //Activo missatges de blynk per debug
#include <ESP8266WiFi.h> //Llibreria WIFI
#include <BlynkSimpleEsp8266.h> //Llibreria Blynk app
BlynkTimer taskScheduler;//organitzador de tasques
//--------------------------------------------------------------------------------------------------

//------------------ CONFIGURACIÓ ------------------------------------------------------------------
char auth[] = "xxxxxxxxxxxxxxxxxxxxxxx"; //token (super-password) obtingut a l'app del mòbil
char ssid[] = "xxxxxxxxxxxx"; //xarxa wifi
char pass[] = "xxxxxxxxxxxx"; //password wifi
#define led_blau 16  //led blau de la nodeMCU 
//--------------------------------------------------------------------------------------------------


//------------------ VARIABLES ---------------------------------------------------------------------
//Més info a : https://programarfacil.com/blog/arduino-blog/tipos-de-datos-en-arduino/
unsigned int valueA0=0;//valor de 0 a 65535,16 bits
//--------------------------------------------------------------------------------------------------

void setup() 
{
  Serial.begin(115200);
  delay(500);
  Serial.println(" ");
  Serial.println("telecos.cat");
  Serial.println("_2022_01_04_ex14-telecos.ino");
  
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

  // Servidor de Blynk
  // IP del servidor privat de blynk del @rprimTech
  Blynk.begin(auth, ssid, pass, IPAddress(5,196,88,155), 8080); 
  
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


// accions disparades per l'app
BLYNK_WRITE (V30)
{
  int kk=param.asInt();
  if (kk==1) digitalWrite(0, !digitalRead(0));
}

BLYNK_WRITE (V31)
{
  int kk=param.asInt();
  analogWrite(13,kk);
}

BLYNK_WRITE (V32)
{
  int kk=param.asInt();
  analogWrite(2,255-kk);
}

BLYNK_WRITE (V33)
{
  int kk=param.asInt();
  analogWrite(13,kk);
}
