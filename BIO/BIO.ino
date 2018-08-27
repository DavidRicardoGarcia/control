
#include "variant.h"
#include <due_can.h>
#include <DueTimer.h>

//Leave defined if you use native port, comment if using programming port
//#define Serial SerialUSB

#define CAN0_TRANSFER_ID    0x05
#define CAN_FRAME_SEND_ID   0x16
#define MAX_CAN_FRAME_DATA_LEN   8

int incomingByte = 0;
float pb = 0, qb = 0, vl = 0;

String i1;

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
  Serial.println(pb);

}

void setqb(CAN_FRAME *frame) {

  qb = int(frame->data.low);

}

void setvl(CAN_FRAME *frame) {

  vl = int(frame->data.low);

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

void receiveRaspData() {
  while (Serial.available() == 0) {}
  while (Serial.available()) {
    i1 = Serial.readString();
  }
}

void loop() {
  //SendDataSensores(0x09,-5000000);
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
  receiveRaspData();

}
