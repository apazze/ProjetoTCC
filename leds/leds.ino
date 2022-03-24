//Leds
#define ledG 14
#define ledR 15
#define ledY 26
void setup()
{
  pinMode(ledG, OUTPUT);
  pinMode(ledR, OUTPUT);
  pinMode(ledY, OUTPUT);
}
void loop()
{
  AcionaLeds();
}
void AcionaLeds()
{
  
  digitalWrite(ledG, HIGH);
  delay(1000);
  digitalWrite(ledG, LOW);
  delay(1000);
  digitalWrite(ledR, HIGH);
  delay(1000);
  digitalWrite(ledR, LOW);
  delay(1000);
  digitalWrite(ledY, HIGH);
  delay(1000);
  digitalWrite(ledY, LOW);
  delay(1000);
  
}
