//_2022_01_04_ex1-telecos.ino
//curs tardor 2022 telecos.cat
//jnogues@gmail.com

int counter=0;

void setup() 
{
  Serial.begin(115200);
  delay(500);
  Serial.println(" ");
  Serial.println("telecos.cat");
  
  pinMode(0, OUTPUT);
  pinMode(2, OUTPUT);
  pinMode(13, OUTPUT);
  pinMode(15, OUTPUT);
  pinMode(4, INPUT);
  pinMode(5, INPUT);
  pinMode(12, INPUT);
  pinMode(14, INPUT);
  pinMode(16, OUTPUT);
}

void loop() 
{
  digitalWrite(0, LOW);
  digitalWrite(2, LOW);
  digitalWrite(13, HIGH);
  digitalWrite(15, HIGH);
  digitalWrite(16, HIGH);

  delay(100);

  digitalWrite(0, HIGH);
  digitalWrite(2, HIGH);
  digitalWrite(13, LOW);
  digitalWrite(15, LOW);
  digitalWrite(16, LOW);

  delay(100);

  counter++;
  Serial.print("[ ");
  Serial.print(counter);
  Serial.print(" ] ");
  Serial.println(analogRead(0));
  //if (!digitalRead(4)||!digitalRead(5)||digitalRead(12)||digitalRead(14)) Serial.println ("@"); 
}
