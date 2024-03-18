#include "U8g2lib.h"
#define CLK 2 //A
#define DT 3 // b
#define SW 5

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0); 

int sensorValue = 5;

void setup() {
  u8g2.begin();
}

void loop() {

  // sensorValue = analogRead(A0);
  sensorValue = 51;

  u8g2.firstPage();
  do {

    u8g2.setFont(u8g2_font_ncenB10_tr);
    u8g2.drawStr(0, 24, " Jaime el crack");

    u8g2.setFont(u8g2_font_ncenB14_tr);
    u8g2.setCursor(0,40);
    u8g2.print(F("Suscribete!"));

    u8g2.setCursor(0,60);
    u8g2.print(sensorValue);


  } while(u8g2.nextPage() );



}
