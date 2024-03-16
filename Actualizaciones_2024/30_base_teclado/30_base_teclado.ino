//Esta no es una librería estándar. Si te da error, la puedes instalar desde el menú TOOLS-->MANAGE LIBRARIES
#include <HID-Project.h>

//Teclas || El número representa el PIN donde está conectada la tecla.
const int b_key  = 2;
const int a_key  = 3;
const int d_key  = 4;
const int c_key  = 5;
const int e_key  = 6;
const int f_key  = 7;
const int g_key  = 8;
const int h_key  = 9;

//Encoder || El número representa los pines usados para conectar el encoder.
#define  OUTPUT_B 15
#define  OUTPUT_A A0
#define  BUTTON A1
#define PIN_5V  A2
#define PIN_GND  A3

//Variables de control
bool aState;
bool aLastState;
bool lastButtonState = 0;
String programa = "windows";


void setup(){
  //TECLAS || NO MODIFICAR
  pinMode(a_key, INPUT_PULLUP);
  digitalWrite(a_key, HIGH);

  pinMode(b_key, INPUT_PULLUP);
  digitalWrite(b_key, HIGH);

  pinMode(c_key, INPUT_PULLUP);
  digitalWrite(c_key, HIGH);

  pinMode(d_key, INPUT_PULLUP);
  digitalWrite(d_key, HIGH);

  pinMode(e_key, INPUT_PULLUP);
  digitalWrite(e_key, HIGH);

  pinMode(f_key, INPUT_PULLUP);
  digitalWrite(f_key, HIGH);

  pinMode(g_key, INPUT_PULLUP);
  digitalWrite(g_key, HIGH);

  pinMode(h_key, INPUT_PULLUP);
  digitalWrite(h_key, HIGH);

  //ENCODER || NO MODIFICAR
  pinMode(OUTPUT_A, INPUT);  
  pinMode(OUTPUT_B, INPUT);  
  pinMode(BUTTON, INPUT_PULLUP);  
  pinMode(PIN_5V, OUTPUT);
  digitalWrite(PIN_5V, HIGH);  
  pinMode(PIN_GND, OUTPUT);
  digitalWrite(PIN_GND, LOW);  
  aLastState = digitalRead(OUTPUT_A);

  Consumer.begin();
  Keyboard.begin();
}

long lastClickTime = 0;

void loop(){

  // SETEAMOS EL PROGRAMA QUE QUEREMOS UTILIZAR CON LOS CUATRO BOTONES DE ARRIBA
  if(digitalRead(a_key)==LOW){
      programa = "photoshop";
  }

  if(digitalRead(b_key)==LOW){      
      programa = "premiere";
  }

  if(digitalRead(c_key)==LOW){      
      programa = "obs";
  }

  if(digitalRead(d_key)==LOW){      
      programa = "windows";
  }

  //FUNCIONAMIENTO DE LAS TECLAS DE ATAJO Y EL ENCODER SI HEMOS SELECCIONADO WINDOWS
  if(programa == "windows"){
    //TECLAS    
    //ABRIMOS EL BLOC DE NOTAS
    if(digitalRead(e_key)==LOW){
      Keyboard.press(KEY_RIGHT_GUI);            
      Keyboard.releaseAll();
      delay(50);
      Keyboard.println("bloc de notas");
      delay(50);
      Keyboard.press(KEY_RETURN);
      Keyboard.releaseAll();
    }

    //ABRIMOS LA CALCULADORA
    if(digitalRead(f_key)==LOW){
      Keyboard.press(KEY_RIGHT_GUI);            
      Keyboard.releaseAll();
      delay(50);
      Keyboard.println("calculadora");
      delay(50);
      Keyboard.press(KEY_RETURN);
      Keyboard.releaseAll();
    }

    //ABRIMOS SPOTIFY
    if(digitalRead(g_key)==LOW){
      Keyboard.press(KEY_RIGHT_GUI);            
      Keyboard.releaseAll();
      delay(50);
      Keyboard.println("spotify");
      delay(50);
      Keyboard.press(KEY_RETURN);
      Keyboard.releaseAll();
    }

    //ABRIMOS FIREFOX
    if(digitalRead(h_key)==LOW){
      Keyboard.press(KEY_RIGHT_GUI);            
      Keyboard.releaseAll();
      delay(50);
      Keyboard.println("firefox");
      delay(50);
      Keyboard.press(KEY_RETURN);
      Keyboard.releaseAll();
    }
    
    //ENCODER
    if (millis() - lastClickTime > 1000){
      aState = digitalRead(OUTPUT_A);
          
      if (aState != aLastState){
        if (digitalRead(OUTPUT_B) != aState){
          //IZQUIERDA || BAJAMOS EL VOLUMEN          
          Consumer.write(MEDIA_VOL_DOWN);
        }else{
          //DERECHA || SUBIMOS EL VOLUMEN          
          Consumer.write(MEDIA_VOL_UP);
        }
        //Keyboard.releaseAll();
        aLastState = aState;
      }
      
      if (digitalRead(BUTTON) == LOW){      
        if (lastButtonState == HIGH){
          //BOTÓN || MUTEAMOS/DESMUTEAMOS EL SONIDO          
          Consumer.write(MEDIA_VOLUME_MUTE);          
          lastClickTime = millis();
        }      
        lastButtonState = LOW;
      } else {      
        lastButtonState = HIGH;
      }
    }
    //FIN DEL ENCODER    
  }

  //FUNCIONAMIENTO DE LAS TECLAS DE ATAJO Y EL ENCODER SI HEMOS SELECCIONADO PHOTOSHOP
  if(programa == "photoshop"){
    //TECLAS    
    //GUARDAR PARA WEB
    if(digitalRead(e_key)==LOW){
      Keyboard.press(KEY_LEFT_ALT);
      Keyboard.press(KEY_LEFT_SHIFT);
      Keyboard.press(KEY_LEFT_CTRL);
      Keyboard.press('s');      
      delay(100);
      Keyboard.releaseAll();      
    }

    //DESHACER
    if(digitalRead(f_key)==LOW){
      Keyboard.press(KEY_LEFT_CTRL);
      Keyboard.press('z');      
      delay(150);
      Keyboard.releaseAll();
    }

    //PASO ATRÁS
    if(digitalRead(g_key)==LOW){
      Keyboard.press(KEY_LEFT_ALT);
      Keyboard.press(KEY_LEFT_CTRL);
      Keyboard.press('z');      
      delay(150);
      Keyboard.releaseAll();
    }

    //PASO ADELANTE
    if(digitalRead(h_key)==LOW){
      Keyboard.press(KEY_LEFT_SHIFT);
      Keyboard.press(KEY_LEFT_CTRL);
      Keyboard.press('z');      
      delay(150);
      Keyboard.releaseAll();
    }
    
    //ENCODER
    if (millis() - lastClickTime > 1000){
      aState = digitalRead(OUTPUT_A);
          
      if (aState != aLastState){
        if (digitalRead(OUTPUT_B) != aState){
          //IZQUIERDA || ZOOM OUT
          Keyboard.press(KEY_LEFT_CTRL);
          Keyboard.press(47);          
          delay(10);
        }else{
          //DERECHA || ZOOm IN
          Keyboard.press(KEY_LEFT_CTRL);
          Keyboard.press(93);
          delay(10);
        }
        Keyboard.releaseAll();
        aLastState = aState;
      }
      
      if (digitalRead(BUTTON) == LOW){      
        if (lastButtonState == HIGH){
          //BOTÓN || GUARDAMOS EL DOCUMENTO ACTUAL (CTRL+S)
          Keyboard.press(KEY_LEFT_CTRL);
          Keyboard.press('s');
          delay(100);
          Keyboard.releaseAll();         
          
          lastClickTime = millis();
        }      
        lastButtonState = LOW;
      } else {      
        lastButtonState = HIGH;
      }
    }
    //FIN DEL ENCODER    
  }

  //FUNCIONAMIENTO DE LAS TECLAS DE ATAJO Y EL ENCODER SI HEMOS SELECCIONADO PREMIERE
  if(programa == "premiere"){
    //TECLAS    
    //EXPORTAR MEDIOS
    if(digitalRead(e_key)==LOW){      
      Keyboard.press(KEY_LEFT_CTRL);
      Keyboard.press('m');      
      delay(100);
      Keyboard.releaseAll();      
    }

    //TRAYECTO IZQUIERDO / AUMENTA LA VELOCIDAD SEGÚN LAS VECES QUE PULSEMOS
    if(digitalRead(f_key)==LOW){      
      Keyboard.press('j');      
      delay(100);
      Keyboard.releaseAll();
    }

    //TRAYECTO DERECHO / AUMENTA LA VELOCIDAD SEGÚN LAS VECES QUE PULSEMOS
    if(digitalRead(g_key)==LOW){      
      Keyboard.press('l');
      delay(100);
      Keyboard.releaseAll();
    }

    //EXPORTAR FOTOGRAMA
    if(digitalRead(h_key)==LOW){
      Keyboard.press(KEY_LEFT_CTRL);
      Keyboard.press(KEY_LEFT_SHIFT);      
      Keyboard.press('e');      
      delay(100);
      Keyboard.releaseAll();
    }
    
    //ENCODER
    if (millis() - lastClickTime > 1000){
      aState = digitalRead(OUTPUT_A);
          
      if (aState != aLastState){
        if (digitalRead(OUTPUT_B) != aState){
          //IZQUIERDA || RETROCEDER FOTOGRAMA
          Keyboard.press(KEY_LEFT_ARROW);
          delay(10);
        }else{
          //DERECHA || AVANZAR FOTOGRAMA
          Keyboard.press(KEY_RIGHT_ARROW);          
          delay(10);
        }
        Keyboard.releaseAll();
        aLastState = aState;
      }
      
      if (digitalRead(BUTTON) == LOW){      
        if (lastButtonState == HIGH){
          //BOTÓN || ACTIVAMOS LA REPRODUCCIÓN          
          Keyboard.press(' ');
          delay(100);
          Keyboard.releaseAll();         
          
          lastClickTime = millis();
        }      
        lastButtonState = LOW;
      } else {      
        lastButtonState = HIGH;
      }
    }
    //FIN DEL ENCODER    
  }

  //FUNCIONAMIENTO DE LAS TECLAS DE ATAJO Y EL ENCODER SI HEMOS SELECCIONADO OBS
  if(programa == "obs"){
    //TECLAS    
    //ATAJO 1
    if(digitalRead(e_key)==LOW){      
      Keyboard.press(KEY_LEFT_ALT);
      Keyboard.press(KEY_LEFT_SHIFT);
      Keyboard.press(KEY_LEFT_CTRL);
      Keyboard.press('1');      
      delay(100);
      Keyboard.releaseAll();          
    }

    //ATAJO 2
    if(digitalRead(f_key)==LOW){      
      Keyboard.press(KEY_LEFT_ALT);
      Keyboard.press(KEY_LEFT_SHIFT);
      Keyboard.press(KEY_LEFT_CTRL);
      Keyboard.press('2');      
      delay(100);
      Keyboard.releaseAll();      
    }

    //ATAJO 3
    if(digitalRead(g_key)==LOW){      
      Keyboard.press(KEY_LEFT_ALT);
      Keyboard.press(KEY_LEFT_SHIFT);
      Keyboard.press(KEY_LEFT_CTRL);
      Keyboard.press('3');      
      delay(100);
      Keyboard.releaseAll();      
    }

    //ATAJO 4
    if(digitalRead(h_key)==LOW){
      Keyboard.press(KEY_LEFT_ALT);
      Keyboard.press(KEY_LEFT_SHIFT);
      Keyboard.press(KEY_LEFT_CTRL);
      Keyboard.press('4');      
      delay(100);
      Keyboard.releaseAll();      
    }
    
    //ENCODER
    if (millis() - lastClickTime > 1000){
      aState = digitalRead(OUTPUT_A);
          
      if (aState != aLastState){
        if (digitalRead(OUTPUT_B) != aState){
          //IZQUIERDA || ATAJO 5
          Keyboard.press(KEY_LEFT_ALT);
          Keyboard.press(KEY_LEFT_SHIFT);
          Keyboard.press(KEY_LEFT_CTRL);
          Keyboard.press('5');                
          delay(10);
        }else{
          //DERECHA || ATAJO 6
          Keyboard.press(KEY_LEFT_ALT);
          Keyboard.press(KEY_LEFT_SHIFT);
          Keyboard.press(KEY_LEFT_CTRL);
          Keyboard.press('6');
          delay(10);
        }
        Keyboard.releaseAll();
        aLastState = aState;
      }
      
      if (digitalRead(BUTTON) == LOW){      
        if (lastButtonState == HIGH){
          //BOTÓN || ATAJO 7
          Keyboard.press(KEY_LEFT_ALT);
          Keyboard.press(KEY_LEFT_SHIFT);
          Keyboard.press(KEY_LEFT_CTRL);
          Keyboard.press('7');
          delay(10);
          Keyboard.releaseAll();         
          
          lastClickTime = millis();
        }      
        lastButtonState = LOW;
      } else {      
        lastButtonState = HIGH;
      }
    }
    //FIN DEL ENCODER    
  }  
}
