#include "U8g2lib.h"
#define CLK 2 //A
#define DT 3 // b
#define SW 5
#include <MemoryFree.h>

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0); 

int sensorValue = 5;

volatile int POSICION = 1;
int ANTERIOR = 1;	
int lastStateCLK;

//************

bool buttonPressed = false;
unsigned long lastButtonPress = 0;

int item_selected = 0; 
int item_selected_anterior = 3; 

int current_screen = 0; 

const int NUM_ITEMS = 4;

bool cuartaPantallaActive = false;

//************

void setup() {
  Serial.begin(9600);
  u8g2.begin();

  pinMode(CLK,INPUT);
  pinMode(DT,INPUT);
  pinMode(SW, INPUT_PULLUP);
  lastStateCLK = digitalRead(CLK);
  attachInterrupt(digitalPinToInterrupt(CLK), encoder, LOW);
}

void loop() {
  

  if (POSICION != ANTERIOR) {	
    ANTERIOR = POSICION ;
  }

  sensorValue = 51;

  checkEncoderButton();

  u8g2.firstPage();
  do {

    if (current_screen == 0) 
    { 
      Serial.println("current_screen  = 0 ");

    } else  if (current_screen == 1)
    { 

      Serial.println("current_screen  = 1 ");
      Serial.print("item_selected  =");
      Serial.println(item_selected );

        switch(item_selected) {

          case 0: 
              primeraPantalla();
              break;
          case 1:
              segundaPantalla();
              break;
          case 2:
              terceraPantalla();
              break;
          case 3: 
              cuartaPantalla();
              break;
          default:
            break;
        }
    } 
    else if (current_screen == 2) 
    { 
      Serial.println("current_screen  = 2 ");
    }  




  } while(u8g2.nextPage() );



}


void encoder()  {
  static unsigned long ultimaInterrupcion = 0;
						// tiempo de interrupcion
  unsigned long tiempoInterrupcion = millis();

  if (tiempoInterrupcion - ultimaInterrupcion > 5) {
			
    if (!cuartaPantallaActive) {

        if (digitalRead(DT) == HIGH)
        {
          POSICION++ ;
          item_selected++;
          if (item_selected >= NUM_ITEMS) {
              item_selected = 0;
          }
          
        }
        else {				
          POSICION-- ;	
          item_selected--;		
          if (item_selected < 0) {
              item_selected = NUM_ITEMS - 1;
          }
          
        }

    }else{

      if (digitalRead(DT) == HIGH) {
            POSICION++;
      }else {
            POSICION--;
      }
      POSICION = min(100, max(0, POSICION));
      item_selected = 3;
    }

    
    ultimaInterrupcion = tiempoInterrupcion;
  }						
}

//**************************************************************
//             FUNCIÓN PULSADOR ENCODER
//**************************************************************

void checkEncoderButton() {
    int btnState = digitalRead(SW);

    if (btnState == LOW) {
        if (!buttonPressed && (millis() - lastButtonPress > 50)) {
            buttonPressed = true;

                 switch (current_screen) {
                    case 0:
                        current_screen = 1;
                        break;
                    case 1:
                        current_screen = 2;
                        cuartaPantallaActive = false;
                        break;
                    case 2:
                        current_screen = 0;
                        break;
                        
                    default:
                        break;
                }
        }
    } else {
        buttonPressed = false;
    }

}

//******************

void primeraPantalla() {

    u8g2.setFont(u8g2_font_ncenB10_tr);
    u8g2.drawStr(0, 24, " primeraPantalla");
    u8g2.setFont(u8g2_font_ncenB14_tr);
    u8g2.setCursor(0,40);
    u8g2.print(F("Suscribete!"));

}


void segundaPantalla() {

    u8g2.setFont(u8g2_font_ncenB10_tr);
    u8g2.drawStr(0, 24, " segundaPantalla");
    u8g2.setFont(u8g2_font_ncenB14_tr);
    u8g2.setCursor(0,40);
    u8g2.print(F("Suscribete!"));

}
void terceraPantalla() {

    u8g2.setFont(u8g2_font_ncenB10_tr);
    u8g2.drawStr(0, 24, " terceraPantalla");
    u8g2.setFont(u8g2_font_ncenB14_tr);
    u8g2.setCursor(0,40);
    u8g2.print(F("Suscribete!"));

}


void cuartaPantalla() {

  cuartaPantallaActive = true;
      u8g2.setFont(u8g2_font_ncenB10_tr);
      u8g2.drawStr(0, 24, " Jaime el crack");

      u8g2.setFont(u8g2_font_ncenB14_tr);
      u8g2.setCursor(0,40);
      u8g2.print(F("Suscribete!"));

      u8g2.setCursor(0,60);
      u8g2.print(POSICION);

}
