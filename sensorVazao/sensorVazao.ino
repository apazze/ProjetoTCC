#define portaVazao 13
 
volatile int pulsos_vazao = 0;
unsigned long ultimo_valor_salvo = 0;
float vazao = 0;
float MiliLitros = 0;
float vazao_somando;

void IRAM_ATTR Interrupcao(void* arg) { // funçao chamada cada vez que o sensor de fluxo mandar um pulso
  pulsos_vazao++;                   //soma a variavel de contagem de pulsos do sensor de fluxo de agua
  portYIELD_FROM_ISR();
}
 
void Configurar_interrupcao(gpio_num_t Port) {
  pinMode(portaVazao, INPUT_PULLUP);            //configura pino como entrada
  gpio_set_intr_type(Port, GPIO_INTR_NEGEDGE);  //tipo de interrupçao
  gpio_intr_enable(Port);                       //ativa a porta
  gpio_install_isr_service(0);                  //instala a interrupçao
  gpio_isr_handler_add(Port, Interrupcao, (void*) Port); // oque fazer ao detectar a interupçao
}

void setup() {
  Serial.begin(115200);
 
  Configurar_interrupcao((gpio_num_t) portaVazao); //chama a funçao que ira configurar a interrupçao
}

void loop()
{
 salvar_dados();
}
 
void salvar_dados(){
  
  if ((millis() - ultimo_valor_salvo) > 1000) { // caso ja tenha passado 1 segundo do ultimo dado aferido e salvo
    ultimo_valor_salvo = millis();
  
    //converte a quantia de pulsos que o sensor mandou para a vazao da agua em litros por minuto
    vazao = pulsos_vazao / 5.5;
    pulsos_vazao = 0;
    MiliLitros = vazao / 60;
    vazao_somando = vazao_somando + MiliLitros; // calcula a vazao total do dia

 
    // mostra o valor da leitura do sensor
    if(vazao_somando > 0){
      Serial.print("Sensor de Vazao registra: [ "); 
      Serial.print(vazao_somando); 
      Serial.print(" ] litros\n");
    }
    
 
  }
}
 
