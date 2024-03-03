#include <RTClib.h>		
RTC_DS3231 rtc;			
bool evento_inicio = true;	
bool evento_fin = true;		
# define RELE 24			
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <LiquidMenu.h>

char bufferHora[9]; 
char bufferHora2[13];
int tempBufferHora2 = 0;
const byte analogPin = A1;
unsigned short valorHora = 0;
LiquidCrystal_I2C lcd(0x27, 20, 4);
RTC_DS3231 modulo_rtc;
//ENCODER
#define outputA 6
#define outputB 7
#define sw 4
int aState;
int aLastState; 
//LEDS
#define led1 8
#define led2 9
#define led3 10

int contador = 0;
int led_seleccionado = 0;

LiquidLine linea1(1, 1, "Configurar Riego");
LiquidLine linea2(1, 2, "Led 2");
LiquidLine linea3(1, 3, "Todos");
LiquidLine linea4(1, 0, "Ver Hora y temp");

LiquidScreen pantalla1(linea1, linea2, linea3, linea4);

LiquidLine linea1_2(1, 0, "ON");
LiquidLine linea2_2(1, 1, "OFF");
LiquidLine linea3_2(1, 2, "Atras");

LiquidScreen pantalla2(linea1_2, linea2_2, linea3_2);

LiquidLine linea1_5_2(1, 0, "Hora: ", bufferHora);
LiquidLine linea2_5_2(1, 1, "P ", bufferHora);
LiquidLine linea3_5_2(1, 2, "Atras");
LiquidScreen pantalla3( linea1_5_2,linea2_5_2,linea3_5_2);


// LA PANTALLA IMPORTANTE #4

LiquidLine linea1_6_2(1, 0, "Modifica los min:");

LiquidLine linea2_6_2(1, 2, "0");

LiquidLine linea3_6_2(1, 2, "Atras");

LiquidScreen pantalla4( linea1_6_2,linea2_6_2,linea3_6_2);


  

LiquidMenu menu(lcd, pantalla1, pantalla2, pantalla3, pantalla4);

  

char DiasDeLaSemana[7][12] = {"Domingo", "Lunes", "Martes", "Miercoles", "Jueves", "Viernes", "Sabado"};

// void customFocusCallback(bool focus) {
//   if (focus) {
//     // Incrementa la variable como ejemplo
//     tempBufferHora2++;

//     // Actualiza la variable vinculada a la línea
//     // (esto se hará automáticamente debido a la asociación)
//   }
// }
void customFocusCallback(bool focus) {
  if (focus) {
    // Cambia la posición de enfoque a la posición personalizada
    //linea2_6_2.set_focusPosition(Position::CUSTOM, 3, 1);

    // Incrementa la variable como ejemplo
    Serial.println("Estas en focus");	
    tempBufferHora2++;
  }
}
void fn_alarma() {

}


void setup() {

            Serial.begin(9600);

	
            pinMode(RELE, OUTPUT);		

            if (! rtc.begin()) {				
              Serial.println("Modulo RTC no encontrado !");	
              while (1);					
            }

  

            pinMode(analogPin, INPUT);

              

            Serial.begin(9600);

              

            pinMode(led1, OUTPUT);

            pinMode(led2, OUTPUT);

            pinMode(led3, OUTPUT);

            pinMode(sw, INPUT_PULLUP);

            lcd.init();
            
            lcd.begin(20, 4);

            lcd.backlight();

              

            menu.init();

              

            linea1.set_focusPosition(Position::LEFT);
            linea2.set_focusPosition(Position::LEFT);
            linea3.set_focusPosition(Position::LEFT);
            linea4.set_focusPosition(Position::LEFT);


            linea1.attach_function(1, fn_modificar_minutos);
            linea2.attach_function(1, fn_led2);
            linea3.attach_function(1, fn_todos); 
            linea4.attach_function(1, fn_ver_hora_temperatura);
            
            menu.add_screen(pantalla1);




            linea1_2.set_focusPosition(Position::LEFT);
            linea2_2.set_focusPosition(Position::LEFT);
            linea3_2.set_focusPosition(Position::LEFT);
            linea1_2.attach_function(1, fn_on);
            linea2_2.attach_function(1, fn_off);
            linea3_2.attach_function(1, fn_atras);

            menu.add_screen(pantalla2);





            linea1_5_2.add_variable(bufferHora);
            linea1_5_2.set_focusPosition(Position::LEFT);
            linea2_5_2.add_variable(bufferHora);
            linea2_5_2.set_focusPosition(Position::LEFT);
            linea3_5_2.set_focusPosition(Position::LEFT);
            linea1_5_2.attach_function(1, fn_vacio);
            linea2_5_2.attach_function(1, fn_vacio);
            linea3_5_2.attach_function(1, fn_atras);

            menu.add_screen(pantalla3);



            //-------- LA PANTALLA IMPORTANTE #4
              

            
            linea1_6_2.set_focusPosition(Position::LEFT);

            //linea2_6_2.add_variable(tempBufferHora2);
            //linea2_6_2.set_focusPosition(Position::CUSTOM);
            linea2_6_2.set_focusPosition(Position::LEFT);
            //linea2_6_2.set_focusPosition(Position::CUSTOM,6);


            linea3_6_2.set_focusPosition(Position::LEFT);


            linea1_6_2.attach_function(1, fn_vacio);

            linea2_6_2.attach_function(1, customFocusCallback);

            linea3_6_2.attach_function(1, fn_atras);

            menu.add_screen(pantalla4);

              



                

              pantalla1.set_displayLineCount(4);

              pantalla2.set_displayLineCount(4);

              pantalla3.set_displayLineCount(4);

              pantalla4.set_displayLineCount(4);

                

              menu.set_focusedLine(0);

                

              menu.update();

                

              if( !modulo_rtc.begin() )

              {

                    Serial.println("No se encontró ningun RTC");

                    Serial.flush();

                    abort();

              }

            

  

}

  

void loop() {

 

 valorHora = analogRead(analogPin);

 selectOption();

  

 aState = digitalRead(outputA);

 if (aState != aLastState){ 

    if (digitalRead(outputB) != aState) {

    menu.switch_focus(false);

    } else {

    menu.switch_focus(true);

    }

    menu.update();

    aLastState = aState;

  }

  snprintf(bufferHora2, sizeof(bufferHora2), "%02d", tempBufferHora2);


// Imprimiendo el valor de tempBufferHora2 en el Monitor Serie
  Serial.print("Valor actual de tempBufferHora2: ");
  Serial.println(tempBufferHora2);
}

  

void selectOption() {

 if (digitalRead(sw) == LOW) {

 menu.call_function(1);

 delay(500);

 }

}

  

void fn_modificar_minutos() {

 menu.change_screen(4);

 menu.set_focusedLine(0);

}

  

void fn_led2() {

 menu.change_screen(2);

 menu.set_focusedLine(0);

 led_seleccionado = 2;

}

  

void fn_led3() {

 menu.change_screen(2);

 menu.set_focusedLine(0);

 led_seleccionado = 3;

}

  

void fn_led3_modificada() {

 menu.change_screen(2);

 menu.set_focusedLine(0);

 led_seleccionado = 3;

}

  

void fn_todos() {

 menu.change_screen(2);

 menu.set_focusedLine(0);

 led_seleccionado = 0;

}

  
  

void fn_ver_hora_temperatura() {

 DateTime ahora = modulo_rtc.now();
 sprintf(bufferHora, "%02d:%02d", ahora.hour(), ahora.minute());

// Cambia a la pantalla que muestra la hora y la temperatura.
 menu.change_screen(3);
 menu.set_focusedLine(0);

// Refresca el menú para mostrar los cambios.
 menu.update();

}

  
  
  

void fn_on() {

 switch(led_seleccionado){

 case 1:

 digitalWrite(led1, HIGH);

 break;

 case 2:

 digitalWrite(led2, HIGH);

 break;

 case 3:

 digitalWrite(led3, HIGH);

 break;

 case 0:

 digitalWrite(led1, HIGH);

 digitalWrite(led2, HIGH);

 digitalWrite(led3, HIGH);

 break; 

 }

}

  

void fn_off() {

 switch(led_seleccionado){

 case 1:

 digitalWrite(led1, LOW);

 break;

 case 2:

 digitalWrite(led2, LOW);

 break;

 case 3:

 digitalWrite(led3, LOW);

 break;

 case 0:

 digitalWrite(led1, LOW);

 digitalWrite(led2, LOW);

 digitalWrite(led3, LOW);

   break; 

 }

}

  

void fn_atras() {

 menu.change_screen(1);

 menu.set_focusedLine(0);

}

  

void fn_vacio(){

}

