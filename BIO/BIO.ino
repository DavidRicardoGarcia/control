
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
int pb = 0, qb = 0, vl = 0;

char aa, tramRasp[25];
String si1;
int i1;

bool p = false, q = false, v = false;

void setup()
{

  Serial.begin(115200);

  // Initialize CAN0 and CAN1, Set the proper baud rates here
  //Can0.begin(1000000);
  Can0.begin(CAN_BPS_250K);
  //set the filters for each mailbox and its respective mask
  Can0.setRXFilter(0, 0x01, 0xFF, false);
  Can0.setRXFilter(1, 0x02, 0xFF, false);
  Can0.setRXFilter(2, 0x03, 0xFF, false);

  Can0.setCallback(0, mensaje0);
  Can0.setCallback(1, mensaje1);
  Can0.setCallback(2, mensaje2);
  // Can0.watchFor();
  // Set the pins to control the steppers

}

void setpb(CAN_FRAME *frame) {

  pb = int(frame->data.low);
  //Serial.println(pb);
  p = true;
  sendToRasp();
}

void setqb(CAN_FRAME *frame) {

  qb = int(frame->data.low);
  q = true;
  sendToRasp();

}

void setvl(CAN_FRAME *frame) {

  vl = int(frame->data.low);
  v = true;
  sendToRasp();
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

void mensaje0(CAN_FRAME *frame) {
  Serial.println("llego el mensaje 0");
  setpb(frame);

}


void mensaje1(CAN_FRAME *frame) {
  // Serial.println("llego el mensaje 1");
  setqb(frame);

}

void mensaje2(CAN_FRAME *frame) {
  //  Serial.println("llego el mensaje 2");
  setvl(frame);

}

void sendToRasp() {
  if (v && p && q) {
    sprintf(tramRasp, "v%07dp%07dq%07de", vl, pb, qb);
    Serial.print(tramRasp);
    v = false;
    p = false;
    q = false;
  }
}

void receiveRaspData() {
  while (Serial.available() == 0) {}
  si1 = "";
  while (Serial.available() > 0) {
    aa = Serial.read();
    si1 += aa;
    if (aa == '\n') {
      i1 = si1.toInt();
    }
  }
}

void loop() {

  receiveRaspData();
  SendData(0x06, i1);

}
