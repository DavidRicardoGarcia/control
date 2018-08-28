
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
float vl = 0;

char aa, tramRasp[9];
String a, si3, ssoc;
int i3, soc;
char frst, scnd;
bool v = false;

void setup()
{

  Serial.begin(115200);

  // Initialize CAN0 and CAN1, Set the proper baud rates here
  //Can0.begin(1000000);
  Can0.begin(CAN_BPS_250K);
  //set the filters for each mailbox and its respective mask
  Can0.setRXFilter(0, 0x03, 0xFF, false);
  Can0.setCallback(0, mensaje0);

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

void mensaje0(CAN_FRAME *frame) {

  setvl(frame);

}

void setvl(CAN_FRAME *frame) {

  vl = int(frame->data.low);
  Serial.println(vl);
  v = true;
  sendToRasp();
}

void sendToRasp() {
  if (v) {
    sprintf(tramRasp, "v%07de", vl);
    Serial.print(tramRasp);
    v = false;
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
      //Serial.println(head);
      if (frst == 'i') {
        si3 = a.substring(1, 8);
        i3 = si3.toInt();
        ssoc = a.substring(9, 15);
        soc = ssoc.toInt();
      }
    }
  }
}


void loop() {
  receiveRaspData();
  SendData(0x08, i3);
  SendData(0x0B, soc);
}
