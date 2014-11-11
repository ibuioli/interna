import oscP5.*;
import netP5.*;
import processing.serial.*;
import gab.opencv.*;
import processing.video.*;
import java.awt.*;

//VIDEO
Capture video;
OpenCV opencv;
OpenCV roi;

//COMUNICACION
Serial arduino;
OscP5 osc;
NetAddress local;

//VARIABLES
PImage procesada;
int posx, posy, ancho, alto, total, promedio, contador, thr;
color blanco = color(255, 255, 255);

void setup() {
  size(100, 100);

  //video = new Capture(this, 640, 480);
  video = new Capture(this, 640, 480, "HD Webcam C525");
  opencv = new OpenCV(this, 640, 480);
  roi = new OpenCV(this, 640, 480);
  opencv.loadCascade("haarcascade_righteye_2splits.xml");

  video.start();

  noSmooth();

  String puertoNombre = Serial.list()[0];
  arduino = new Serial(this, puertoNombre, 9600);
  arduino.bufferUntil('\n');

  osc = new OscP5(this, 12000);
  local = new NetAddress("127.0.0.1", 12000);

  thr = 54;
}

void draw() {
  contador = 0;
  background(0);
  opencv.loadImage(video);
  roi.loadImage(video);

  Rectangle[] ojos = opencv.detect();

  if (frameCount % 10 == 0) {
    if (ojos.length > 0) {
      posx = ojos[0].x + 15;
      posy = ojos[0].y + 20;
      ancho = ojos[0].width - 25;
      alto = ojos[0].height - 25;
    }
  }

  roi.setROI(posx, posy, abs(ancho), abs(alto));
  roi.threshold(thr);
  roi.invert();
  procesada = roi.getOutput().get(posx, posy, abs(ancho), abs(alto));

  procesada.loadPixels();
  for (int i = 0; i < ancho*alto; i++) {
    color b = procesada.pixels[i];
    if (b == blanco) {
      contador++;
    }
  }
  procesada.updatePixels();

  if (ojos.length > 0) {
    if (frameCount > 60) {
      total = ancho*alto;
      promedio = (contador*100)/total;
    }
  }

  OscMessage msj = new OscMessage("/ojos");
  msj.add(ojos.length);
  osc.send(msj, local); 

  if (ojos.length > 0) {
    if (promedio < 9) {
      arduino.write('H');
      thr = 35;
    } else {
      arduino.write('L');
      thr = 60;
    }
  } else {
    arduino.write('L');
  }

  println(promedio, thr);
  image(procesada, 0, 0);
}


void captureEvent(Capture c) {
  c.read();
}

