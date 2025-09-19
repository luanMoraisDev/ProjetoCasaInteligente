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

#define botaoProximo 12  // Evitar uso pino que não tem resitor interno, previne pontos flutuantes e oscilações
#define botaoConfirmar 13
#define botaoVoltar 14  
#define buzzer 15

#define luzQuarto1 27
#define luzQuarto2 26
#define luzQuarto3 25
#define luzSala 33
#define luzGaragem 4
#define luzCozinha 2
#define luzBanheiro 15

#define NTC 39

//Declaração de objetos
Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);

//Váriaveis de controle
int controleInterruptores = 1;
bool estadoLuzQuarto1 = false;
bool estadoLuzQuarto2 = false;
bool estadoLuzQuarto3 = false;
bool estadoLuzSala = false;
bool estadoLuzGaragem = false;
bool estadoLuzCozinha = false;
bool estadoLuzBanheiro = false;

//Variáveis sensores
float leitura = analogRead(NTC);
float temperatura = (1 / ( (log(1/(1024 /leitura - 1))) /(3950) + 1 / (298.15))) - 273.15;

//Funções
void layout(){
  tft.drawRect(0, 0, 240, 320, 0x2B40);
  tft.drawLine(0, 22, 240, 22, 0x2B40);
  tft.setTextColor(ST77XX_WHITE, ST77XX_BLACK);
  tft.setCursor(45,5);
  tft.print("INTERRUPTORES");

  tft.drawLine(0, 177, 240, 177, 0x2B40);
  tft.setCursor(50,185);
  tft.print("TEMPERATURA");
  tft.setCursor(30, 215);
  tft.setTextSize(4);
  tft.setTextColor(ST77XX_BLUE, ST77XX_BLACK);
  tft.print(temperatura);
  tft.setTextSize(2);
}

void ajustes(){ //Apenas para já começar com o texto de OFF na tela
  tft.setTextColor(ST77XX_RED, ST77XX_WHITE);
  tft.setCursor(150, 30);
  tft.print("OFF");

  tft.setCursor(150, 50);
  tft.print("OFF");

  tft.setCursor(150, 70);
  tft.print("OFF");

  tft.setCursor(150, 90);
  tft.print("OFF");

  tft.setCursor(150, 110);
  tft.print("OFF");

  tft.setCursor(150, 130);
  tft.print("OFF");

  tft.setCursor(150, 150);
  tft.print("OFF");

}

void controleBotoesInterruptores(){  
  if (digitalRead(botaoProximo) == LOW) {
    if (controleInterruptores >= 7) {
      controleInterruptores = 7;
    } else {
      delay(100);
      controleInterruptores++;
    }
  } else if (digitalRead(botaoVoltar) == LOW) {
    if (controleInterruptores <= 1) {
      controleInterruptores = 1;
    } else {
      delay(100);
      controleInterruptores--;
    }
  }
}

void exibirInterruptores(int op){
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

void setup(){
  pinMode(botaoVoltar, INPUT_PULLUP);
  pinMode(botaoProximo, INPUT_PULLUP);
  pinMode(botaoConfirmar, INPUT_PULLUP);
  pinMode(buzzer, OUTPUT);

  pinMode(luzBanheiro , OUTPUT);
  pinMode(luzQuarto1 , OUTPUT);
  pinMode(luzQuarto2 , OUTPUT);
  pinMode(luzQuarto3 , OUTPUT);
  pinMode(luzGaragem , OUTPUT);
  pinMode(luzCozinha , OUTPUT);
  pinMode(luzSala, OUTPUT);

  pinMode(NTC, INPUT);

  Serial.begin(9600);

  tft.init(240, 320);
  tft.invertDisplay(false);
  tft.fillScreen(ST77XX_BLACK);
  tft.setTextSize(2);
  tft.setTextColor(ST77XX_BLACK);

  layout();
  ajustes();
}


void loop(){
  leitura = analogRead(NTC);
  //temperatura = (1 / ( (log(1/(1024 /leitura - 1))) /(3950) + 1 / (298.15))) - 273.15;
  temperatura = (1 / ( (log( (4095.0 / leitura) - 1.0 ) / 3950.0 + (1.0 / 298.15) ) )) - 273.15;

  layout();

  controleBotoesInterruptores();

  switch(controleInterruptores){
    case 1:
      exibirInterruptores(1);

      if(digitalRead(botaoConfirmar) == LOW){
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
      exibirInterruptores(2);

      if(digitalRead(botaoConfirmar) == LOW){
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

    case 3:
      exibirInterruptores(3);

      if(digitalRead(botaoConfirmar) == LOW){
        estadoLuzQuarto3 = !estadoLuzQuarto3;
        delay(100);
      }

      if(estadoLuzQuarto3){
        digitalWrite(luzQuarto3, HIGH);
        tft.setCursor(150, 70);
        tft.setTextColor(ST77XX_GREEN, ST77XX_WHITE);
        tft.print("ON ");
      } else {
        digitalWrite(luzQuarto3, LOW);
        tft.setCursor(150, 70);
        tft.setTextColor(ST77XX_RED, ST77XX_WHITE);
        tft.print("OFF");
      }
    break;
    
    case 4:
      exibirInterruptores(4);

      if(digitalRead(botaoConfirmar) == LOW){
        estadoLuzBanheiro = !estadoLuzBanheiro;
        delay(100);
      }

      if(estadoLuzBanheiro){
        digitalWrite(luzBanheiro, HIGH);
        tft.setCursor(150, 90);
        tft.setTextColor(ST77XX_GREEN, ST77XX_WHITE);
        tft.print("ON ");
      } else {
        digitalWrite(luzBanheiro, LOW);
        tft.setCursor(150, 90);
        tft.setTextColor(ST77XX_RED, ST77XX_WHITE);
        tft.print("OFF");
      }
    break;

    case 5:
      exibirInterruptores(5);

      if(digitalRead(botaoConfirmar) == LOW){
        estadoLuzCozinha = !estadoLuzCozinha;
        delay(100);
      }

      if(estadoLuzCozinha){
        digitalWrite(luzCozinha, HIGH);
        tft.setCursor(150, 110);
        tft.setTextColor(ST77XX_GREEN, ST77XX_WHITE);
        tft.print("ON ");
      } else {
        digitalWrite(luzCozinha, LOW);
        tft.setCursor(150, 110);
        tft.setTextColor(ST77XX_RED, ST77XX_WHITE);
        tft.print("OFF");
      }
    break;

    case 6:
      exibirInterruptores(6);

      if(digitalRead(botaoConfirmar) == LOW){
        estadoLuzSala = !estadoLuzSala;
        delay(100);
      }

      if(estadoLuzSala){
        digitalWrite(luzSala, HIGH);
        tft.setCursor(150, 130);
        tft.setTextColor(ST77XX_GREEN, ST77XX_WHITE);
        tft.print("ON ");
      } else {
        digitalWrite(luzSala, LOW);
        tft.setCursor(150, 130);
        tft.setTextColor(ST77XX_RED, ST77XX_WHITE);
        tft.print("OFF");
      }      
    break;

    case 7:
      exibirInterruptores(7);

      if(digitalRead(botaoConfirmar) == LOW){
        estadoLuzGaragem = !estadoLuzGaragem;
        delay(100);
      }

      if(estadoLuzGaragem){
        digitalWrite(luzGaragem, HIGH);
        tft.setCursor(150, 150);
        tft.setTextColor(ST77XX_GREEN, ST77XX_WHITE);
        tft.print("ON ");
      } else {
        digitalWrite(luzGaragem, LOW);
        tft.setCursor(150, 150);
        tft.setTextColor(ST77XX_RED, ST77XX_WHITE);
        tft.print("OFF");
      }
    break;

  }

}