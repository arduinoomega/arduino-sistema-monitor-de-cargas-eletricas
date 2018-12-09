#include <LiquidCrystal.h> //Biblioteca para o display LCD
#include "EmonLib.h" //Biblioteca para o sensor de corrente
#include <SPI.h> //Biblioteca SPI
EnergyMonitor emon1; //cria objeto para sensor de corrente
#define botao 10 // define o pino 10 com o nome botao
#define botaoB 11 // define o pino 11 com o nome botaoB
#define pin_sct A0 //sensor de corrente
#define menu 8 // define o pino 8 com o nome menu
#define tempoMaximoDeUmToque  200 // define o tempo maximo de um toque no botao de 200 milisegundos
int rede; // cria uma variavel int de nome rede
int contaToques = 0; //cria a variavel contaToques int
long tempoEspera = 0; //cria a variavel tempoEspera long

LiquidCrystal lcd(7,6,5,4,3,2); // pinos para a comunicaçao do lcd

void setup()
{
  pinMode(botao  , INPUT); // define o pino botao como entrada digital
  pinMode(botaoB  , INPUT); // define o pino botaoB como entrada digital
  pinMode(pin_sct, INPUT); //entrada para sensor de corrente
  pinMode(menu, INPUT); // define o pino menu como entrada digital
  lcd.begin(16,2); //Inicializa LCD 16 x 2
  emon1.current(pin_sct, 60); //Pino, calibracao - Cur Const= Ratio/BurdenR. 2000/33 = 60

  // apresentacao inicial no display
  lcd.setCursor(5,0); lcd.print("Energy ");
  lcd.setCursor(0,1); lcd.print(">"); delay(125); lcd.setCursor(1,1); lcd.print(">"); delay(125);
  lcd.setCursor(2,1); lcd.print(">"); delay(125); lcd.setCursor(3,1); lcd.print(">"); delay(125);
  lcd.setCursor(4,1); lcd.print(">"); delay(125); lcd.setCursor(5,1); lcd.print(">"); delay(125);
  lcd.setCursor(6,1); lcd.print(">"); delay(125); lcd.setCursor(7,1); lcd.print(">"); delay(125);
  lcd.setCursor(8,1); lcd.print(">"); delay(125); lcd.setCursor(9,1); lcd.print(">"); delay(125);
  lcd.setCursor(10,1); lcd.print(">"); delay(125); lcd.setCursor(11,1); lcd.print(">"); delay(125);
  lcd.setCursor(12,1); lcd.print(">"); delay(125); lcd.setCursor(13,1); lcd.print(">"); delay(125);
  lcd.setCursor(14,1); lcd.print(">"); delay(125); lcd.setCursor(15,1); lcd.print(">"); delay(125);lcd.clear();
  lcd.setCursor(1,0); lcd.print("Voltage Select");
  lcd.setCursor(0,1); lcd.print(" "); lcd.setCursor(1,1); lcd.print("127V"); lcd.setCursor(5,1); lcd.print("           "); delay(700);
  lcd.setCursor(6,1); lcd.print("220V"); lcd.setCursor(0,1); lcd.print("      "); lcd.setCursor(10,1); lcd.print("       "); delay(700);
  lcd.setCursor(11,1); lcd.print("380V"); lcd.setCursor(0,1); lcd.print("           "); lcd.setCursor(15,1); lcd.print(" "); delay(700);
  lcd.setCursor(0,1); lcd.print("               ");
  // apresentacao inicial no display
} 
void loop(){

//MENU************************************************
int botao_menu = digitalRead(botao); // variavel de nome botao_menu que armazena dados digitais da leitura do botao
int botaoB_menu = digitalRead(botaoB); // variavel de nome botaoB_menu que armazena dados digitais da leitura do botao B

 if (botao_menu == HIGH) { //detectou toque no botao, executa o codigo abaixo

     //espera um tempo para nao detectar o mesmo toque no botao mais de uma vez 
     if (tempoEspera == 0) {
        tempoEspera = millis(); 
        contaToques++;} 
     else if ((millis() - tempoEspera) >= tempoMaximoDeUmToque) {
        tempoEspera = 0;}
  }

  if (contaToques == 0 ){ // se nao houver toques no botao apresenta a tensao de 127V
    lcd.setCursor(0,1); lcd.print(" ");
    lcd.setCursor(1,1); lcd.print("127V");
    lcd.setCursor(5,1); lcd.print("           ");
    if(botaoB_menu == HIGH){ // se durante 127V pressionar o botao B, seleciona 127V
      digitalWrite(menu,HIGH);
      rede = 127;
      lcd.clear();
    }
  }
  if (contaToques == 1 ){ // se houver 1 toque no botao apresenta a tensao de 220V
    lcd.setCursor(6,1); lcd.print("220V");
    lcd.setCursor(0,1); lcd.print("      ");
    lcd.setCursor(10,1); lcd.print("       ");
    if(botaoB_menu == HIGH){ // se durante 220V pressionar o botao B, seleciona 220V
      digitalWrite(menu,HIGH);
      rede = 220;
      lcd.clear();
    }
  }
  if (contaToques == 2 ){ // se houverem 2 toque no botao apresenta a tensao de 380V
    lcd.setCursor(11,1); lcd.print("380V");
    lcd.setCursor(0,1); lcd.print("           ");
    lcd.setCursor(15,1); lcd.print(" ");
    if(botaoB_menu == HIGH){ // se durante 380V pressionar o botao B, seleciona 380V
      digitalWrite(menu,HIGH);
      rede = 380;
      lcd.clear();
    }
  }
  if (contaToques == 3 ){ // se houverem 3 toque no botao, zera o contador (inicia novamente) e limpa o display
     contaToques = 0;
     lcd.setCursor(0,1); lcd.print("     ");
  }
  
//FIM MENU************************************************


int menu_select = digitalRead(menu);
while(menu_select == HIGH){ // após selecionar a tensao desejada, o programa roda dentro do laço while
int menu_select = digitalRead(menu);
  

  //Calculo da corrente
  double Irms = emon1.calcIrms(1480);
  double corrente = Irms;
  if(corrente < 0.15){
  corrente = Irms*0;
  lcd.setCursor(13,1); lcd.print(" ");
  lcd.setCursor(13,0); lcd.print(" ");}
  //Calculo da potencia
  float potencia = corrente*rede;

  lcd.setCursor(0,0); lcd.print("Energy "); //imprime valores de tensão, corrente e potencia
  lcd.setCursor(3,1); lcd.print("V");
  lcd.setCursor(7,0); lcd.print("I=");
  lcd.setCursor(7,1); lcd.print("P=");
  lcd.setCursor(14,1); lcd.print("VA"); 
  lcd.setCursor(14,0); lcd.print("A"); 
  lcd.setCursor(9,0); lcd.print(corrente);
  lcd.setCursor(9,1); lcd.print(potencia);
  lcd.setCursor(0,1); lcd.print(rede);
  delay(10);  
}

}
