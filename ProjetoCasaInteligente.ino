/*
Sketch: Automação residencial com ESP32
Autor: Luan Vinicius
Ultima modificação: 25.09.2025
*/

// Inclusão de Bibliotecas
#include <Adafruit_GFX.h>       
#include <Adafruit_ST7789.h>
#include "RTClib.h" 
#include <Wire.h>
#include <SPI.h>

// Definição de Pinos
#define TFT_CS 19
#define TFT_DC 5
#define TFT_MOSI 23
#define TFT_SCLK 18
#define TFT_RST -1

#define botaoProximo 12
#define botaoVoltar 14
#define botaoConfirmar 27

#define luzQuarto1 15
#define luzQuarto2 2
#define luzQuarto3 4
#define luzSala 16
#define luzGaragem 17 
#define luzCozinha 26
#define luzBanheiro 25
#define buzzer 33

// Criação de objetos
Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);

RTC_DS1307 rtc;
char daysOfTheWeek[7][12] = {"Dom", "Seg", "Ter", "Qua", "Qui", "Sex", "Sat"};

// Criação de variáveis
int segundoAtual = 0 , minutoAtual = 0 , horaAtual = 0 , diaAtual = 0 , mesAtual = 0 , anoAtual = 0; 
String semanaAtual = "";

int controleInterruptores = 1;
bool estadoLuzQuarto1 = false;
bool estadoLuzQuarto2 = false;
bool estadoLuzQuarto3 = false;
bool estadoLuzSala = false;
bool estadoLuzGaragem = false;
bool estadoLuzCozinha = false;
bool estadoLuzBanheiro = false;

// Criação de funções
void iniciarDisplay(){
  tft.init(240, 320);
  tft.setRotation(0);
  tft.invertDisplay(false);
  tft.fillScreen(ST77XX_BLACK);
  tft.setTextColor(ST77XX_WHITE);
  tft.setTextSize(2);
}

void layout(){
  tft.drawRect(0, 0, 240, 320, ST77XX_BLUE);
  tft.drawFastHLine(0, 20, 240, ST77XX_BLUE);
  tft.drawFastVLine(45, 0, 20, ST77XX_BLUE);
  tft.drawFastVLine(170, 0, 20, ST77XX_BLUE);
  tft.drawFastHLine(0, 80, 240, ST77XX_BLUE);
  tft.drawFastVLine(120, 20, 60, ST77XX_BLUE);
  tft.drawFastHLine(0, 100, 240, ST77XX_BLUE);
}

void RTC(){
  DateTime now = rtc.now();
  
  int segundo = now.second();
  int minuto = now.minute();
  int hora = now.hour();
  int dia = now.day();
  int mes = now.month();
  int ano = now.year();
  String semana = daysOfTheWeek[now.dayOfTheWeek()];

  tft.setTextColor(ST77XX_WHITE, ST77XX_BLACK);

  if (!semana.equals(semanaAtual)) {
    semanaAtual = semana;  
    tft.setCursor(5,3);
    tft.setTextSize(2);
    tft.fillRect(5, 3, 25, 15, ST77XX_BLACK);
    tft.print(semana);
  }

  if (dia != diaAtual) {
    diaAtual = dia;  
    tft.setCursor(55,3);
    tft.setTextSize(2);
    tft.fillRect(55, 3, 25, 15, ST77XX_BLACK);
    tft.print(dia);
    tft.print(".");
  }

  if (mes != mesAtual) {
    mesAtual = mes;  
    tft.setCursor(90,3);
    tft.setTextSize(2);
    tft.fillRect(90, 3, 25, 15, ST77XX_BLACK);
    tft.print(mes);
    tft.print(".");
  }

  if (ano != anoAtual) {
    anoAtual = ano;  
    tft.setCursor(115,3);
    tft.setTextSize(2);
    tft.fillRect(115, 3, 25, 15, ST77XX_BLACK);
    tft.print(ano);
  }

  if (hora != horaAtual) {
    horaAtual = hora;
    tft.setCursor(177,3);
    tft.setTextSize(2);
    tft.fillRect(177, 3, 25, 15, ST77XX_BLACK);
    tft.print(hora);
    tft.setCursor(199,3);
    tft.print(":");
  }

  if (minuto != minutoAtual) {
    minutoAtual = minuto;
    tft.setCursor(210,3);
    tft.setTextSize(2);
    tft.fillRect(210, 3, 25, 15, ST77XX_BLACK);
    tft.print(minuto);
  }

}

void temperaturaNTC(){
  float leitura = analogRead(36);
  float temp = (1 / ( (log(1/(4095 /leitura - 1))) /(3950) + 1 / (298.15))) - 273.15;  
  float tempAtual;

  tft.setTextSize(2);
  tft.setCursor(5, 25);
  tft.setTextColor(ST77XX_WHITE, ST77XX_BLACK);
  tft.print("TEMP. NTC");
  
  if(temp != tempAtual){
    tempAtual = temp;
    Serial.println(temp);
    tft.setCursor(10, 50);
    tft.setTextSize(3);
    tft.fillRect(10,49, 90, 25, ST77XX_BLACK);
    
    if(temp > 50){
      tft.setTextColor(ST77XX_RED);
      tft.print(temp);
    } else if(temp < 20){
      tft.setTextColor(ST77XX_BLUE);
      tft.print(temp);
    } else{
      tft.setTextColor(ST77XX_WHITE);
      tft.print(temp);
    }


  }

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
  tft.setTextSize(2);
  tft.setCursor(45, 83);
  tft.print("Interruptores");

  if(op == 1){
    tft.setTextColor(ST77XX_WHITE, ST77XX_BLACK);
    tft.setCursor(10,110);
    tft.print("Quarto 1 <");

    tft.setCursor(10,130);
    tft.print("Quarto 2  ");
    
    tft.setCursor(10,150);
    tft.print("Quarto 3  ");
    
    tft.setCursor(10,170);
    tft.print("Banheiro  ");

    tft.setCursor(10,190);
    tft.print("Cozinha   ");
    
    tft.setCursor(10,210);
    tft.print("Sala      ");
    
    tft.setCursor(10,230);
    tft.print("Garagem   ");
  }

  if(op == 2){
    tft.setTextColor(ST77XX_WHITE, ST77XX_BLACK);
    tft.setCursor(10,110);
    tft.print("Quarto 1  ");

    tft.setCursor(10,130);
    tft.print("Quarto 2 <");
    
    tft.setCursor(10,150);
    tft.print("Quarto 3  ");
    
    tft.setCursor(10,170);
    tft.print("Banheiro  ");

    tft.setCursor(10,190);
    tft.print("Cozinha   ");
    
    tft.setCursor(10,210);
    tft.print("Sala      ");
    
    tft.setCursor(10,230);
    tft.print("Garagem   ");
  }

  if(op == 3){
   tft.setTextColor(ST77XX_WHITE, ST77XX_BLACK);
    tft.setCursor(10,110);
    tft.print("Quarto 1  ");

    tft.setCursor(10,130);
    tft.print("Quarto 2  ");
    
    tft.setCursor(10,150);
    tft.print("Quarto 3 <");
    
    tft.setCursor(10,170);
    tft.print("Banheiro  ");

    tft.setCursor(10,190);
    tft.print("Cozinha   ");
    
    tft.setCursor(10,210);
    tft.print("Sala      ");
    
    tft.setCursor(10,230);
    tft.print("Garagem   ");
  }

  if(op == 4){
    tft.setTextColor(ST77XX_WHITE, ST77XX_BLACK);
    tft.setCursor(10,110);
    tft.print("Quarto 1  ");

    tft.setCursor(10,130);
    tft.print("Quarto 2  ");
    
    tft.setCursor(10,150);
    tft.print("Quarto 3  ");
    
    tft.setCursor(10,170);
    tft.print("Banheiro <");

    tft.setCursor(10,190);
    tft.print("Cozinha   ");
    
    tft.setCursor(10,210);
    tft.print("Sala      ");
    
    tft.setCursor(10,230);
    tft.print("Garagem   ");
  }

  if(op == 5){
    tft.setTextColor(ST77XX_WHITE, ST77XX_BLACK);
    tft.setCursor(10,110);
    tft.print("Quarto 1  ");

    tft.setCursor(10,130);
    tft.print("Quarto 2  ");
    
    tft.setCursor(10,150);
    tft.print("Quarto 3  ");
    
    tft.setCursor(10,170);
    tft.print("Banheiro  ");

    tft.setCursor(10,190);
    tft.print("Cozinha  <");
    
    tft.setCursor(10,210);
    tft.print("Sala      ");
    
    tft.setCursor(10,230);
    tft.print("Garagem   ");
  }

  if(op == 6){
    tft.setTextColor(ST77XX_WHITE, ST77XX_BLACK);
    tft.setCursor(10,110);
    tft.print("Quarto 1  ");

    tft.setCursor(10,130);
    tft.print("Quarto 2  ");
    
    tft.setCursor(10,150);
    tft.print("Quarto 3  ");
    
    tft.setCursor(10,170);
    tft.print("Banheiro  ");

    tft.setCursor(10,190);
    tft.print("Cozinha   ");
    
    tft.setCursor(10,210);
    tft.print("Sala     <");
    
    tft.setCursor(10,230);
    tft.print("Garagem   ");
  }

  if(op == 7){
    tft.setTextColor(ST77XX_WHITE, ST77XX_BLACK);
    tft.setCursor(10,110);
    tft.print("Quarto 1  ");

    tft.setCursor(10,130);
    tft.print("Quarto 2  ");
    
    tft.setCursor(10,150);
    tft.print("Quarto 3  ");
    
    tft.setCursor(10,170);
    tft.print("Banheiro  ");

    tft.setCursor(10,190);
    tft.print("Cozinha   ");
    
    tft.setCursor(10,210);
    tft.print("Sala      ");
    
    tft.setCursor(10,230);
    tft.print("Garagem  <");
  }

}

void ajustesDisplay(){ //Correção de posição inical dos botões ON/OFF
  tft.setTextColor(ST77XX_RED, ST77XX_WHITE);
  tft.setCursor(150, 110);  tft.print("OFF");
  tft.setCursor(150, 130);  tft.print("OFF");
  tft.setCursor(150, 150);   tft.print("OFF");
  tft.setCursor(150, 170);  tft.print("OFF");
  tft.setCursor(150, 190);  tft.print("OFF");
  tft.setCursor(150, 210);  tft.print("OFF");
  tft.setCursor(150, 230);  tft.print("OFF");
}

void setup() {
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
  
  iniciarDisplay();

  Serial.begin(57600);
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  if (! rtc.isrunning()) {
    Serial.println("RTC is NOT running!");
    // following line sets the RTC to the date & time this sketch was compiled
    // rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
  }
    rtc.adjust(DateTime(2025, 9, 23, 12, 31, 0));

  layout();
  ajustesDisplay();

}

void loop(){
  RTC();
  temperaturaNTC();

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
        tft.setCursor(150, 110);
        tft.setTextColor(ST77XX_GREEN, ST77XX_WHITE);
        tft.print("ON ");
      } else {
        digitalWrite(luzQuarto1, LOW);
        tft.setCursor(150, 110);
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
        tft.setCursor(150, 130);
        tft.setTextColor(ST77XX_GREEN, ST77XX_WHITE);
        tft.print("ON ");
      } else {
        digitalWrite(luzQuarto2, LOW);
        tft.setCursor(150, 130);
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
        tft.setCursor(150, 150);
        tft.setTextColor(ST77XX_GREEN, ST77XX_WHITE);
        tft.print("ON ");
      } else {
        digitalWrite(luzQuarto3, LOW);
        tft.setCursor(150, 150);
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
        tft.setCursor(150, 170);
        tft.setTextColor(ST77XX_GREEN, ST77XX_WHITE);
        tft.print("ON ");
      } else {
        digitalWrite(luzBanheiro, LOW);
        tft.setCursor(150, 170);
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
        tft.setCursor(150, 190);
        tft.setTextColor(ST77XX_GREEN, ST77XX_WHITE);
        tft.print("ON ");
      } else {
        digitalWrite(luzCozinha, LOW);
        tft.setCursor(150, 190);
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
        tft.setCursor(150, 210);
        tft.setTextColor(ST77XX_GREEN, ST77XX_WHITE);
        tft.print("ON ");
      } else {
        digitalWrite(luzSala, LOW);
        tft.setCursor(150, 210);
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
        tft.setCursor(150, 230);
        tft.setTextColor(ST77XX_GREEN, ST77XX_WHITE);
        tft.print("ON ");
      } else {
        digitalWrite(luzGaragem, LOW);
        tft.setCursor(150, 230);
        tft.setTextColor(ST77XX_RED, ST77XX_WHITE);
        tft.print("OFF");
      }
    break;

  }

}