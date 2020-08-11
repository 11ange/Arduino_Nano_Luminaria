
// Saídas PWM para os LEDs RGB
const int pinoLedVm = 9;    //Led Vermelho -> D9
const int pinoLedVd = 10;   //Led Verde -> D10
const int pinoLedAz = 11;   //Led Azul -> D11

// Se o LED for CATODO comum, comentar a linha abaixo.
#define ANODO_COMUM
// Para debug na porta serial deixar descomentada a linha abaixo
#define DEBUG

int valorLedVm = 0;
int valorLedVd = 0;
int valorLedAz = 0;
int i;
int corInicial = 0;
int corFinal = 0;
int novaCor = 0;
int novoTempo = 1000;
int valoresCorInicial[3] = {0, 0, 0};
int valoresCorFinal[3] = {0, 0, 0};

#ifdef DEBUG
//#define DEBUG_STEPS
String nomeCorAntiga = "Desligado";
String nomeCorNova;
int contador = 0;
int passoContador = 0;
#endif

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// SETUP
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void setup()  {

  Serial.begin(9600);   // Inicializa a porta serial.

  // Seta as saídas dos LEDs como OUTPUT:
  pinMode(pinoLedVm, OUTPUT);
  pinMode(pinoLedVd, OUTPUT);
  pinMode(pinoLedAz, OUTPUT);

  // Inicializa o gerador de números aleatórios.
  // Um pino analógico desconectado irá retornar um
  // valor aleatório de tensão em analogRead().
  // Dessa forma a cor inicial ao ligar será aleatória.
  randomSeed(analogRead(0));

  setaCor(255, 255, 255);
  delay(500);
  setaCor(255, 255, 0);
  delay(500);
  setaCor(255, 0 , 255);
  delay(500);
  setaCor(255, 0, 0);
  delay(500);
  setaCor(0, 255, 255);
  delay(500);
  setaCor(0, 255, 0);
  delay(500);
  setaCor(0, 0, 255);
  //delay(500);
  //setaCor(0, 0, 0);
  delay(2000);
  valoresCorInicial[2] = 255;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// LOOP
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void loop() {
  // Determina a próxima cor aleatóriamente entre 1 e 7
  novaCor = random(1, 7);

  // Determina um tempo de transição aleatório de 1.000ms a 20.000ms
  novoTempo = random(1000, 20000);
#ifdef DEBUG
  passoContador = novoTempo / 10;   // Só quero 10 passos na tela de debug da saída
#endif

  // Chama a rotina de transição de cor
  transicao(novaCor, novoTempo);

}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// SETACOR
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Função para comandar a cor do LED RGB
void setaCor(float valorLedVm, float valorLedVd, float valorLedAz) {

#ifdef DEBUG_STEPS
#undef ANODO_COMUM
#endif

  // Caso o LED seja anodo comum (positivo comum), a saída deve ser o inverso da entrada da função.
  // P. ex., se enviarmos 255 pra saída, o PWM será 100% de 5V, ou seja, 5V.
  // Como o LED é anodo comum precisamos enviar 0V ao invés de 5V.
#ifdef ANODO_COMUM
  valorLedVm = 255 - valorLedVm;
  valorLedVd = 255 - valorLedVd;
  valorLedAz = 255 - valorLedAz;
#endif

  analogWrite(pinoLedVm, valorLedVm);
  analogWrite(pinoLedVd, valorLedVd);
  analogWrite(pinoLedAz, valorLedAz);

#ifdef DEBUG_STEPS
  contador++;
  if (contador == passoContador) {
    Serial.print("Saída: ");
    Serial.print(valorLedVm);
    Serial.print(" / ");
    Serial.print(valorLedVd);
    Serial.print(" / ");
    Serial.println(valorLedAz);
    contador = 0;
  }
#endif
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// TRANSICAO
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void transicao(int cor, int tempo) {
  float stepsCor[3] = {0, 0, 0};
  corFinal = cor;
  buscaCor(corFinal);

#ifdef DEBUG
  Serial.println();
  Serial.println("-------------------------------------------");
  Serial.print("Cor Inicial: ");
  Serial.print(corInicial, DEC);
  Serial.print(" (");
  Serial.print(nomeCorAntiga);
  Serial.print(")");
  for (i = 0; i < 3; i++)
  {
    Serial.print(" - ");
    Serial.print(*(valoresCorInicial + i));
  }
  Serial.println();
  Serial.print("Cor Final: ");
  Serial.print(corFinal, DEC);
  Serial.print(" (");
  Serial.print(nomeCorNova);
  Serial.print(")");
  for (i = 0; i < 3; i++)
  {
    Serial.print(" - ");
    Serial.print(*(valoresCorFinal + i));
  }
  Serial.println();
  Serial.print("Tempo Transição: ");
  Serial.println(tempo, DEC);
  Serial.println();
#endif

  // Calcula steps de cor de acordo com o tempo desejado de transição (um para cada saída RGB)
  for (i = 0; i < 3; i++) {
    stepsCor[i] = (valoresCorFinal[i] - valoresCorInicial[i]) / (float)tempo;
  }

  // Aciona a saida usando os steps calculados anteriormente e vai adicionando a cada 1ms para
  // dar o efeito de mudança de cor
  for (i = 0; i < tempo; i++) {
    setaCor(valoresCorInicial[0] + stepsCor[0]*i, valoresCorInicial[1] + stepsCor[1]*i, valoresCorInicial[2] + stepsCor[2]*i);
    delay(1);
  }

  // Salva os valores atuais para os iniciais para usar como entrada do próximo loop
  copiaArray(valoresCorFinal, valoresCorInicial, 3);
  corInicial = corFinal;
#ifdef DEBUG
  nomeCorAntiga = nomeCorNova;
#endif
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// BUSCACOR
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void buscaCor(int cor) {
  switch (cor) {
    case 1:   // Vermelho
#ifdef DEBUG
      nomeCorNova = "Vermelho";
#endif
      valoresCorFinal[0] = 255;
      valoresCorFinal[1] = 0;
      valoresCorFinal[2] = 0;
      break;
    case 2:   // Verde
#ifdef DEBUG
      nomeCorNova = "Verde";
#endif
      valoresCorFinal[0] = 0;
      valoresCorFinal[1] = 255;
      valoresCorFinal[2] = 0;
      break;
    case 3:   // Azul
#ifdef DEBUG
      nomeCorNova = "Azul";
#endif
      valoresCorFinal[0] = 0;
      valoresCorFinal[1] = 0;
      valoresCorFinal[2] = 255;
      break;
    case 4:   // Amarelo
#ifdef DEBUG
      nomeCorNova = "Amarelo";
#endif
      valoresCorFinal[0] = 255;
      valoresCorFinal[1] = 255;
      valoresCorFinal[2] = 0;
      break;
    case 5:   // Cyan
#ifdef DEBUG
      nomeCorNova = "Cyan";
#endif
      valoresCorFinal[0] = 0;
      valoresCorFinal[1] = 255;
      valoresCorFinal[2] = 255;
      break;
    case 6:   // Violeta
#ifdef DEBUG
      nomeCorNova = "Violeta";
#endif
      // Verificar se 80,0,80 ou 150,0,255 ficam melhores que 255,0,255
      valoresCorFinal[0] = 255;
      valoresCorFinal[1] = 0;
      valoresCorFinal[2] = 255;
      break;
    case 7:   // Branco
#ifdef DEBUG
      nomeCorNova = "Branco";
#endif
      valoresCorFinal[0] = 255;
      valoresCorFinal[1] = 255;
      valoresCorFinal[2] = 255;
      break;
    default:   // Desligado
#ifdef DEBUG
      nomeCorNova = "Desligado";
#endif
      valoresCorFinal[0] = 0;
      valoresCorFinal[1] = 0;
      valoresCorFinal[2] = 0;
      break;
  }
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// COPIAARRAY
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/*void copiaArray(int* src, int* dst, int len) {
  for (i = 0; i < len; i++) {
    dst++ = *src++;
  }
  }

  void copyA(int* src, int* dst, int len) {
  memcpy(dst, src, sizeof(src[0])*len);
  }*/
void copiaArray(int* src, int* dst, int len) {
  for (i = 0; i < len; i++) {
    dst[i] = src[i];
  }
}
