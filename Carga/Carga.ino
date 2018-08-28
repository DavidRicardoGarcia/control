
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
int i1 = 0, i2 = 0, i3 = 0;

String a, spm, sqm, svl;
int pm, qm, vl;
char frst, scnd, thrd;

char aa, tramRasp[25];
bool bi1 = false, bi2 = false, bi3 = false;

void setup()
{

  Serial.begin(115200);

  // Initialize CAN0 and CAN1, Set the proper baud rates here
  //Can0.begin(1000000);
  Can0.begin(CAN_BPS_250K);
  //set the filters for each mailbox and its respective mask
  Can0.setRXFilter(0, 0x06, 0xFF, false);
  Can0.setRXFilter(1, 0x07, 0xFF, false);
  Can0.setRXFilter(2, 0x08, 0xFF, false);

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

void seti1(CAN_FRAME *frame) {

  i1 = int(frame->data.low);
  //Serial.println(i1);
  bi1 = true;
  sendToRasp();
}

void seti2(CAN_FRAME *frame) {

  i2 = int(frame->data.low);
  bi2 = true;
  sendToRasp();
}

void seti3(CAN_FRAME *frame) {

  i3 = int(frame->data.low);
  bi3 = true;
  sendToRasp();
}

void mensaje0(CAN_FRAME *frame) {

  seti1(frame);

}


void mensaje1(CAN_FRAME *frame) {
  seti2(frame);

}

void mensaje2(CAN_FRAME *frame) {
  seti3(frame);

}

void sendToRasp() {
  if (bi1 && bi2 && bi3) {
    sprintf(tramRasp, "b%07dd%07dp%07de", i1, i2, i3);
    Serial.print(tramRasp);
    bi1 = false;
    bi2 = false;
    bi3 = false;
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
      //Serial.println(head);
      if (frst == 'a') {
        spm = a.substring(1, 8);
        pm = spm.toInt();
        sqm = a.substring(9, 16);
        qm = sqm.toInt();
        svl = a.substring(17, 24);
        vl = svl.toInt();
      }
    }
  }
}

void loop() {
  receiveRaspData();
  SendData(0x01, pm);
  SendData(0x02, qm);
  SendData(0x04, vl);
}
