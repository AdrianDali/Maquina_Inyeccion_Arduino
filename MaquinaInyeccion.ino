 //---LLIBRERIAS
#include <EEPROM.h>
#include <LiquidCrystal_I2C.h> 
LiquidCrystal_I2C lcd(0x27,16,4); 
//----ENTRADAS Y SALIDAS------------
const int ReleCierreMolde = 10;
const int ReleAbreMolde = 9;
const int ReleInyeccion = 8;
    //--BotonIniciarCicloAutomatico---
const int CicloAutomaticoBoton = 4;
    //--------------------------------
const int BotonSelectorAutomatico = 2;
const int ManualInyeccion = 7;
const int ManualAbreMolde = 6;
//const int Motor = 5;
const int MicMoldeCerrado = 1;
const int MicMoldePosicion = 3;
int ValCicloAuto;
int ValorBotAuto;
int ValorManInye;
int ValorManAbre;
//int ValorMotor;
int ValorMicCerrado;
int ValorMicPosicion;
float TiempoPausa = (EEPROM.get(20,TiempoPausa));
float TiempoApertura = (EEPROM.get(30,TiempoApertura));
float TiempoCarga = (EEPROM.get(10,TiempoCarga));
float TiempoInyeccion = (EEPROM.get(0,TiempoInyeccion));
float ayuda = 0;
float ayudaDos = 0; 

//-----------------------------------
//------------------------------------- 
const int Arriba = 13;
const int Abajo = 12;
const int Enter = 11;
int ValorArriba;
int ValorAbajo;
int ValorEnter;
int Contador = 1 ;
int ContadorEnter = 0;
int Aux = 0;


void setup() {
  Serial.begin(9600);
  pinMode(Arriba,INPUT_PULLUP);
  pinMode(Abajo,INPUT_PULLUP);
  pinMode(Enter,INPUT_PULLUP);
  IniciarLCD();
  LCDTiempos();
  ImpresionTiempos();
  //---ENTRADAS Y SALIDAS------------
  pinMode(ReleCierreMolde,OUTPUT);
  pinMode(ReleAbreMolde,OUTPUT);
  pinMode(ReleInyeccion,OUTPUT);
  pinMode(CicloAutomaticoBoton, INPUT_PULLUP);
  pinMode(BotonSelectorAutomatico,INPUT_PULLUP);
  pinMode(ManualInyeccion,INPUT_PULLUP);
  pinMode(ManualAbreMolde,INPUT_PULLUP);
  //pinMode(Motor,INPUT_PULLUP);
  pinMode(MicMoldeCerrado,INPUT_PULLUP);
  pinMode(MicMoldePosicion,INPUT_PULLUP);
  //----------------------------------
  //attachInterrupt(digitalPinToInterrupt(3),Manual,RISING);
  //---INICIO-------------------------
  Reset();
  Serial.println("gola");
  
  
}

//---VOID LOOP-------------------
void loop() {

  //--------------------------------------------------
  ValCicloAuto = digitalRead(CicloAutomaticoBoton);
  ValorBotAuto = digitalRead(BotonSelectorAutomatico);
  ValorManInye = digitalRead(ManualInyeccion);
  ValorManAbre = digitalRead(ManualAbreMolde);
  //ValorManCierra = digitalRead(ManualCierraMolde);
  ValorMicCerrado = digitalRead(MicMoldeCerrado);
  ValorMicPosicion = digitalRead(MicMoldePosicion);
  Modo();
  

  
  
  //----------------------------------
    ValorArriba = digitalRead(Arriba);
  ValorAbajo = digitalRead(Abajo);
  ValorEnter = digitalRead(Enter);
  lcd.display();
  if(Contador){
   //----------------------- 
   if(Aux == 0){
   if(ValorArriba == 0){
    Contador = Contador + 1;
     if(Contador > 4){
    Contador = 1;
   }
    Serial.println(Contador);
    SeleccionMenu(); 
    delay(100);
   }
   }
   //----------------------
   if(ValorEnter == 0){
    delay(100);
    Aux ++;
    Serial.println("AUX" );
     Serial.println(Aux);
    if(Aux == 2){
      Eeprom();
     Aux = 0;
     SeleccionMenu();
      Serial.println("REI");
      Serial.println(Aux);
      Serial.println(Contador);  
      }
      if(Aux == 1){
       switch (Contador){
        case 1 : 
        DentroInyeccion();
        ImpresionTiempos();
        break;
        case 2:
        DentroCarga();
        ImpresionTiempos();
        break;
        case 3:
        DentroPausa();
        ImpresionTiempos();
        break;
        case 4:
        DentroApertura();
        ImpresionTiempos();
        break;
      }
               
        
        
    
      }
    
   }
   if(Aux == 1){
    if(ValorArriba ==0){
        switch (Contador){
        case 1 :
        TiempoInyeccion = TiempoInyeccion + .10;
        ImpresionTiempos();
        delay(100);
        break;
        case 2 :
         TiempoCarga = TiempoCarga + .10;
        ImpresionTiempos();
        delay(100);
        break;
        case 3:
         TiempoPausa = TiempoPausa + .10;
        ImpresionTiempos();
        delay(100);
        break;
        case 4:
         TiempoApertura = TiempoApertura + .10;
        ImpresionTiempos();
        delay(100);
        break;
      }
    }
      if(ValorAbajo==0){
       switch (Contador){
        case 1 :
        if(TiempoInyeccion >= 0.10){
        TiempoInyeccion = TiempoInyeccion - .10;
        ImpresionTiempos();
        delay(100);
        }
        break;
        case 2 :
        if(TiempoCarga >= 0.10){
         TiempoCarga = TiempoCarga - .10;
        ImpresionTiempos();
        delay(100);
        }
        break;
        case 3:
         if(TiempoPausa >= 0.10){
         TiempoPausa = TiempoPausa - .10;
        ImpresionTiempos();
        delay(100);
         }
        break;
        case 4:
        if(TiempoApertura >= 0.10){
         TiempoApertura = TiempoApertura - .10;
        ImpresionTiempos();
        delay(100);
        break;
        }
      }
      }
   }
        
   //---------------------------
    if(Aux == 0){
    if(ValorAbajo == 0){
      Contador = Contador - 1;
           if(Contador == 0){
              Contador = 4;
             }
      Serial.println(Contador);
      SeleccionMenu(); 
      delay(100);
       }
    }
  }  
      
}
//----------------------------------------
//----------------------------------------
void Modo(){
  if(ValorBotAuto == 0 ){
    ayudaDos = 0;
    
    CicloAutomatico();
  
  }
  else{
    ayudaDos = 1;
    ayuda = 1; 
    Manual();
  }
}
//-------------------------------------
void Reset(){
  digitalWrite(ReleCierreMolde,HIGH);
  digitalWrite(ReleAbreMolde,HIGH);
  digitalWrite(ReleInyeccion,HIGH);
}
//----------------------------------------
void MCierreMolde(){
  digitalWrite(ReleCierreMolde,LOW);
}
//-----------------------------------------
void MAbreMolde(){
  digitalWrite(ReleAbreMolde,LOW);
}
//-------------------------------------------
void MInyeccion(){
  digitalWrite(ReleInyeccion, LOW);
  digitalWrite(ReleAbreMolde, LOW);
}
//-------------------------------------------
void CicloAutomatico(){
  if(ValCicloAuto == 0 ){
    ayuda =0;
  if(ValorMicCerrado == 0 and ValorMicPosicion == 0 and ayuda == 0 and ayudaDos == 0){
    digitalWrite(ReleCierreMolde, LOW);
   //digitalWrite(ReleAbreMolde, LOW);
      digitalWrite(ReleInyeccion, LOW);
      delay(TiempoInyeccion * 1000);
      digitalWrite(ReleInyeccion, HIGH);
      delay(TiempoCarga * 1000);
      
      Reset();
      delay(TiempoPausa * 1000);
      digitalWrite(ReleAbreMolde,LOW);
      delay(TiempoApertura * 1000);
      digitalWrite(ReleAbreMolde,HIGH);
      
    }
  }
   if (ValCicloAuto == 0 and ValorMicCerrado == 0 and ValorMicPosicion == 1){
    digitalWrite(ReleCierreMolde, LOW);
    ayuda = 2;
   }
   if(ValorMicPosicion == 0 and ayuda == 2){
      digitalWrite(ReleInyeccion, LOW);
      delay(TiempoInyeccion * 1000);
      digitalWrite(ReleInyeccion, HIGH);
      delay(TiempoCarga * 1000);
      digitalWrite(ReleCierreMolde, HIGH);
      Reset();
      delay(TiempoPausa * 1000);
      digitalWrite(ReleAbreMolde,LOW);
      delay(TiempoApertura * 1000);
      digitalWrite(ReleAbreMolde,HIGH);
      ayuda = 0;
      
    } 
  
}
void Manual(){
  Serial.print("Si");
  if(/*ValorManCierra*/ ValCicloAuto == 0 and ayudaDos == 1 and ValorMicPosicion == 0){
    MCierreMolde();
  }else{
    Reset();
  }
  if(ValorManAbre == 0){
    MAbreMolde();
  }
  if(ValorManInye == 0){
    MInyeccion();
  }
  
}
//---------------------------------------------
//--------------------------------------------------
//-----------------------------------------------------
void IniciarLCD(){
  lcd.init();
  lcd.backlight();
  lcd.clear(); 
}
//-----------------------------------------------------
void LCDTiempos(){
  lcd.setCursor(1,0);
  lcd.print("T-INYECCION:"); 
  lcd.setCursor (1,1);
  lcd.print("T-CARGA:");
  lcd.setCursor(1,2);
  lcd.print("T-PAUSA:");
  lcd.setCursor(1,3);
  lcd.print("T-APERTURA:");
  
}
//----------------------------------------------------
void SeleccionMenu(){
  switch(Contador){
    case 1:
    MenuInyeccion();
      ImpresionTiempos();
    //Serial.println("inyeccion");
    break;
    case 2:
    MenuCarga();
      ImpresionTiempos();
     //Serial.println("carga");
    break;
    case 3:
    MenuPausa();
      ImpresionTiempos();
    //Serial.println("Pausa");
    break;
    case 4:
    MenuApertura();
      ImpresionTiempos();
     //Serial.println("apertura");
     break;
    
  }
  
}
//------------------------------------------------------
void MenuInyeccion(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(">");
  LCDTiempos();
  Contador = 1;
}
//-----------------------------------------------------
void MenuCarga(){
  lcd.clear();
  lcd.setCursor(0,1);
  lcd.print(">");
  LCDTiempos();
  Contador = 2;
}
//-----------------------------------------------
void MenuPausa(){
  lcd.clear();
  lcd.setCursor(0,2);
  lcd.print(">");
  LCDTiempos();
  Contador = 3;
}
//----------------------------------------------
void MenuApertura(){
  lcd.clear();
  lcd.setCursor(0,3);
  lcd.print(">");
  LCDTiempos();
  Contador = 4;
}
//--------------------------------------------
void DentroCarga(){
  lcd.clear();
   LCDTiempos();
   
  lcd.setCursor(18,1);
  lcd.print("<<");

}
//---------------------------------------------
void DentroInyeccion(){
  lcd.clear();
  LCDTiempos();
 
  lcd.setCursor(18,0);
  lcd.print("<<");
  
}
//---------------------------------------------
void DentroPausa(){
  lcd.clear();
  LCDTiempos();
  
  lcd.setCursor(18,2);
  lcd.print("<<");
  LCDTiempos();
 
}
//---------------------------------------------
void DentroApertura(){
  lcd.clear();
 LCDTiempos();
  lcd.setCursor(18,3);
  lcd.print("<<");
 
 
}
//----------------------------------------
void ImpresionTiempos(){
  lcd.setCursor(13,0);
  lcd.print(TiempoInyeccion);
  lcd.setCursor(13,1);
  lcd.print(TiempoCarga);
  lcd.setCursor(13,2);
  lcd.print(TiempoPausa);
  lcd.setCursor(13,3);
  lcd.print(TiempoApertura);
}
//---------------------------------
void ModificarTiempos(){
  switch(Contador){
    case 1:

    Serial.println("inyeccion");
    
    break;
    case 2:
   
     Serial.println("carga");
    break;
    case 3:
    
    Serial.println("Pausa");
    break;
    case 4:
  
    Serial.println("apertura");
    break;
    
  }
   delay(100);  
}
//-----------------------------------------------------------------------
void Eeprom(){
  EEPROM.put(0,TiempoInyeccion);
  Serial.println(EEPROM.get(0,TiempoInyeccion));
  EEPROM.put(10,TiempoCarga);
  Serial.println(EEPROM.get(10,TiempoCarga));
  EEPROM.put(20,TiempoPausa);
  Serial.println(EEPROM.get(20,TiempoPausa));
  EEPROM.put(30,TiempoApertura);
  Serial.println(EEPROM.get(30,TiempoApertura));
  
  
}
