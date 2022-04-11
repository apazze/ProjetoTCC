#define RELE 27
unsigned long millisAcionaRele = millis();
unsigned long lastMillis = 0;
bool notifica = true; 
int cont = 0;
void setup() {
  // put your setup code here, to run once:
  pinMode(RELE, OUTPUT);
  
  Serial.begin(115200);
  
}

void loop() {
  // put your main code here, to run repeatedly:

  if(notifica)
  {
    // Verifica se já passou 500 milisegundos
    if(millis() - millisAcionaRele < 500)
    {
      digitalWrite(RELE, HIGH);
      if(millis() - lastMillis > 500)
      {
        lastMillis = millis();
        cont++;
      }
    }
      
    else
      digitalWrite(RELE, LOW);

    // Verifica se já passou 1000 milisegundos
    if((millis() - millisAcionaRele) > 1000)
      millisAcionaRele = millis();

    if(cont == 3)
    {
      notifica = false;
      digitalWrite(RELE, LOW);
    }
  }

  
  
}
