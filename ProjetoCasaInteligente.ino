/*
     AUTOR:   Luan Vinicius dos Santos Morais
     DATA:    27/08/2025
     SKETCH:  CasaInteligente  
*/

//   Inclusão de Biliotecas
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7789.h> // Hardware-specific library for ST7789
#include <SPI.h>

//   Definições de pinos
#define TFT_CS   21
#define TFT_DC   22
#define TFT_MOSI 23 
#define TFT_SCLK 18  
#define TFT_RST  -1

#define botaoProximo 39
#define botaoVoltar 36

// Criando objetos
Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);

//Criação de Variáveis
int selecionarMenu = 0;
int ultimoMenu = 0;

void setup(){
     pinMode(botaoProximo, INPUT_PULLUP);
     pinMode(botaoVoltar, INPUT_PULLUP);
     
     Serial.begin(9600);

     inicarDisplay();
}

void loop(){
     if(digitalRead(botaoProximo) == LOW){
          if(selecionarMenu == 3){
               selecionarMenu = 3;
          } else selecionarMenu++;
     } else if(digitalRead(botaoVoltar) == LOW){
          if(selecionarMenu == 1){
               selecionarMenu = 1;
          } else selecionarMenu--;
     }

     selecionarMenu = Serial.parseInt();
     
     if(selecionarMenu != ultimoMenu){
          switch(selecionarMenu) {
               case 1:
                    escreverTexto("INTERRUPTORES", 10, 10, ST77XX_WHITE, ST77XX_BLACK, 2);
               break;

               case 2:
                    escreverTexto("SENSORES", 10, 10, ST77XX_WHITE, ST77XX_BLACK, 2);
               break;

               case 3:
                    escreverTexto("ATUADORES", 10, 10, ST77XX_WHITE, ST77XX_BLACK, 2);
               break;
               
               default:
               // statements
               break;
          }
          ultimoMenu = selecionarMenu;
     }

}

// Funções

void inicarDisplay(){
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
}

// Função para escrever um texto "limpando" antes
void escreverTexto(const char* texto, int x, int y, uint16_t corTexto, uint16_t corFundo, int tamanho) {
    int larguraChar = 6 * tamanho;   // largura aproximada de cada caractere (fonte padrão do Adafruit_GFX)
    int alturaChar  = 8 * tamanho;   // altura aproximada de cada caractere
    int larguraTexto = strlen(texto) * larguraChar;

    // Apaga a área onde o texto vai ficar
    tft.fillRect(x, y, larguraTexto, alturaChar, corFundo);

    // Configura cor, tamanho e escreve
    tft.setTextSize(tamanho);
    tft.setTextColor(corTexto);
    tft.setCursor(x, y);
    tft.print(texto);
}
