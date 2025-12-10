#include <Adafruit_NeoPixel.h>
#include <Adafruit_GFX.h>
#include <MCUFRIEND_kbv.h> 
#include <SD.h>
#include <SPI.h>  
#include <GFButton.h>
#include <avr/pgmspace.h>
#include <string.h>


#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF


// 'claveDeSolInvertida', 50x80px
const unsigned char claveSol [] PROGMEM = {
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x78, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xfc, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xfe, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x07, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xc7, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x07, 0x83, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x83, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 
	0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0e, 0x03, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x0e, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0e, 0x07, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x0e, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0c, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x0c, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x1e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x3e, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x7e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0xfe, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x07, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xfc, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x07, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xf8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 
	0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xc0, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 0x01, 0xff, 0x80, 0x00, 0x00, 0x00, 
	0x00, 0x03, 0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 0x07, 0xfd, 0x80, 0x00, 0x00, 0x00, 0x00, 0x07, 
	0xf0, 0x80, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xe0, 0x80, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xc0, 0x80, 
	0x00, 0x00, 0x00, 0x00, 0x1f, 0x80, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x3f, 0x00, 0x40, 0x00, 0x00, 
	0x00, 0x00, 0x3e, 0x00, 0xff, 0x80, 0x00, 0x00, 0x00, 0x3e, 0x03, 0xff, 0xc0, 0x00, 0x00, 0x00, 
	0x7c, 0x07, 0xff, 0xe0, 0x00, 0x00, 0x00, 0x78, 0x0f, 0xff, 0xf0, 0x00, 0x00, 0x00, 0x78, 0x1f, 
	0xff, 0xf8, 0x00, 0x00, 0x00, 0x78, 0x1f, 0x21, 0xf8, 0x00, 0x00, 0x00, 0x78, 0x3e, 0x30, 0xfc, 
	0x00, 0x00, 0x00, 0x70, 0x3c, 0x30, 0x7c, 0x00, 0x00, 0x00, 0x70, 0x3c, 0x30, 0x3c, 0x00, 0x00, 
	0x00, 0x70, 0x38, 0x10, 0x3c, 0x00, 0x00, 0x00, 0x38, 0x38, 0x18, 0x3c, 0x00, 0x00, 0x00, 0x38, 
	0x38, 0x18, 0x3c, 0x00, 0x00, 0x00, 0x38, 0x1c, 0x18, 0x3c, 0x00, 0x00, 0x00, 0x1c, 0x0c, 0x08, 
	0x3c, 0x00, 0x00, 0x00, 0x1c, 0x06, 0x0c, 0x3c, 0x00, 0x00, 0x00, 0x0e, 0x03, 0x0c, 0x38, 0x00, 
	0x00, 0x00, 0x07, 0x00, 0x8c, 0x70, 0x00, 0x00, 0x00, 0x03, 0x80, 0x0c, 0xf0, 0x00, 0x00, 0x00, 
	0x01, 0xe0, 0x05, 0xc0, 0x00, 0x00, 0x00, 0x00, 0xf8, 0x07, 0x80, 0x00, 0x00, 0x00, 0x00, 0x1f, 
	0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xe6, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3c, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfe, 0x03, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x01, 0x80, 0x00, 
	0x00, 0x00, 0x01, 0xff, 0x01, 0x00, 0x00, 0x00, 0x00, 0x01, 0xff, 0x03, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0xfe, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf8, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x78, 
	0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x38, 0x1c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xf8, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x01, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

// Array of all bitmaps for convenience. (Total bytes used to store images in PROGMEM = 576)
const int epd_bitmap_allArray_LEN = 1;
const unsigned char* epd_bitmap_allArray[1] = {
	claveSol
};


#define NUM_LEDS  12
#define PIN  48
#define SD_CS 10
#define BUFFPIXEL      20
#define PALETTEDEPTH   0 
#define BMPIMAGEOFFSET 54

#define TFT_GRAY    0x8410 

#define num_teclas 12

GFButton botao[12] = {
  GFButton(30), GFButton(31), GFButton(32), GFButton(33),
  GFButton(34), GFButton(35), GFButton(36), GFButton(37),
  GFButton(38), GFButton(39), GFButton(40), GFButton(41)
};


Adafruit_NeoPixel strip(NUM_LEDS,PIN,NEO_GRB + NEO_KHZ800);
MCUFRIEND_kbv tela;

String lAcorde[] = {"DO","DOS","RE","RES","MI","FA","FAS","SOL","SOLS","LA","LAS","SI"};

String acordesBrancos[] = {"DO", "RE","MI","FA","SOL","LA", "SI"};
String acordesPretos[] = {"DOS","RES", "FAS","SOLS", "LAS"};

int coordTeclasX[num_teclas];
int coordTeclasY[num_teclas];

bool pisca[NUM_LEDS];
bool estadoPisca = false;

bool teclaLigada[num_teclas];
bool estadoTecla = false;

unsigned long erro[NUM_LEDS];
int intervaloErro = 1000;

unsigned long tempoAnterior = 0;
int intervalo = 500;

uint32_t verde = strip.Color(0,150,0);
uint32_t amarelo = strip.Color(255,180,0);
uint32_t vermelho = strip.Color(188,0,0);
uint32_t preto = strip.Color(0,0,0);

const int teclaBrancoLargura = 30;
const int teclaBrancoAltura = 80;
const int teclaPretoLargura = 20;
const int teclaPretoAltura = 50;
const int tecladoY = 160;
const int maoX = 10;
const int maoY = 20;

int teclaPosX[12]; 

int teclaXPreta[5]; 
int teclaYPreta[5]; 
int teclaXBranca[7]; 
int teclaYBranca[7]; 

int dedoCoord[5][2] = { {20,100},{50,40},{70,30},{90,40},{110,50} };

void atualizaTecla(int nota, int dedo, uint16_t cor);
void botaoNaTecla(String notaNome, int dedo);
uint8_t showBMP(char *nm, int x, int y); 

unsigned long tempo;

//String filaNotas[];

void LeAcorde(String nota_recebida){

  int separa = nota_recebida.indexOf("_");
  String nota = nota_recebida.substring(0, separa);
  String status = nota_recebida.substring(separa + 1);

  for(int i = 0;i<NUM_LEDS;i++){
    if(nota == lAcorde[i]){

      if(status == "ON"){
        pisca[i] = false;
        teclaLigada[i] = true;
        strip.setPixelColor(i,verde);
      }
      else if(status == "OFF"){
        pisca[i] = false;
        teclaLigada[i] = false;
        strip.setPixelColor(i,preto);
      }
      else if(status == "PISCAR"){
        pisca[i] = true;
      }
      else if(status == "ERRO"){
        pisca[i] = false;
        erro[i] = millis();
        strip.setPixelColor(i,vermelho);
      }
    }
  }
  strip.show();
}

void desenhaTecladoBase() {
    int x = 10;
    x = (tela.width() - (7 * (teclaBrancoLargura + 2))) / 2;
    int x_base = x;

    for(int i = 0; i < 7; ++i)
    {
      int notaBranca;
      //teclas brancas
      if(i == 0 || i == 2 || i == 4 || i == 5 || i == 7)
      {
        teclaPosX[notaBranca] = x;
      }
      coordTeclasX[i] = x;
      tela.drawRect(x, tecladoY, teclaBrancoLargura, teclaBrancoAltura, TFT_WHITE);

      //tela.drawCircle(x + teclaBrancoLargura / 2, tecladoY + (teclaBrancoAltura * 3 / 4), 4, TFT_GREEN);
      //Serial.println("acorde " + acordesBrancos[i] + " x: " + String(x + teclaBrancoLargura / 2) + " y: " + String(tecladoY + (teclaBrancoAltura * 3 / 4)));
      x += teclaBrancoLargura + 2;
    }

    int offset[] = { 20, 52, 116, 148, 180 };

    int teclasBrancasX[] = {48, 80, 112, 144, 176, 208, 240};
    int teclasBrancasY = 220;

    int notaPreta[] = { 1, 3, 6, 8, 10 };
    

    int notaPreta2;
    for (int i = 0; i < 5; i++) {
      teclaPosX[notaPreta[i]] = x_base + offset[i];
      tela.fillRect(x_base + offset[i], tecladoY, teclaPretoLargura, teclaPretoAltura, TFT_GRAY);
      //tela.drawCircle(x_base + offset[i] + teclaPretoLargura / 2, tecladoY + (teclaPretoAltura * 1 / 2), 4, TFT_GREEN);
      //Serial.println("acorde " + acordesPretos[i] + " x: " + String(x_base + offset[i] + teclaPretoLargura / 2) + " y: " + String(tecladoY + (teclaPretoAltura * 1 / 2)));
      teclaXBranca[i] = x_base + offset[i] + (teclaPretoLargura / 2);
      teclaYBranca[i] = tecladoY + (teclaPretoAltura / 2);
    }
}

void desenha_clave_sol(String nota, bool ligar){
  int y_inicial = 40;
  int espacamento = 10;
  int largura_tela = tela.width();
  int x_bola = 100;
  int y_bola = 0;
  int inicio_txt = 180;

  for(int i = 0; i <5; i++){
    int y_pos = y_inicial + (i * espacamento);
    tela.drawLine(20, y_pos, 140, y_pos, TFT_WHITE);
  }

  int PAUTA_Y_INICIAL = 40;
  int PAUTA_ESPACO = 10;

  if(nota == "DO"  || nota == "DOS")  {   y_bola = PAUTA_Y_INICIAL + (5.0 * PAUTA_ESPACO); } // Dó (Linha suplementar)
  else if (nota == "RE"  || nota == "RES") {  y_bola = PAUTA_Y_INICIAL + (4.5 * PAUTA_ESPACO); } // Ré (Espaço)
  else if (nota == "MI") { x_bola = 90;  y_bola = PAUTA_Y_INICIAL + (4.0 * PAUTA_ESPACO);  }   // Mi (Linha)
  else if (nota == "FA"  || nota == "FAS")  {  y_bola = PAUTA_Y_INICIAL + (3.5 * PAUTA_ESPACO); } // Fá (Espaço)
  else if (nota == "SOL" || nota == "SOLS") {  y_bola = PAUTA_Y_INICIAL + (3.0 * PAUTA_ESPACO);  }   // Sol (Linha)
  else if (nota == "LA"  || nota == "LAS")  { y_bola = PAUTA_Y_INICIAL + (2.5 * PAUTA_ESPACO); } // Lá (Espaço)
  else if (nota == "SI"){ y_bola = PAUTA_Y_INICIAL + (2.0 * PAUTA_ESPACO); }   // Si (Linha)
  
  if(ligar){
    tela.fillCircle(x_bola, y_bola, 4 , TFT_YELLOW);
    tela.setTextSize(3);
    tela.setCursor(inicio_txt, 50);
    tela.setTextColor(TFT_WHITE, TFT_BLACK);
    tela.print(nota);
  }
  else{
    tela.fillRect(inicio_txt, 50, 80, 40, TFT_BLACK);
    tela.fillCircle(x_bola, y_bola, 4 , TFT_BLACK);
    for(int i = 0; i <5; i++){
      int y_pos = y_inicial + (i * espacamento);
      tela.drawLine(20, y_pos, 140, y_pos, TFT_WHITE);
    }
  }

}


void desenha_dica(String nota, bool ligar, char* acao) {
  int indice = -1;
  for(int i=0; i<12; i++){
    if(lAcorde[i] == nota) { indice = i; break; }
  }
  if(indice == -1) return;

  bool preta = (indice==1 || indice==3 || indice==6 || indice==8 || indice==10);
  
  int y = preta ? 185 : 220; 
  int x = 0;
 
  if(nota=="DO") x=63; else if(nota=="DOS") x=78;
  else if(nota=="RE") x=95; else if(nota=="RES") x=110;
  else if(nota=="MI") x=127; else if(nota=="FA") x=159;
  else if(nota=="FAS") x=174; else if(nota=="SOL") x=191;
  else if(nota=="SOLS") x=206; else if(nota=="LA") x=223;
  else if(nota=="LAS") x=238; else if(nota=="SI") x=255;

  if(strcmp(acao, "erro") == 0){
    //liga vermelho
    tela.fillCircle(x, y, 4, TFT_RED);
    strip.setPixelColor(indice, vermelho);

  }

  else if (ligar) {
    if(x>0) tela.fillCircle(x, y, 4, TFT_YELLOW);
    strip.setPixelColor(indice, amarelo);
    desenha_clave_sol(nota, true);
  } 
  
  else {
    uint16_t corFundo = TFT_BLACK;
    desenha_clave_sol(nota, false);
    if(x>0) tela.fillCircle(x, y, 4, corFundo);
    strip.setPixelColor(indice, strip.Color(0,0,0));
  }
  strip.show();
}


void verificarSerialPC() {
  if (Serial.available() > 0) {
    String cmd = Serial.readStringUntil('\n');
    cmd.trim();

    int sep = cmd.indexOf('_');
    if (sep > 0) {
      String nota = cmd.substring(0, sep);
      String acao = cmd.substring(sep + 1);

      if (acao == "YELLOW") {
        desenha_dica(nota, true, "desenha");
      } 
      else if (acao == "NOYELLOW") {

        desenha_dica(nota, false, "desenha");  
      }
      else if(acao == "RED"){
        desenha_dica(nota, true, "erro");
        Serial.println("ok");
      }
      else {
        LeAcorde(cmd);
        int idx = -1;
        for(int i=0; i<12; i++) if(lAcorde[i] == nota) idx=i;
        bool ehPreta = (idx==1 || idx==3 || idx==6 || idx==8 || idx==10);

        if (acao == "ON") desenha_circulo(!ehPreta, -1, 1);
        else desenha_circulo(!ehPreta, -1, 0);
      }
    }
  }
}
/*
void desenhaNumeroDedos() {
    tela.setTextSize(3);
    tela.setTextColor(TFT_BLUE);

    tela.setCursor(dedoCoord[0][0] + maoX - 13, dedoCoord[0][1] + maoY - 58); tela.print("5");
    tela.setCursor(dedoCoord[1][0] + maoX  - 13, dedoCoord[1][1] + maoY - 37); tela.print("1");
    tela.setCursor(dedoCoord[2][0] + maoX - 14, dedoCoord[2][1] + maoY - 35); tela.print("2");
    tela.setCursor(dedoCoord[3][0] + maoX - 15, dedoCoord[3][1] + maoY - 39); tela.print("3");
    tela.setCursor(dedoCoord[4][0] + maoX - 18, dedoCoord[4][1] + maoY - 40); tela.print("4");
}
*/
void desenha_circulo(bool cor_tecla, int indice_nota, bool estado)
{
  int brancoY = 220;
  int pretoY = 185;

  //indice do vetor corresponde a nota
  //63 = DO, 95 = RE, etc
  int brancoX[7] = {63, 95, 127, 159, 191, 223, 255};

  //sustenido dos acima
  int pretoX[5] = {78, 110, 174, 206, 238};

  //teclas pretas
  if(cor_tecla == 0)
  {
    if(estado == 0) 
    {
      tela.fillCircle(pretoX[indice_nota], pretoY, 4, TFT_GRAY);
      Serial.println("preto desligado");
    }
    else if (estado == 1) 
    {
      tela.fillCircle(pretoX[indice_nota], pretoY, 4, TFT_GREEN);
      Serial.println("preto ligado");
    }
  }
  
  //teclas brancas
  if(cor_tecla == 1)
  {
    if(estado == 0) 
    {
      tela.fillCircle(brancoX[indice_nota], brancoY, 4, TFT_BLACK);
      Serial.println("branco desligado");
    }
    else if (estado == 1) 
    {
      tela.fillCircle(brancoX[indice_nota], brancoY, 4, TFT_GREEN);  
      Serial.println("branco ligado");
    }
  }
}



void chama_do(){ LeAcorde("DO_ON"); Serial.println("DO_ON"); desenha_circulo(1, 0, 1);};
void desliga_do(){ LeAcorde("DO_OFF"); Serial.println("DO_OFF"); desenha_circulo(1, 0, 0);};

void chama_re(){ LeAcorde("RE_ON"); Serial.println("RE_ON"); desenha_circulo(1, 1, 1);};
void desliga_re(){ LeAcorde("RE_OFF"); Serial.println("RE_OFF"); desenha_circulo(1, 1, 0);};

void chama_mi(){ LeAcorde("MI_ON"); Serial.println("MI_ON"); desenha_circulo(1, 2, 1);};
void desliga_mi(){ LeAcorde("MI_OFF"); Serial.println("MI_OFF"); desenha_circulo(1, 2, 0);};

void chama_fa(){ LeAcorde("FA_ON"); Serial.println("FA_ON"); desenha_circulo(1, 3, 1);};
void desliga_fa(){ LeAcorde("FA_OFF"); Serial.println("FA_OFF"); desenha_circulo(1, 3, 0);};

void chama_sol(){ LeAcorde("SOL_ON"); Serial.println("SOL_ON"); desenha_circulo(1, 4, 1);};
void desliga_sol(){ LeAcorde("SOL_OFF"); Serial.println("SOL_OFF"); desenha_circulo(1, 4, 0);};

void chama_la(){ LeAcorde("LA_ON"); Serial.println("LA_ON"); desenha_circulo(1, 5, 1);};
void desliga_la(){ LeAcorde("LA_OFF"); Serial.println("LA_OFF"); desenha_circulo(1, 5, 0);};

void chama_si(){ LeAcorde("SI_ON"); Serial.println("SI_ON"); desenha_circulo(1, 6, 1);};
void desliga_si(){ LeAcorde("SI_OFF"); Serial.println("SI_OFF"); desenha_circulo(1, 6, 0);};

void chama_dos(){ LeAcorde("DOS_ON"); Serial.println("DOS_ON"); desenha_circulo(0, 0, 1);};
void desliga_dos(){ LeAcorde("DOS_OFF"); Serial.println("DOS_OFF"); desenha_circulo(0, 0, 0);};

void chama_res(){ LeAcorde("RES_ON"); Serial.println("RES_ON"); desenha_circulo(0, 1, 1);};
void desliga_res(){ LeAcorde("RES_OFF"); Serial.println("RES_OFF"); desenha_circulo(0, 1, 0);};

void chama_fas(){ LeAcorde("FAS_ON"); Serial.println("FAS_ON"); desenha_circulo(0, 2, 1);};
void desliga_fas(){ LeAcorde("FAS_OFF"); Serial.println("FAS_OFF"); desenha_circulo(0, 2, 0);};

void chama_sols(){ LeAcorde("SOLS_ON"); Serial.println("SOLS_ON"); desenha_circulo(0, 3, 1);};
void desliga_sols(){ LeAcorde("SOLS_OFF"); Serial.println("SOLS_OFF"); desenha_circulo(0, 3, 0);};

void chama_las(){ LeAcorde("LAS_ON"); Serial.println("LAS_ON"); desenha_circulo(0, 4, 1);};
void desliga_las(){ LeAcorde("LAS_OFF"); Serial.println("LAS_OFF"); desenha_circulo(0, 4, 0);};

void setup() {
  Serial.begin(9600);
  strip.begin();

  for(int i = 0; i < NUM_LEDS; i++){
    pisca[i] = false;
  }
  strip.show();
  strip.setBrightness(100);

  uint16_t ID = tela.readID();
  if (ID == 0xD3D3) ID = 0x9486; 
  tela.begin(ID);
  tela.setRotation(1);
  tela.fillScreen(TFT_BLACK);
  
  if (!SD.begin(SD_CS)) {
    Serial.println("Falha no SD Card!"); 
  } else {
    Serial.println("SD Card OK!"); 
  }

  uint8_t ret = showBMP("testebmp24", maoX, maoY);
  
  if (ret == 0) Serial.println(F("BMP OK"));
  else { Serial.print("BMP Erro: "); Serial.println(ret); }
  //drawTrebleClef(10, 5, TFT_WHITE);

  int largura = 50; 
  int altura = 80;

  int y_inicial = 40;
  int espacamento = 10;
  int largura_tela = tela.width();
  Serial.println(largura_tela);

  tela.drawBitmap(20, 20, claveSol, largura, altura, TFT_WHITE);
  
  for(int i = 0; i <5; i++){
    int y_pos = y_inicial + (i * espacamento);
    tela.drawLine(20, y_pos, 140, y_pos, TFT_WHITE);
  }

  //botoes
  botao[0].setPressHandler(chama_do);
  botao[0].setReleaseHandler(desliga_do);

  botao[1].setPressHandler(chama_dos);
  botao[1].setReleaseHandler(desliga_dos);

  botao[2].setPressHandler(chama_re);
  botao[2].setReleaseHandler(desliga_re);

  botao[3].setPressHandler(chama_res);
  botao[3].setReleaseHandler(desliga_res);
  
  botao[4].setPressHandler(chama_mi);
  botao[4].setReleaseHandler(desliga_mi);
  
  botao[5].setPressHandler(chama_fa);
  botao[5].setReleaseHandler(desliga_fa);
  
  botao[6].setPressHandler(chama_fas);
  botao[6].setReleaseHandler(desliga_fas);
  
  botao[7].setPressHandler(chama_sol);
  botao[7].setReleaseHandler(desliga_sol);
  
  botao[8].setPressHandler(chama_sols);
  botao[8].setReleaseHandler(desliga_sols);
  
  botao[9].setPressHandler(chama_la);
  botao[9].setReleaseHandler(desliga_la);
  
  botao[10].setPressHandler(chama_las);
  botao[10].setReleaseHandler(desliga_las);
  
  botao[11].setPressHandler(chama_si);
  botao[11].setReleaseHandler(desliga_si);

  desenhaTecladoBase();
  //desenhaNumeroDedos();
}

void loop() {
  //processos dos botoes
  botao[0].process();
  botao[1].process();
  botao[2].process();
  botao[3].process();
  botao[4].process();
  botao[5].process();
  botao[6].process();
  botao[7].process();
  botao[8].process();
  botao[9].process();
  botao[10].process();
  botao[11].process(); 
  
  verificarSerialPC();

  //LeAcorde();
  unsigned long tempoAtual = millis();
  if (tempoAtual - tempoAnterior >= intervalo){
    tempoAnterior = tempoAtual;
    estadoPisca = !estadoPisca;
  }
  for(int i = 0; i < NUM_LEDS; i++){
    if(pisca[i] == true){
      if(estadoPisca == true){
        strip.setPixelColor(i, amarelo);
      } 
      else {
        strip.setPixelColor(i, preto);
      }
    }
  }
  for(int j = 0; j< NUM_LEDS; j++){
    if (erro[j] != 0 && tempoAtual - erro[j] >= intervaloErro){
      erro[j] = 0;
      strip.setPixelColor(j, preto);
    }
  }
  strip.show();
}

uint16_t read16(File& f) {
    uint16_t result;         
    f.read((uint8_t*)&result, sizeof(result));
    return result;
}

uint32_t read32(File& f) {
    uint32_t result;
    f.read((uint8_t*)&result, sizeof(result));
    return result;
}

uint8_t showBMP(char *nm, int x, int y) {
    File bmpFile;
    int bmpWidth, bmpHeight;    
    uint8_t bmpDepth;           
    uint32_t bmpImageoffset;    
    uint32_t rowSize;           
    uint8_t sdbuffer[3 * BUFFPIXEL];    
    uint8_t buffidx = sizeof(sdbuffer); 
    boolean flip = true;        
    int w, h, row, col;
    uint8_t r, g, b;
    uint32_t pos = 0;
    boolean first = true; 

    if ((x >= tela.width()) || (y >= tela.height())) return 1;

    if ((bmpFile = SD.open(nm)) == NULL) {
        return 1; 
    }

    if (read16(bmpFile) == 0x4D42) { 
        (void) read32(bmpFile);     
        (void) read32(bmpFile);     
        bmpImageoffset = read32(bmpFile);       
        (void) read32(bmpFile);     
        bmpWidth = read32(bmpFile);
        bmpHeight = read32(bmpFile);
        if (read16(bmpFile) == 1) { 
            bmpDepth = read16(bmpFile); 
            if ((bmpDepth == 24) && (read32(bmpFile) == 0)) { 

                rowSize = (bmpWidth * 3 + 3) & ~3;
                if (bmpHeight < 0) {              
                    bmpHeight = -bmpHeight;
                    flip = false;
                }

                w = bmpWidth;
                h = bmpHeight;
                if ((x + w) >= tela.width()) w = tela.width() - x;
                if ((y + h) >= tela.height()) h = tela.height() - y;

                tela.setAddrWindow(x, y, x + w - 1, y + h - 1);

                for (row = 0; row < h; row++) { 
                    if (flip)   
                        pos = bmpImageoffset + (bmpHeight - 1 - row) * rowSize;
                    else        
                        pos = bmpImageoffset + row * rowSize;
                    if (bmpFile.position() != pos) { 
                        bmpFile.seek(pos);
                        buffidx = sizeof(sdbuffer); 
                    }
                    for (col = 0; col < w; col++) { 
                        if (buffidx >= sizeof(sdbuffer)) { 
                            bmpFile.read(sdbuffer, sizeof(sdbuffer));
                            buffidx = 0; 
                        }
                        b = sdbuffer[buffidx++];
                        g = sdbuffer[buffidx++];
                        r = sdbuffer[buffidx++];
                        
                        uint16_t color = tela.color565(r, g, b);
                        
                        tela.pushColors(&color, 1, first);
                        first = false; 
                    } 
                } 
            }
        }
    }
    bmpFile.close();
    return 0;
}