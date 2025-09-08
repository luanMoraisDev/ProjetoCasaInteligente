/*
          AUTOR:   Luan Vinicius dos Santos Morais
          DATA:    27/08/2025
          SKETCH:  CasaInteligente  
*/

//Inclusão de Biliotecas
#include <WiFi.h>
#include <Adafruit_GFX.h>     // Core graphics library
#include <Adafruit_ST7789.h>  // Hardware-specific library for ST7789
#include <SPI.h>

//Definições de pinos
#define TFT_CS 21
#define TFT_DC 22
#define TFT_MOSI 23
#define TFT_SCLK 18
#define TFT_RST -1

#define botaoProximo 26  // Evitar uso pino que não tem resitor interno, previne pontos flutuantes e oscilações
#define botaoVoltar 27   // Evitar uso pino que não tem resitor interno, previne pontos flutuantes e oscilações
#define botaoControle 25
#define botaoConfirmar 14
#define buzzer 15

#define luzQuarto1 4
#define luzQuarto2 2
/*
#define luzQuarto3 
#define luzSala
#define luzCozinha
#define luzBanheiro
#define luzGaragem
*/
//Criando objetos
Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);

//Variáveis de controle
int selecionarMenu = 1;
int ultimoMenu = 0;
bool estadoBotaoControle = false;
int controleInterruptores = 1;
bool sair = false;

//Váriaveis controle de comodos
bool estadoLuzQuarto1 = false;
bool estadoLuzQuarto2 = false;
bool estadoLuzQuarto3 = false;
bool estadoLuzGaragem = false;
bool estadoLuzCozinha = false;
bool estadoLuzBanheiro = false;
bool estadoLuzSala = false;

//Variáveis sensores


//Configuração de WIFI

void setup() {
  pinMode(botaoVoltar, INPUT_PULLUP);
  pinMode(botaoProximo, INPUT_PULLUP);
  pinMode(botaoControle, INPUT_PULLUP);
  pinMode(botaoConfirmar, INPUT_PULLUP);
  pinMode(buzzer, OUTPUT);


//  pinMode(luzBanheiro, OUTPUT);
  pinMode(luzQuarto1, OUTPUT);
  pinMode(luzQuarto2, OUTPUT);
  
  /*
  pinMode(luzQuarto3, OUTPUT);
  pinMode(luzCozinha, OUTPUT);
  pinMode(luzGaragem, OUTPUT);
  pinMode(luzSala, OUTPUT);
  */

  Serial.begin(9600);
  inicarDisplay();
}

void loop() {
  Serial.println(selecionarMenu);
  controleBotoesNavegar();

  while(selecionarMenu == 1){
    
    layout();
    controleBotoesNavegar();
    tft.setTextColor(ST77XX_WHITE, ST77XX_BLACK);
    tft.setCursor(45,5);
    tft.print("INTERRUPTORES");

    if(digitalRead(botaoConfirmar) == LOW){
        sair = !sair;
    }

    while(sair){
      controleBotoesInterruptores();
      switch(controleInterruptores) {
        case 1:
            exibirControlesInterruptores(1);

            if(digitalRead(botaoControle) == LOW){
                estadoLuzQuarto1 = !estadoLuzQuarto1;
                delay(100);
            }

            if(estadoLuzQuarto1){
                digitalWrite(luzQuarto1, HIGH);
                tft.setCursor(150, 30);
                tft.setTextColor(ST77XX_GREEN, ST77XX_WHITE);
                tft.print("ON ");
            } else {
                digitalWrite(luzQuarto1, LOW);
                tft.setCursor(150, 30);
                tft.setTextColor(ST77XX_RED, ST77XX_WHITE);
                tft.print("OFF");
            }
        break;

        case 2:
            exibirControlesInterruptores(2);

            if(digitalRead(botaoControle) == LOW){
                estadoLuzQuarto2 = !estadoLuzQuarto2;
                delay(100);
            }

            if(estadoLuzQuarto2){
                digitalWrite(luzQuarto2, HIGH);
                tft.setCursor(150, 50);
                tft.setTextColor(ST77XX_GREEN, ST77XX_WHITE);
                tft.print("ON ");
            } else {
                digitalWrite(luzQuarto2, LOW);
                tft.setCursor(150, 50);
                tft.setTextColor(ST77XX_RED, ST77XX_WHITE);
                tft.print("OFF");
            }
        break;
      }
      if(digitalRead(botaoConfirmar) == LOW){
        sair = !sair;
      }
    }
  }
  while(selecionarMenu == 2){
    layout();
    controleBotoesNavegar();
    Serial.println("While menu 2");
    tft.setTextColor(ST77XX_WHITE, ST77XX_BLACK);
    tft.setCursor(45,5);
    tft.print("  SENSORES     ");
    
  }

  while(selecionarMenu == 3){
    layout();
    controleBotoesNavegar();
    Serial.println("While menu 3");
    tft.setTextColor(ST77XX_WHITE, ST77XX_BLACK);
    tft.setCursor(45,5);
    tft.print(" ATUADORES    ");
  }
}

//Funções

void inicarDisplay() {
  tft.init(240, 320);
  tft.invertDisplay(false);
  tft.fillScreen(0x2B40);
  tft.setTextSize(4);
  tft.setTextColor(ST77XX_BLACK);
  tft.setCursor(70, 40);
  tft.print("SEMA");
  tft.setTextSize(2);
  tft.setCursor(30, 100);
  tft.print("Casa Inteligente");
  delay(1000);
  tft.fillScreen(0x0000);

  tft.setCursor(0, 50);
}

void layout(){
  tft.drawRect(0, 0, 240, 320, 0x2B40);
  tft.drawLine(0, 25, 240, 25, 0x2B40);
}


void controleBotoesNavegar() {
  // Controle de botão avançar e retornar
  if (digitalRead(botaoProximo) == LOW) {
    if (selecionarMenu >= 3) {
      selecionarMenu = 3;
    } else {
      delay(150);
      selecionarMenu++;
    }
  } else if (digitalRead(botaoVoltar) == LOW) {
    if (selecionarMenu <= 1) {
      selecionarMenu = 1;
    } else {
      delay(150);
      selecionarMenu--;
    }
  }

     if (digitalRead(botaoControle) == LOW) {
            estadoBotaoControle = !estadoBotaoControle;
            delay(150);
     }
}

void controleBotoesInterruptores(){
    if (digitalRead(botaoProximo) == LOW) {
    if (controleInterruptores >= 7) {
      controleInterruptores = 7;
    } else {
      delay(150);
      controleInterruptores++;
    }
  } else if (digitalRead(botaoVoltar) == LOW) {
    if (controleInterruptores <= 1) {
      controleInterruptores = 1;
    } else {
      delay(150);
      controleInterruptores--;
    }
  }
}

void exibirControlesInterruptores(int op){
  if(op == 1){
    tft.setTextColor(ST77XX_WHITE, ST77XX_BLACK);
    tft.setCursor(10,30);
    tft.print("Quarto 1 <");

    tft.setCursor(10,50);
    tft.print("Quarto 2  ");
    
    tft.setCursor(10,70);
    tft.print("Quarto 3  ");
    
    tft.setCursor(10,90);
    tft.print("Banheiro  ");

    tft.setCursor(10,110);
    tft.print("Cozinha   ");
    
    tft.setCursor(10,130);
    tft.print("Sala      ");
    
    tft.setCursor(10,150);
    tft.print("Garagem   ");
  }

  if(op == 2){
    tft.setTextColor(ST77XX_WHITE, ST77XX_BLACK);
    tft.setCursor(10,30);
    tft.print("Quarto 1  ");

    tft.setCursor(10,50);
    tft.print("Quarto 2 <");
    
    tft.setCursor(10,70);
    tft.print("Quarto 3  ");
    
    tft.setCursor(10,90);
    tft.print("Banheiro  ");

    tft.setCursor(10,110);
    tft.print("Cozinha   ");
    
    tft.setCursor(10,130);
    tft.print("Sala      ");
    
    tft.setCursor(10,150);
    tft.print("Garagem   ");  
  }

  if(op == 3){
    tft.setTextColor(ST77XX_WHITE, ST77XX_BLACK);
    tft.setCursor(10,30);
    tft.print("Quarto 1  ");

    tft.setCursor(10,50);
    tft.print("Quarto 2  ");
    
    tft.setCursor(10,70);
    tft.print("Quarto 3 <");
    
    tft.setCursor(10,90);
    tft.print("Banheiro  ");

    tft.setCursor(10,110);
    tft.print("Cozinha   ");
    
    tft.setCursor(10,130);
    tft.print("Sala      ");
    
    tft.setCursor(10,150);
    tft.print("Garagem   ");
  }

  if(op == 4){
    tft.setTextColor(ST77XX_WHITE, ST77XX_BLACK);
    tft.setCursor(10,30);
    tft.print("Quarto 1  ");

    tft.setCursor(10,50);
    tft.print("Quarto 2  ");
    
    tft.setCursor(10,70);
    tft.print("Quarto 3  ");
    
    tft.setCursor(10,90);
    tft.print("Banheiro <");

    tft.setCursor(10,110);
    tft.print("Cozinha   ");
    
    tft.setCursor(10,130);
    tft.print("Sala      ");
    
    tft.setCursor(10,150);
    tft.print("Garagem   ");
  }

  if(op == 5){
    tft.setTextColor(ST77XX_WHITE, ST77XX_BLACK);
    tft.setCursor(10,30);
    tft.print("Quarto 1  ");

    tft.setCursor(10,50);
    tft.print("Quarto 2  ");
    
    tft.setCursor(10,70);
    tft.print("Quarto 3  ");
    
    tft.setCursor(10,90);
    tft.print("Banheiro  ");

    tft.setCursor(10,110);
    tft.print("Cozinha  <");
    
    tft.setCursor(10,130);
    tft.print("Sala      ");
    
    tft.setCursor(10,150);
    tft.print("Garagem   ");
  }

  if(op == 6){
    tft.setTextColor(ST77XX_WHITE, ST77XX_BLACK);
    tft.setCursor(10,30);
    tft.print("Quarto 1  ");

    tft.setCursor(10,50);
    tft.print("Quarto 2  ");
    
    tft.setCursor(10,70);
    tft.print("Quarto 3  ");
    
    tft.setCursor(10,90);
    tft.print("Banheiro  ");

    tft.setCursor(10,110);
    tft.print("Cozinha   ");
    
    tft.setCursor(10,130);
    tft.print("Sala     <");
    
    tft.setCursor(10,150);
    tft.print("Garagem   ");
  }

  if(op == 7){
    tft.setTextColor(ST77XX_WHITE, ST77XX_BLACK);
    tft.setCursor(10,30);
    tft.print("Quarto 1  ");

    tft.setCursor(10,50);
    tft.print("Quarto 2  ");
    
    tft.setCursor(10,70);
    tft.print("Quarto 3  ");
    
    tft.setCursor(10,90);
    tft.print("Banheiro  ");

    tft.setCursor(10,110);
    tft.print("Cozinha   ");
    
    tft.setCursor(10,130);
    tft.print("Sala      ");
    
    tft.setCursor(10,150);
    tft.print("Garagem  <");
  }

}