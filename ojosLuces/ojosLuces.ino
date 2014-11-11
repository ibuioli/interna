//**************************************************************//
//  Name    : shiftOutCode, Hello World                                
//  Author  : Carlyn Maw,Tom Igoe, David A. Mellis 
//  Date    : 25 Oct, 2006    
//  Modified: 23 Mar 2010                                 
//  Version : 2.0                                             
//  Notes   : Code for using a 74HC595 Shift Register           //
//          : to count from 0 to 255                           
//****************************************************************

//Pin connected to ST_CP of 74HC595
int latchPin = 8;
//Pin connected to SH_CP of 74HC595
int clockPin = 12;
////Pin connected to DS of 74HC595
int dataPin = 11;

long distancia;
long tiempo;

int incomingByte;


void setup() {
  //MULTIPLEXOR
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);

  //SENSOR PROXIMIDAD:
  Serial.begin(9600);
  pinMode(7, OUTPUT); //trigger
  pinMode(6, INPUT); //echo
}

void loop() {
  //SENSOR PROXIMIDAD:
  digitalWrite(7,LOW); /* Por cuestión de estabilización del sensor*/
  delayMicroseconds(5);
  digitalWrite(7, HIGH); /* envío del pulso ultrasónico*/
  delayMicroseconds(10);
  /*tiempo=pulseIn(6, HIGH);*/ /* Función para medir la longitud del pulso entrante. Mide el tiempo que transcurrido entre el envío
   del pulso ultrasónico y cuando el sensor recibe el rebote, es decir: desde que el pin 12 empieza a recibir el rebote, HIGH, hasta que
   deja de hacerlo, LOW, la longitud del pulso entrante*/
  /*distancia= int(0.017*tiempo);*/ /*fórmula para calcular la distancia obteniendo un valor entero*/
  /*Monitorización en centímetros por el monitor serial*/
  //Serial.print("Distancia ");
  //Serial.println(distancia);
  //Serial.println(" cm");
  //Serial.write(distancia);

  //MULTIPLEXOR
  if (Serial.available() > 0) {
   incomingByte = Serial.read();
    
    if (incomingByte == 'H') {
      digitalWrite(latchPin, LOW);
      shiftOut(dataPin, clockPin, MSBFIRST, random(0, 255));// Enciende las primeras 8 salidas. 595 UNO
      shiftOut(dataPin, clockPin, MSBFIRST, random(0, 255));//595 DOS
      shiftOut(dataPin, clockPin, MSBFIRST, random(0, 255));//595 TRES
      digitalWrite(latchPin, HIGH);
    } 
    
    if (incomingByte == 'L') {
      digitalWrite(latchPin, LOW);
      shiftOut(dataPin, clockPin, MSBFIRST, 255);// Enciende las primeras 8 salidas. 595 UNO
      shiftOut(dataPin, clockPin, MSBFIRST, 255);//595 DOS
      shiftOut(dataPin, clockPin, MSBFIRST, 255);//595 TRES
      digitalWrite(latchPin, HIGH);
    }
  }else{
    digitalWrite(latchPin, LOW);
      shiftOut(dataPin, clockPin, MSBFIRST, 255);// Enciende las primeras 8 salidas. 595 UNO
      shiftOut(dataPin, clockPin, MSBFIRST, 255);//595 DOS
      shiftOut(dataPin, clockPin, MSBFIRST, 255);//595 TRES
      digitalWrite(latchPin, HIGH);
  }
  
  delay(100);
}



