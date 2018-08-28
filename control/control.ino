
#include "variant.h"
#include <due_can.h>
#include <DueTimer.h>
#include <stdio.h>

//Leave defined if you use native port, comment if using programming port
//#define Serial SerialUSB

#define CAN0_TRANSFER_ID    0x05
#define CAN_FRAME_SEND_ID   0x16
#define MAX_CAN_FRAME_DATA_LEN   8

int incomingByte = 0;
String inString = "";
String an = " ";
int pm = 0, qm = 0, soc = 0;

String a, spd, sqd, spb, sqb;
int pd, qd, pb, qb;
char frst, scnd, thrd, frth;

char aa, tramRasp[25];
bool p = false, q = false, s = false;

void setup()
{

  Serial.begin(115200);

  // Initialize CAN0 and CAN1, Set the proper baud rates here
  //Can0.begin(1000000);
  Can0.begin(CAN_BPS_250K);
  //set the filters for each mailbox and its respective mask
  Can0.setRXFilter(0, 0x09, 0xFF, false);
  Can0.setRXFilter(1, 0x0A, 0xFF, false);
  Can0.setRXFilter(2, 0x0B, 0xFF, false);

  Can0.setCallback(0, mensaje0);
  Can0.setCallback(1, mensaje1);
  Can0.setCallback(2, mensaje2);
  // Can0.watchFor();
  // Set the pins to control the steppers

}

void SendData(int ident, int a )
{
  CAN_FRAME outgoing;
  outgoing.id = ident;
  outgoing.extended = false;
  outgoing.priority = 0; //0-15 lower is higher priority
  outgoing.length = MAX_CAN_FRAME_DATA_LEN;
  outgoing.data.low = a;
  Can0.sendFrame(outgoing);

}


void setpm(CAN_FRAME *frame) {

  pm = int(frame->data.low);
  //Serial.println(pm);
  p = true;
  sendToRasp();
}

void setqm(CAN_FRAME *frame) {

  qm = int(frame->data.low);
  q = true;
  sendToRasp();
}

void setsoc(CAN_FRAME *frame) {

  soc = int(frame->data.low);
  s = true;
  sendToRasp();
}


void mensaje0(CAN_FRAME *frame) {
  setpm(frame);

}


void mensaje1(CAN_FRAME *frame) {
  setqm(frame);

}

void mensaje2(CAN_FRAME *frame) {
  setsoc(frame);

}

void sendToRasp() {
  if (p && q && s) {
    sprintf(tramRasp, "p%07dq%07ds%07de", pm, qm, soc);
    Serial.print(tramRasp);
    p = false;
    q = false;
    s = false;
  }
}

void receiveRaspData() {

  while (Serial.available() == 0) {}
  a = "";
  while (Serial.available() > 0) {
    aa = Serial.read();
    a += aa;
    if (aa == '\n') {
      frst = a.charAt(0);
      scnd = a.charAt(8);
      thrd = a.charAt(16);
      frth = a.charAt(24);
      //Serial.println(head);
      if (frst == 'a') {
        spb = a.substring(1, 8);
        pb = spb.toInt();
        sqb = a.substring(9, 16);
        qb = sqb.toInt();
        spd = a.substring(17, 24);
        pd = spd.toInt();
        sqd = a.substring(25, 32);
        qd = sqd.toInt();
      }
    }
  }
}

void loop() {

  receiveRaspData();
  SendData(0x01, pb);
  SendData(0x02, qb);
  SendData(0x04, pd);
  SendData(0x05, qd);
}
