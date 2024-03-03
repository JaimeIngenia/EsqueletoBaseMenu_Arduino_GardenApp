//Estamos en 1da_humedad
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <LiquidMenu.h>
//RTC_Relay Changes
#include <RTClib.h>	
# define RELE 24	
RTC_DS3231 rtc;	
RTC_DS3231 modulo_rtc;
const byte analogPin = A1;
unsigned short valorHora = 0;
char bufferHora[9]; 
float temperatura = 0.0; 
bool evento_inicio = true;	// variable de control para inicio de evento con valor true
bool evento_fin = true;		// variable de control para finalizacion de evento con valor true
DateTime fecha_Alarma_Inicio_Riego;
DateTime fecha_Alarma_Fin_Riego;
int hora_alarma=0;
int minuto_alarma=0;
int segundo_alarma=0;

//LCD
LiquidCrystal_I2C lcd(0x27, 20, 4);
//ENCODER
#define outputA 6
#define outputB 7
#define sw 4
int aState;
int aLastState;  
int led_seleccionado = 10;
int variableMostrarPantalla = 10; // 10 segundo
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
LiquidLine linea2(1, 1, "1) Ver Hora y temp"); 
LiquidLine linea3(1, 2, "2) Conf sensor R"); 
LiquidScreen pantalla1(linea1,linea2,linea3);

LiquidLine linea1_3(1, 0, "Hora: ",bufferHora);
LiquidLine linea2_3(1, 1, "Temp: ",temperatura);
LiquidLine linea3_3(1, 2, "Atras");
LiquidScreen pantalla3( linea1_3,linea2_3,linea3_3);

LiquidLine linea1_2(1, 0, "Modifica los min:");
LiquidLine linea2_2(1, 1, "Minutos:  ",variableMostrarPantalla);
LiquidLine linea3_2(1, 2, "Atras");

LiquidScreen pantalla2(linea1_2,linea2_2,linea3_2);



LiquidMenu menu(lcd,pantalla1,pantalla2,pantalla3);

void setup() {

  Serial.begin(9600);
  Serial.println("Bienvenido");	
  pinMode(sw,INPUT_PULLUP);
  lcd.init();
  lcd.backlight();
  menu.init();

  pinMode(RELE, OUTPUT);		// pin 3 como salida

   if (! rtc.begin()) {				// si falla la inicializacion del modulo
    Serial.println("Modulo RTC no encontrado !");	// muestra mensaje de error
    while (1);					// bucle infinito que detiene ejecucion del programa
  }

  linea1.set_focusPosition(Position::LEFT); 
  linea2.set_focusPosition(Position::LEFT);
  linea3.set_focusPosition(Position::LEFT);

  linea1.attach_function(1, fn_vacio); 
  linea2.attach_function(1, fn_Hora_temperatura_RTC); 
  linea3.attach_function(1, fn_sensor_riego); 

  menu.add_screen(pantalla1);

  linea1_3.set_focusPosition(Position::LEFT); 
  linea2_3.set_focusPosition(Position::LEFT); 
  linea3_3.set_focusPosition(Position::LEFT); 

  linea1_3.attach_function(1, fn_vacio); 
  linea2_3.attach_function(1, fn_vacio);
  linea3_3.attach_function(1, fn_atras);

  menu.add_screen(pantalla3);

  linea1_2.set_focusPosition(Position::LEFT); 
  linea2_2.set_focusPosition(Position::LEFT); 
  linea3_2.set_focusPosition(Position::LEFT); 

  linea1_2.attach_function(1, fn_on); 
  linea2_2.attach_function(1, fn_variable);
  linea3_2.attach_function(1, fn_atras);

  menu.add_screen(pantalla2);



  pantalla1.set_displayLineCount(4);
  pantalla2.set_displayLineCount(4);
  pantalla3.set_displayLineCount(4);

  menu.set_focusedLine(1);
  menu.update();

}

void loop() {

   DateTime fecha_Alarma_Inicio_Riego = rtc.now();	

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

        // Actualizar la alarma basándose en la hora de inicio (6 AM) y los segundos seleccionados

  DateTime alarmaFin = calcularAlarmaRiego(fecha_Alarma_Inicio_Riego,variableMostrarPantalla);
  // Serial.println("-------------------------------------------------------------------");
  
  // Serial.print("Año: "); Serial.print(fecha_Alarma_Inicio_Riego.year(), DEC);
  // Serial.print(" Mes: "); Serial.print(fecha_Alarma_Inicio_Riego.month(), DEC);
  // Serial.print(" Día: "); Serial.print(fecha_Alarma_Inicio_Riego.day(), DEC);
  // Serial.print(" Hora: "); Serial.print(fecha_Alarma_Inicio_Riego.hour(), DEC);
  // Serial.print(" Minuto: "); Serial.print(fecha_Alarma_Inicio_Riego.minute(), DEC);
  // Serial.print(" Segundo: "); Serial.print(fecha_Alarma_Inicio_Riego.second(), DEC);
  
  // Serial.println("\n-------------------------------------------------------------------");

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
                  // Incrementar y limitar al rango de 0 a 360 segundos es decir 5 minutos
                  variableMostrarPantalla = (variableMostrarPantalla + 1) % 301;
              } else {
                  // Decrementar y limitar al rango de 0 a 360 segundos es decir 5 minutos
                  variableMostrarPantalla = (variableMostrarPantalla - 1 + 301) % 301;
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

// Función para actualizar la alarma dependiendo de los segundos ingresados, ya que todos los dìas comienza a las 6am y termina dependiendo de los segundos que ingresen para apagar el relay
DateTime calcularAlarmaRiego(DateTime fecha_Alarma_Inicio_Riego,int segundos) {
    

  if ( fecha_Alarma_Inicio_Riego.hour() == 18 && fecha_Alarma_Inicio_Riego.minute() == 30  && fecha_Alarma_Inicio_Riego.second() == 15)
  {
    if ( evento_inicio == true ){		
      digitalWrite(RELE, HIGH);			
      Serial.println( "Rele encendido" );		
      evento_inicio = false;		

      Serial.println("-------------------------------------------------------------------");
      Serial.print(" fecha_Alarma_Inicio_Riego: ");
      Serial.print(" Hora: "); Serial.print(fecha_Alarma_Inicio_Riego.hour(), DEC);
      Serial.print(" Minuto: "); Serial.print(fecha_Alarma_Inicio_Riego.minute(), DEC);
      Serial.print(" Segundo: "); Serial.print(fecha_Alarma_Inicio_Riego.second(), DEC);
      
      Serial.println("\n-------------------------------------------------------------------");

      DateTime fecha_Alarma_Fin_Riego = fecha_Alarma_Inicio_Riego + TimeSpan(segundos);
      hora_alarma=fecha_Alarma_Fin_Riego.hour();
      minuto_alarma=fecha_Alarma_Fin_Riego.minute();
      segundo_alarma=fecha_Alarma_Fin_Riego.second();

      Serial.println("-------------------------------------------------------------------");
      Serial.print(" fecha_Alarma_Fin_Riego: ");
      Serial.print(" Hora: "); Serial.print(fecha_Alarma_Fin_Riego.hour(), DEC);
      Serial.print(" Minuto: "); Serial.print(fecha_Alarma_Fin_Riego.minute(), DEC);
      Serial.print(" Segundo: "); Serial.print(fecha_Alarma_Fin_Riego.second(), DEC);
      Serial.print(" Hora_Variable: ");
      Serial.print(hora_alarma);
      Serial.print(" Minuto_Variable: ");
      Serial.print(minuto_alarma);
      Serial.print(" Hora_Variable: ");
      Serial.print(hora_alarma);
       

      
      Serial.println("\n-------------------------------------------------------------------");	
    }	


    

  }

  //DateTime fecha_Alarma_Fin_Riego = fecha_Alarma_Inicio_Riego + TimeSpan(segundos);

  if ( fecha_Alarma_Inicio_Riego.hour() == hora_alarma && 
      fecha_Alarma_Inicio_Riego.minute() == minuto_alarma  && 
      fecha_Alarma_Inicio_Riego.second() == segundo_alarma)
      {	
      if ( evento_fin == true ){				
        digitalWrite(RELE, LOW);				
        Serial.println( "Rele apagado" );			
        evento_fin = false;				
      }							
    }

    return fecha_Alarma_Fin_Riego;
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

void fn_Hora_temperatura_RTC(){

  DateTime ahora = modulo_rtc.now();
  sprintf(bufferHora, "%02d:%02d", ahora.hour(), ahora.minute());
  
  temperatura = modulo_rtc.getTemperature();

  menu.change_screen(3);
  menu.set_focusedLine(2);
  menu.update();
}


