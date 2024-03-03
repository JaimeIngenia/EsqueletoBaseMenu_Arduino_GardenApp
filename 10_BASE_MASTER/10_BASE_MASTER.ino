//Estamos en 1era_RTC_Relay
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
int variableMostrarPantalla = 0;
//estados
int salida=0;
int estado=1;
int estadoAnterior=1;
enum estados {
  EST_ENCENDIDO_EDITAR,
  EST_LOGICA_EDITAR,
  EST_APAGADO_EDITAR,
};
int estado_actual=EST_ENCENDIDO_EDITAR;
int estado_anterior=EST_APAGADO_EDITAR;

LiquidLine linea1(1, 0, "    MAIN MENU    "); 
LiquidLine linea2(1, 1,"1) Conf sensor R"); 
LiquidScreen pantalla1(linea1,linea2);

LiquidLine linea1_2(1, 0, "Modifica los min:");
LiquidLine linea2_2(1, 1, "Minutos:  ");
LiquidLine linea3_2(1, 2, "Atras");

LiquidScreen pantalla2(linea1_2,linea2_2,linea3_2);

LiquidMenu menu(lcd,pantalla1,pantalla2);

void setup() {

  Serial.begin(9600);
  Serial.println("Bienvenido");	
  pinMode(sw,INPUT_PULLUP);
  lcd.init();
  lcd.backlight();
  menu.init();

  linea1.set_focusPosition(Position::LEFT); 
  linea2.set_focusPosition(Position::LEFT);
  linea1.attach_function(1, fn_vacio); 
  linea2.attach_function(1, fn_sensor_riego); 
  menu.add_screen(pantalla1);

  linea1_2.set_focusPosition(Position::LEFT); 
  linea2_2.set_focusPosition(Position::LEFT); 
  linea3_2.set_focusPosition(Position::LEFT); 

  linea1_2.attach_function(1, fn_on); 
  linea2_2.attach_function(1, fn_variable);
  linea2_2.add_variable(variableMostrarPantalla);
  linea3_2.attach_function(1, fn_atras);

  menu.add_screen(pantalla2);

  pantalla1.set_displayLineCount(4);
  pantalla2.set_displayLineCount(4);

  menu.set_focusedLine(1);
  menu.update();

}

void loop() {

  selectOption();
  estado = digitalRead(sw);
   // Debounce
  if (estado != estadoAnterior) {
    delay(60);  
    estado = digitalRead(sw);
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



void fn_sensor_riego(){
  menu.change_screen(2);
  menu.set_focusedLine(1);
  linea2_2.set_focusPosition(Position::CUSTOM, 9, 1);
}

void fn_variable() {

  arrancar();

  while(salida != 0)
  {
    switch(estado_actual){

      case EST_LOGICA_EDITAR:
        funcion_editando();
      break;

      case EST_APAGADO_EDITAR:
        funcion_apagar();
      break;

      default:
      break;

    }
    
  }
}
//*******************************************************
//FUNCIÓN PULSADOR SOSTENIDO START
void arrancar()
{
  if (estado_actual == EST_ENCENDIDO_EDITAR)
  {
    estado = digitalRead(sw);
    delay(1500);

    estado_actual = EST_LOGICA_EDITAR;

    salida = 1;
        
    Serial.println("ESTADO HOOOOOOOO NOOO ");
    Serial.println(estado); 
    Serial.println("estadoAnterior NOOO ");
    Serial.println(estadoAnterior); 

  }  
}

//*******************************************************
//FUNCIÓN PULSADOR SOSTENIDO START
void funcion_editando()
{
    
    if (estado_actual == EST_LOGICA_EDITAR)
    { 
        
        Serial.println("Estas en modo edición y sw = HIGTH");	
          aState = digitalRead(outputA);
          if (aState != aLastState) {     
              if (digitalRead(outputB) != aState) { 
                  // Incrementar y limitar al rango de 0 a 5
                  variableMostrarPantalla = (variableMostrarPantalla + 1) % 6;
              } else {
                  // Decrementar y limitar al rango de 0 a 5
                  variableMostrarPantalla = (variableMostrarPantalla - 1 + 6) % 6;
              }
              menu.update();
              aLastState = aState;
          }
        
        Serial.println("EST_LOGICA_EDITAR");
        Serial.print("Estad: ");
        Serial.println(estado); 

        Serial.println("estadoAnterior: ");
        Serial.println(estadoAnterior); 
        
        estado = digitalRead(sw);
        
        if((estado == LOW)&&(estadoAnterior == HIGH))
        {
            menu.change_screen(1);
            menu.set_focusedLine(0);

            estadoAnterior = estado;
            estado_actual = EST_APAGADO_EDITAR;
            delay(150);
        }
        
    }
}

//*******************************************************
//FUNCIÓN PULSADOR SOSTENIDO START
void funcion_apagar()
{
    if (estado_actual == EST_APAGADO_EDITAR)
    {
        Serial.println("EST_APAGADO_EDITAR apagado apagado ");

        menu.change_screen(2);
        menu.set_focusedLine(2);
        estado_actual = EST_ENCENDIDO_EDITAR;
        salida = 0;
        estado=1;
        estadoAnterior=1;

    }
}


void fn_on(){
}
void fn_off(){
}
void fn_vacio(){
}


void selectOption(){
  if(digitalRead(sw) == LOW){
      menu.call_function(1);
      delay(100);
  }
}

void fn_atras(){
  menu.change_screen(1);
  menu.set_focusedLine(1);
}


