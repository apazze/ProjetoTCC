//Pinos SDA na GPIO 21 e SCL na GPIO 22 (ESP32)

//RTC
#include <Wire.h>
#include "RTClib.h"
RTC_DS1307 rtc;
char diasDaSemana[7][12] = {"Domingo", "Segunda", "Terca", "Quarta", "Quinta", "Sexta", "Sabado"};

void setup()
{
  Serial.begin(115200);
  InicializaRTC();
}
void loop()
{
}
void InicializaRTC()
{
  if (! rtc.begin())
  {
    Serial.println("RTC NAO INICIALIZADO");
    while(1);
  }
    Serial.print("\n\nRTC INICIALIZADO COM SUCESSO ");
    ImprimeData();
  
  //Necessita ajustar o relogio manualmente na primeira utilização:
  //rtc.adjust(DateTime(2022, 3, 23, 20, 07, 00));
  
  delay(100);
}
void ImprimeData()
{
  DateTime now = rtc.now();
  String data = String(now.day()) + "/" + String(now.month()) + "/" + String(now.year());
  String hora = String(now.hour()) + ":" + String(now.minute()) + ":" + String(now.second());
  String dataEHora = "[ " + String(diasDaSemana[now.dayOfTheWeek()]) + ", " + data + " " + hora + " ]";
  Serial.print(dataEHora);
}
