
#include "variant.h"
#include <due_can.h>
#include <DueTimer.h>

//Leave defined if you use native port, comment if using programming port
//#define Serial SerialUSB

#define CAN0_TRANSFER_ID    0x05
#define CAN_FRAME_SEND_ID   0x16
#define MAX_CAN_FRAME_DATA_LEN   8
int incomingByte = 0; 
String inString="";
String an=" ";
float i1=0,i2=0,i3=0;

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


void SendDataSensores(int ident,int a )
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

  i1=int(frame->data.low);
  Serial.println(i1);
  
}

void seti2(CAN_FRAME *frame) {

  i2=int(frame->data.low);
  
}

void seti3(CAN_FRAME *frame) {

  i3=int(frame->data.low);
  
}

 void mensaje0(CAN_FRAME *frame){
 
  seti1(frame);
   
  }


 void mensaje1(CAN_FRAME *frame){
  seti2(frame);
    
  }

 void mensaje2(CAN_FRAME *frame){
  seti3(frame);
    
  }


void loop() {

float a=101;
      an=String(a,HEX);
   Serial.println(an);
  while(Serial.available()>0){
    char a=Serial.read();
    inString += a;
    if (a == '\n') {
      Serial.println(inString);
      inString = "";
    }
}
}