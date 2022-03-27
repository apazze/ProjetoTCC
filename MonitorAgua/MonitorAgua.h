// BIBLIOTECAS E VARIAVEIS GLOBAIS
//#include "tcc.h"
//RTC
#include <Wire.h>
#include "RTClib.h"
//SD Card
#include "FS.h"
#include "SD.h"
#include "SPI.h"
//Temperatura
#include <Adafruit_Sensor.h>
#include <DHT.h>
#define DHTPIN 4
#define DHTTYPE DHT11
DHT dht = DHT(DHTPIN, DHTTYPE);
//Leds
#define ledG 14
#define ledR 15
#define ledY 26
//Vazão
#define portaVazao 13

volatile int pulsos_vazao = 0;
unsigned long ultimo_valor_salvo = 0;
float vazao = 0;
float vazao_somando = 0;
float MiliLitros = 0;
 
void IRAM_ATTR Interrupcao(void* arg) { // funçao chamada cada vez que o sensor de fluxo mandar um pulso
  pulsos_vazao++;                  //soma a variavel de contagem de pulsos do sensor de fluxo de agua
  portYIELD_FROM_ISR();
}
 
void Configurar_interrupcao(gpio_num_t Port) {
  pinMode(portaVazao, INPUT_PULLUP);            //configura pino como entrada
  gpio_set_intr_type(Port, GPIO_INTR_NEGEDGE);  //tipo de interrupçao
  gpio_intr_enable(Port);                       //ativa a porta
  gpio_install_isr_service(0);                  //instala a interrupçao
  gpio_isr_handler_add(Port, Interrupcao, (void*) Port); // oque fazer ao detectar a interupçao
  Serial.print("");
}

RTC_DS1307 rtc;
char diasDaSemana[7][12] = {"Domingo", "Segunda", "Terca", "Quarta", "Quinta", "Sexta", "Sabado"};

String nomeDoArquivo = "registra_temperatura";
