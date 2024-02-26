#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <LiquidMenu.h>

#include <RTClib.h>

//Codigo funcional

char bufferHora[9];

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

LiquidLine linea5(2, 0, "Ver Hora y temp");
LiquidLine linea1(1, 1, "Led 1 Jaime"); 
LiquidLine linea2(1, 2, "Led 2");
LiquidLine linea4(1, 3, "Todos");
LiquidScreen pantalla1(linea1, linea2, linea4, linea5);

LiquidLine linea1_2(1, 0, "ON");
LiquidLine linea2_2(1, 1, "OFF");
LiquidLine linea3_2(1, 2, "Atras");
LiquidScreen pantalla2(linea1_2, linea2_2, linea3_2);

// LiquidLine linea1_5_2(1, 0,"Hora: ",valorHora);
LiquidLine linea1_5_2(1, 0, "Hora: ", bufferHora);
LiquidLine linea3_5_2(1, 2, "Atras");
LiquidScreen pantalla3( linea1_5_2,linea3_5_2);

LiquidMenu menu(lcd, pantalla1, pantalla2, pantalla3);

char DiasDeLaSemana[7][12] = {"Domingo", "Lunes", "Martes", "Miercoles", "Jueves", "Viernes", "Sabado"};

void setup() {

  pinMode(analogPin, INPUT);

  Serial.begin(9600);

  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT); 
  
  pinMode(sw, INPUT_PULLUP);
  
  lcd.init();
  lcd.begin(20, 4);  // Configura las dimensiones de la pantalla (20 columnas, 4 filas)
  lcd.backlight();

  menu.init();

  linea1.set_focusPosition(Position::LEFT); 
  linea2.set_focusPosition(Position::LEFT); 
  linea4.set_focusPosition(Position::LEFT); 
  linea5.set_focusPosition(Position::LEFT); 
   
  linea5.attach_function(1, fn_ver_hora_temperatura);
  linea1.attach_function(1, fn_led1); 
  linea2.attach_function(1, fn_led2);
  linea4.attach_function(1, fn_todos);  
  
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
  linea3_5_2.set_focusPosition(Position::LEFT); 
  
  linea1_5_2.attach_function(1, fn_vacio); 
  linea3_5_2.attach_function(1, fn_atras);
   
  menu.add_screen(pantalla3);

  pantalla1.set_displayLineCount(4);
  pantalla2.set_displayLineCount(4);
  pantalla3.set_displayLineCount(4);

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
  menu.change_screen(2);
  menu.set_focusedLine(0);
  led_seleccionado = 1;
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
  sprintf(bufferHora, "%02d:%02d:%02d", ahora.hour(), ahora.minute(), ahora.second());

  // Cambia a la pantalla que muestra la hora y la temperatura.
  menu.change_screen(3);
  menu.set_focusedLine(0);

  // Refresca el menú para mostrar los cambios.
  menu.update();

  // Puedes quitar el 'delay' si quieres que la pantalla no se congele aquí.
  // delay(1000);
}


// void fn_ver_hora_temperatura() {
//   DateTime ahora = modulo_rtc.now();
//   menu.change_screen(3);
//   menu.set_focusedLine(0);

//   valorHora = ahora.hour();
  
//   lcd.clear();
//   lcd.print(ahora.hour());
//   lcd.print(':');
//   lcd.print(ahora.minute());
//   lcd.print(':');
//   lcd.print(ahora.second());

//   lcd.setCursor(0, 1);
//   lcd.print("Temp: ");
//   lcd.print(modulo_rtc.getTemperature());
//   lcd.print(" *C");

//   delay(1000);  

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
