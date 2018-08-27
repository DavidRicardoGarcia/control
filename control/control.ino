
#include "variant.h"
#include <due_can.h>
#include <DueTimer.h>

//Leave defined if you use native port, comment if using programming port
//#define Serial SerialUSB

#define CAN0_TRANSFER_ID    0x05
#define CAN_FRAME_SEND_ID   0x16
#define MAX_CAN_FRAME_DATA_LEN   8

int incomingByte = 0;
String inString = "";
String an = " ";
float pm = 0, qm = 0, soc = 0;

String a, pd, qd, pb, qb;
char head, frst, scnd;

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

void SendDataSensores(int ident, int a )
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
  Serial.println(pm);

}

void setqm(CAN_FRAME *frame) {

  qm = int(frame->data.low);

}

void setsoc(CAN_FRAME *frame) {

  soc = int(frame->data.low);

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

void receiveRaspData() {
  while (Serial.available() == 0) {}
  while (Serial.available()) {
    a = Serial.readString();
  }
//  Serial.println(a);
//  Serial.println("Pasa While");
  head = a.charAt(0);
  frst = a.charAt(1);
  scnd = a.charAt(10);
  //Serial.println(head);
  switch (head) {
    case 'd':
      pd = a.substring(2, 9);
//      Serial.println("Pd =");
//      Serial.println(pd);
      qd = a.substring(10, 17);
//      Serial.println("Qd =");
//      Serial.println(qd);
      break;
    case 'b':
      pb = a.substring(2, 9);
//      Serial.println("Pb =");
//      Serial.println(pb);
      qb = a.substring(10, 17);
//      Serial.println("Qb =");
//      Serial.println(qb);
      break;
  }
}

void loop() {
  SendDataSensores(0x01, -10000000);
  //float a=101;
  //      an=String(a,HEX);
  //   Serial.println(an);
  //  while(Serial.available()>0){
  //    char a=Serial.read();
  //    inString += a;
  //    if (a == '\n') {
  //      Serial.println(inString);
  //      inString = "";
  //    }
  //}
  delay(100);
//  Serial.println("Pase poaca");
  receiveRaspData();

}
