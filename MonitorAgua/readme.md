# Firmware ESP32
- Rotinas de inicialização da data e hora, wifi e mqtt, sensor de vazão e relé
- Rotina principal que lê o sensor de vazão. Obs.: Conectado no pino de interrupção da ESP32 que dá total prioridade quando é acionado.
- Rotina de serialização para Json para enviar o valor da vazão (publish)
- Rotina de deserialização das mensagens recebidas (subscribe) para a rotina de advertência
- Rotina de advertência, aciona brevemente o relé para acionar um alto falante de 12V
