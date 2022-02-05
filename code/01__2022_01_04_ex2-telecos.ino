//_2022_01_04_ex2-telecos.ino
//curs tardor 2022 telecos.cat
//jnogues@gmail.com
//pseudo-multitasking

//llibreria blynk
#include <Blynk.h>
BlynkTimer taskScheduler;//organitzador de tasques

#define led_blau 16

void setup() 
{
  Serial.begin(115200);
  delay(500);
  Serial.println(" ");
  Serial.println("telecos.cat");
  Serial.println("_2022_01_04_ex2-telecos.ino");
  
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

  taskScheduler.setInterval(500L, toggleLed);//tasca 1
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
