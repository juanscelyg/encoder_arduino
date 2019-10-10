#include "Timer.h"
#include "Arduino.h"
#include <digitalWriteFast.h>

#define InterrupcionA 0
#define InterrupcionB 1
#define PinInterrupcionA 2
#define PinInterrupcionB 3
#define PinEntrada 7
#define analogo 0

volatile bool EncoderA;
volatile bool EncoderB;
volatile bool EncoderA_1;
volatile bool EncoderB_1;
volatile long EncoderTicks = 0;

volatile bool Entrada;

volatile long TicksAntes = 0;
volatile long TicksActual = 0;
volatile long TiempoAntes = 0;
volatile long TiempoActual = 0;
volatile long DifTicks = 0;

double DifTiempo = 0;
double velocidad = 0;

Timer t;

void setup()
{
  Serial.begin(115200);
  //Inicializar encoder
  pinMode(PinInterrupcionA, INPUT);
  digitalWrite(PinInterrupcionA, LOW);
  pinMode(PinInterrupcionB, INPUT);
  digitalWrite(PinInterrupcionB, LOW);
  pinMode(PinEntrada, INPUT);
  digitalWrite(PinEntrada, LOW);
  attachInterrupt(InterrupcionA, motorA, CHANGE);
  attachInterrupt(InterrupcionB, motorB, CHANGE);
  
  //Inicializar time
  int tickEvent = t.every(30, calcular);
}

void loop()
{
  t.update();
}

void calcular()
{
  Entrada=digitalReadFast(PinEntrada);
  //Envio señal de diferencia de pulsos
  TicksActual=EncoderTicks;
  TiempoActual=millis();
  DifTicks=TicksActual-TicksAntes;
  DifTiempo=TiempoActual-TiempoAntes;
  DifTiempo=DifTiempo/1000; //Tiempo en segundos
  velocidad=DifTicks/DifTiempo;
  TicksAntes=TicksActual;
  TiempoAntes=TiempoActual;
  //Serial.print(velocidad);
  Serial.print(EncoderTicks);
  Serial.print('\n');
}

void motorA(){
  EncoderB = digitalReadFast(PinInterrupcionB);
  EncoderA = digitalReadFast(PinInterrupcionA);
  
  EncoderTicks+=contador();
  
  EncoderA_1=EncoderA;
  EncoderB_1=EncoderB;
}

void motorB(){
  EncoderB = digitalReadFast(PinInterrupcionB);
  EncoderA = digitalReadFast(PinInterrupcionA);
  
  EncoderTicks+=contador();
  
  EncoderA_1=EncoderA;
  EncoderB_1=EncoderB;  
}

int contador(){
  if(EncoderA_1 && EncoderB_1){
    if(!EncoderA && EncoderB) return 1;
    if(EncoderA && !EncoderB) return -1;
  }else if(!EncoderA_1 && EncoderB_1){
    if(!EncoderA && !EncoderB) return 1;
    if(EncoderA && EncoderB) return -1;
  }else if(!EncoderA_1 && !EncoderB_1){
    if(EncoderA && !EncoderB) return 1;
    if(!EncoderA && EncoderB) return -1;
  }else if(EncoderA_1 && !EncoderB_1){
    if(EncoderA && EncoderB) return 1;
    if(!EncoderA && !EncoderB) return -1;
  }  
}
