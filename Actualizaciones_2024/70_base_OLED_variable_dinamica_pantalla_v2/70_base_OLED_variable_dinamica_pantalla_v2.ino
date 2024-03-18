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
    Serial.println(POSICION);	
    ANTERIOR = POSICION ;
  }
  Serial.println(POSICION);
  //Serial.println(freeMemory());


  sensorValue = 51;

  u8g2.firstPage();
  do {

    u8g2.setFont(u8g2_font_ncenB10_tr);
    u8g2.drawStr(0, 24, " Jaime el crack");

    u8g2.setFont(u8g2_font_ncenB14_tr);
    u8g2.setCursor(0,40);
    u8g2.print(F("Suscribete!"));

    u8g2.setCursor(0,60);
    u8g2.print(POSICION);


  } while(u8g2.nextPage() );



}


void encoder()  {
  static unsigned long ultimaInterrupcion = 0;
						// tiempo de interrupcion
  unsigned long tiempoInterrupcion = millis();

  if (tiempoInterrupcion - ultimaInterrupcion > 5) {
				
    if (digitalRead(DT) == HIGH)
    {
      POSICION++ ;

      
    }
    else {				
      POSICION-- ;			

      
    }

    POSICION = min(100, max(0, POSICION));
    ultimaInterrupcion = tiempoInterrupcion;
  }						
}