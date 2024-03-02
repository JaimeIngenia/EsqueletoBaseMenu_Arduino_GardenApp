#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <LiquidMenu.h>
LiquidCrystal_I2C lcd(0x27, 20, 4);
//ENCODER
#define outputA 6
#define outputB 7
#define sw 4
int aState;
int aLastState;  
int led_seleccionado = 10;
// Variable para controlar si estás en modo de edición de la variable
bool editingMode = false;
int variableMostrarPantalla = 0;
LiquidLine linea1(1, 0, "Led 1 Jaime"); // LA POSICIÒN CERO ES PARA LA FLECHA 
LiquidScreen pantalla1(linea1);
LiquidLine linea1_2(1, 0, "ON");
LiquidLine linea2_2(1, 1, "OFF");
LiquidLine linea3_2(1, 2, "Atras");
LiquidLine linea4_2(1, 3, "Variable:");
LiquidScreen pantalla2(linea1_2,linea2_2,linea3_2,linea4_2);
LiquidMenu menu(lcd,pantalla1,pantalla2);

void setup() {
  Serial.begin(9600);
  Serial.println("Bienvenido");	
  pinMode(sw,INPUT_PULLUP);
  lcd.init();
  lcd.backlight();
  menu.init();
  linea1.set_focusPosition(Position::LEFT); 
  linea1.attach_function(1, fn_prueba1); 
  menu.add_screen(pantalla1);
  linea1_2.set_focusPosition(Position::LEFT); 
  linea2_2.set_focusPosition(Position::LEFT); 
  linea3_2.set_focusPosition(Position::LEFT); 
  linea4_2.set_focusPosition(Position::LEFT); 
  linea1_2.attach_function(1, fn_on); 
  linea2_2.attach_function(1, fn_off);
  linea3_2.attach_function(1, fn_atras);
  linea4_2.attach_function(1, fn_variable);
  linea4_2.attach_function(2, fn_cambio_variable);
  linea4_2.add_variable(variableMostrarPantalla);
  menu.add_screen(pantalla2);
  pantalla1.set_displayLineCount(4);
  pantalla2.set_displayLineCount(4);
  menu.set_focusedLine(0);
  menu.update();
}
void loop() {
  selectOption();

  if (editingMode) {
    handleVariableEditing();
  } else {
    handleNavigation();
  }

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

void handleNavigation() {
  Serial.println("Estas en modo handleNavigation");	
  // menu.set_focusedLine(3);
  aState = digitalRead(outputA); 
  if (aState != aLastState) {     
    if (digitalRead(outputB) != aState) { 
      menu.switch_focus(false);
    } else {
      menu.switch_focus(true);
    }
    menu.update();
    aLastState = aState;
  }
}

void handleVariableEditing() {

  Serial.println("Estas en modo edición********************************");	
  if(editingMode == true){

        if (digitalRead(sw) == LOW) {
          editingMode = false;
          Serial.println("Estas en modo edición y sw = LOW");	
        } else {
          Serial.println("Estas en modo edición y sw = HIGTH");	
          aState = digitalRead(outputA);
          if (aState != aLastState) {     
            if (digitalRead(outputB) != aState) { 
              variableMostrarPantalla++;
            } else {
              variableMostrarPantalla--;
            }
            menu.update();
            aLastState = aState;
          }
        }



  }else{
    Serial.println("No deberías estar en modo edición********************************-------------------------------------------");	
  }

}
void fn_prueba1(){
  menu.change_screen(2);
  menu.set_focusedLine(0);
  led_seleccionado = 1;
}

void fn_variable() {
  editingMode = true;
  menu.change_screen(2);
  if(editingMode == true){
      linea4_2.set_focusPosition(Position::CUSTOM, 10, 3);
  }else{
      linea4_2.set_focusPosition(Position::LEFT);
  }
}
void fn_cambio_variable() {

}

void fn_on(){
}
void fn_off(){
}

void selectOption(){
  if(digitalRead(sw) == LOW){
      menu.call_function(1);
      delay(500);
  }
}

void fn_atras(){
  menu.change_screen(1);
  menu.set_focusedLine(0);
}
// void fn_variable() {
//   static bool customPosition = false;
  
//   // Si el enfoque está en la posición personalizada (10, 3)
//   if (menu.get_focusedLine() == 3 && customPosition) {
//     // Incrementa la variable mostrada cada vez que se hace clic
//     variableMostrarPantalla++;
//     // Actualiza la pantalla para reflejar el cambio en la variable mostrada
//     menu.update();
//   } else {
//     // Si el enfoque no está en la posición personalizada, cambia a la pantalla que muestra la variable y permite su modificación
//     menu.change_screen(2);
//     // Cambia la línea enfocada a la línea que contiene la variable
//     menu.set_focusedLine(3);
//     // Asigna la posición de enfoque personalizada en la línea4_2
//     customPosition = true;
//   }
// }


