#include <Wire.h>
#include "RTClib.h"
RTC_DS1307 rtc;
char diasDaSemana[7][12] = {"Domingo", "Segunda", "Terca", "Quarta", "Quinta", "Sexta", "Sabado"};

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
  //rtc.adjust(DateTime(2022, 3, 20, 11, 26, 00));
  
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
