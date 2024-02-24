#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <LiquidMenu.h>

#include <RTClib.h>

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

int led_seleccionado = 0;

LiquidLine linea1(1, 0, "Led 1 Jaime"); // LA POSICIÓN CERO ES PARA LA FLECHA 
LiquidLine linea2(1, 1, "Led 2");
// LiquidLine linea3(1, 2, "Led 3");
LiquidLine linea4(1, 2, "Todos");
LiquidLine linea5(1, 3, "Led 3_modificado");
LiquidScreen pantalla1(linea1, linea2, linea4, linea5);

LiquidLine linea1_2(1, 0, "ON");
LiquidLine linea2_2(1, 1, "OFF");
LiquidLine linea3_2(1, 2, "Atras");
LiquidScreen pantalla2(linea1_2, linea2_2, linea3_2);

LiquidMenu menu(lcd, pantalla1, pantalla2);

char DiasDeLaSemana[7][12] = {"Domingo", "Lunes", "Martes", "Miercoles", "Jueves", "Viernes", "Sabado"};

void setup() {

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
  // linea3.set_focusPosition(Position::LEFT); 
  linea4.set_focusPosition(Position::LEFT); 
  linea5.set_focusPosition(Position::LEFT); 
   
  linea1.attach_function(1, fn_led1); 
  linea2.attach_function(1, fn_led2);
  // linea3.attach_function(1, fn_led3);
  linea4.attach_function(1, fn_todos);
  // linea5.attach_function(1, fn_led3_modificada);
  linea5.attach_function(1, fn_ver_hora);
  
  menu.add_screen(pantalla1);
  
  linea1_2.set_focusPosition(Position::LEFT); 
  linea2_2.set_focusPosition(Position::LEFT); 
  linea3_2.set_focusPosition(Position::LEFT); 
  
  linea1_2.attach_function(1, fn_on); 
  linea2_2.attach_function(1, fn_off);
  linea3_2.attach_function(1, fn_atras);
   
  menu.add_screen(pantalla2);

  pantalla1.set_displayLineCount(4);
  pantalla2.set_displayLineCount(4);

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

void fn_ver_hora() {
  DateTime ahora = modulo_rtc.now();
  
  // Display the current time
  lcd.clear();
  lcd.print(ahora.hour());
  lcd.print(':');
  lcd.print(ahora.minute());
  lcd.print(':');
  lcd.print(ahora.second());

  // Display the temperature
  lcd.setCursor(0, 1);
  lcd.print("Temp: ");
  lcd.print(modulo_rtc.getTemperature());
  lcd.print(" *C");

  // You can add additional logic or delay as needed
  delay(5000);  // Display for 5 seconds (adjust as needed)

  // Switch back to the main menu
  menu.change_screen(1);
  menu.set_focusedLine(0);
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
