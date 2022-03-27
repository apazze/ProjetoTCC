#include "MonitorAgua.h"

void setup()
{
 Serial.begin(9600);
 Configurar_interrupcao((gpio_num_t) portaVazao); 

}

void loop()
{

Serial.println("Inicia relogio");
}
