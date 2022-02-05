//_2022_01_04_ex4-telecos.ino
//curs tardor 2022 telecos.cat
//jnogues@gmail.com
//pseudo-multitasking

//llibreria blynk
#include <Blynk.h>
BlynkTimer taskScheduler;//organitzador de tasques

#define led_blau 16

//variables
unsigned int valueA0=0;//valor de 0 a 65535,16 bits

void setup() 
{
  Serial.begin(115200);
  delay(500);
  Serial.println(" ");
  Serial.println("telecos.cat");
  Serial.println("_2022_01_04_ex4-telecos.ino");
  
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
}

void loop() 
{
   taskScheduler.run();
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
  Serial.printf("[upT] %u \n", millis());//fa el mateix
}

//tasca 3
void analogA0()
{
  valueA0=analogRead(A0);
  Serial.printf("[ANG] %d \n", valueA0);
}
