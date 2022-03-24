#include <Wire.h>
#include "RTClib.h"

RTC_DS1307 rtc;
//Vazão
#define portaVazao 13

volatile int pulsos_vazao = 0;
unsigned long ultimo_valor_salvo = 0;
float vazao = 0;
float vazao_somando = 0;
float MiliLitros = 0;
char diasDaSemana[7][12] = {"Domingo", "Segunda", "Terca", "Quarta", "Quinta", "Sexta", "Sabado"};

void setup() {
 Serial.begin(115200);
 InicializaRTC();
 Configurar_interrupcao((gpio_num_t) portaVazao); 

}

void loop()
{
  // caso ja tenha passado 1 segundo do ultimo dado aferido e salvo
  if ((millis() - ultimo_valor_salvo) > 1000)
  { 
    ultimo_valor_salvo = millis();
    //guarda as informaçoes de data e tempo no objeto now
    DateTime now = rtc.now();
 
    //converte a quantia de pulsos que o sensor mandou para a vazao da agua em litros por minuto
    vazao = pulsos_vazao / 5.5;
    pulsos_vazao = 0;
    MiliLitros = vazao / 60;

    // calcula a vazao total do dia
    vazao_somando = vazao_somando + MiliLitros;
    
    // caso ja seja 23:59:59 vamos salvar os dados no cartao sd
    
    if(now.hour() == 23 && now.minute() == 59 && now.second() == 59)
    {
       Serial.println(" ---------------------------------- ");
       Serial.println(vazao_somando);
       Serial.println(" ---------------------------------- ");

       //cria a string que sera salva no cartao sd
       String dataMessage = String(now.day()) + "/" + String(now.month()) + "/" + String(now.year()) + " ---  " + String(vazao_somando) + " L/dia <br> \n ";
       Serial.println(dataMessage);
       
       //EditarArquivo(SD, "/Fluxo_de_agua_dia.txt", dataMessage.c_str());
       
       //reinicia a contagem de vazao de agua diarios
       vazao_somando = 0;
    }
 
    // mostra o valor da leitura do sensor
    if(MiliLitros > 0)
    {
      Serial.print(" Sensor de Vazao esta registrando "); Serial.print(MiliLitros); Serial.println(" litros/Segundo");
    }

  }
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

// funçao chamada cada vez que o sensor de fluxo mandar um pulso
void IRAM_ATTR Interrupcao(void* arg)
{ 
  //soma a variavel de contagem de pulsos do sensor de fluxo de agua
  pulsos_vazao++;
  
  portYIELD_FROM_ISR();
}
void Configurar_interrupcao(gpio_num_t Port) 
{
  //configura pino como entrada
  pinMode(portaVazao, INPUT_PULLUP);
  
  //tipo de interrupçao
  gpio_set_intr_type(Port, GPIO_INTR_NEGEDGE);

  //ativa a porta
  gpio_intr_enable(Port);    
                    
  //instala a interrupçao
  gpio_install_isr_service(0);
  
  //o que fazer ao detectar a interupçao
  gpio_isr_handler_add(Port, Interrupcao, (void*) Port);
}
