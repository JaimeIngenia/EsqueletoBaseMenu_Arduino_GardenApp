
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

  

int A_estado_actual;

int A_ultimo_estado;

  

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

  

// LiquidLine linea1_5_2(1, 0,"Hora: ",valorHora);

LiquidLine linea1_5_2(1, 0, "Hora: ", bufferHora);

LiquidLine linea2_5_2(1, 1, "P ", bufferHora);

LiquidLine linea3_5_2(1, 2, "Atras");

LiquidScreen pantalla3( linea1_5_2,linea2_5_2,linea3_5_2);




LiquidLine linea1_6_2(1, 0, "Modifica los min:");

LiquidLine linea2_6_2(1, 1, bufferHora2);

LiquidLine linea3_6_2(1, 2, "Atras");

LiquidScreen pantalla4( linea1_6_2,linea2_6_2,linea3_6_2);


  

LiquidMenu menu(lcd, pantalla1, pantalla2, pantalla3, pantalla4);

  

char DiasDeLaSemana[7][12] = {"Domingo", "Lunes", "Martes", "Miercoles", "Jueves", "Viernes", "Sabado"};

  

void setup() {

	
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


            linea1.attach_function(1, fn_led1);

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



            //--------
              

            
            linea1_6_2.set_focusPosition(Position::LEFT);

            linea2_6_2.add_variable(bufferHora2);

            linea2_6_2.set_focusPosition(Position::LEFT);

            linea3_6_2.set_focusPosition(Position::LEFT);


            linea1_6_2.attach_function(1, fn_vacio);

            linea2_6_2.attach_function(1, fn_alarma);

            linea3_6_2.attach_function(1, fn_atras);

            menu.add_screen(pantalla4);

              

// LiquidLine linea1_6_2(1, 0, "Modifica los minutos: ", bufferHora2);

// LiquidLine linea2_6_2(1, 2, "Atras");

// LiquidScreen pantalla4( linea1_6_2,linea2_6_2);

// --------------------

                

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

}

  

void selectOption() {

 if (digitalRead(sw) == LOW) {

 menu.call_function(1);

 delay(500);

 }

}

  

void fn_led1() {

 menu.change_screen(4);

 menu.set_focusedLine(0);

//  led_seleccionado = 1;

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

 // Formatea la hora actual en 'bufferHora'.

//  sprintf(bufferHora, "%02d:%02d:%02d", ahora.hour(), ahora.minute(), ahora.second());
 sprintf(bufferHora, "%02d:%02d", ahora.hour(), ahora.minute());

  

// Cambia a la pantalla que muestra la hora y la temperatura.

 menu.change_screen(3);

 menu.set_focusedLine(0);

  

// Refresca el menú para mostrar los cambios.

 menu.update();

  

// Puedes quitar el 'delay' si quieres que la pantalla no se congele aquí.

// delay(1000);

}

  
  

// void fn_ver_hora_temperatura() {

//   DateTime ahora = modulo_rtc.now();

//   menu.change_screen(3);

//   menu.set_focusedLine(0);

  

//   valorHora = ahora.hour();

//   lcd.clear();

//   lcd.print(ahora.hour());

//   lcd.print(':');

//   lcd.print(ahora.minute());

//   lcd.print(':');

//   lcd.print(ahora.second());

  

//   lcd.setCursor(0, 1);

//   lcd.print("Temp: ");

//   lcd.print(modulo_rtc.getTemperature());

//   lcd.print(" *C");

  

//   delay(1000);  

  

// }

  
  
  
  

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


void fn_alarma(){

  

}









//----------------------------------------------------------------------------------------------------------------------------------------------------------
// #include <Wire.h>		
// #include <RTClib.h>		

// RTC_DS3231 rtc;			

// bool evento_inicio = true;	
// bool evento_fin = true;		

// # define RELE 24			

// void setup () {
//  Serial.begin(9600);		
//  pinMode(RELE, OUTPUT);		

//  if (! rtc.begin()) {				
//  Serial.println("Modulo RTC no encontrado !");	
//  while (1);					
//  }
// }

// void loop () {
//  DateTime fecha = rtc.now();				
							
//  if ( fecha.hour() == 21 && fecha.minute() == 2 ){
//     if ( evento_inicio == true ){			
//       digitalWrite(RELE, HIGH);				
//       Serial.println( "Rele encendido" );		
//       evento_inicio = false;				
//     }							
//   }

//  if ( fecha.hour() == 21 && fecha.minute() == 3 ){	
//     if ( evento_fin == true ){				
//       digitalWrite(RELE, LOW);				
//       Serial.println( "Rele apagado" );			
//       evento_fin = false;				
//     }						
//   }

//  Serial.print(fecha.day());				// funcion que obtiene el dia de la fecha completa
//  Serial.print("/");					// caracter barra como separador
//  Serial.print(fecha.month());				// funcion que obtiene el mes de la fecha completa
//  Serial.print("/");					// caracter barra como separador
//  Serial.print(fecha.year());				// funcion que obtiene el año de la fecha completa
//  Serial.print(" ");					// caracter espacio en blanco como separador
//  Serial.print(fecha.hour());				// funcion que obtiene la hora de la fecha completa
//  Serial.print(":");					// caracter dos puntos como separador
//  Serial.print(fecha.minute());				// funcion que obtiene los minutos de la fecha completa
//  Serial.print(":");					// caracter dos puntos como separador
//  Serial.println(fecha.second());			// funcion que obtiene los segundos de la fecha completa
 
//  delay(1000);						// demora de 1 segundo

//   if ( fecha.hour() == 2 && fecha.minute() == 0 ){ 	// si hora = 2 y minutos = 0 restablece valores de
//     evento_inicio = true;				// variables de control en verdadero
//     evento_fin = true;
//   }
// }