
#include "variant.h"
#include <due_can.h>
#include <DueTimer.h>

//Leave defined if you use native port, comment if using programming port
//#define Serial SerialUSB

#define CAN0_TRANSFER_ID    0x05
#define CAN_FRAME_SEND_ID   0x16
#define MAX_CAN_FRAME_DATA_LEN   1

int incomingByte = 0; 

void setup()
{

Serial.begin(115200);
  
// Initialize CAN0 and CAN1, Set the proper baud rates here
//Can0.begin(1000000);
Can0.begin(CAN_BPS_250K);
//set the filters for each mailbox and its respective mask
 Can0.setRXFilter(0, 0x01, 0xFF, false);
 Can0.setRXFilter(1, 0x02, 0xFF, false);

 Can0.setCallback(0, mensaje0);
 Can0.setCallback(1, mensaje1); 

// Can0.watchFor();
 // Set the pins to control the steppers  

}

void printFrame(CAN_FRAME *frame, int filter) {
   Serial.print("Fltr: ");
   if (filter > -1) Serial.print(filter);
   else Serial.print("???");
   Serial.print(" ID: 0x");
   Serial.print(frame->id, HEX);
   Serial.print(" Len: ");
   Serial.print(frame->length);
   Serial.print(" Data: 0x");
   for (int count = 0; count < frame->length; count++) {
       Serial.print(frame->data.bytes[count], HEX);
       Serial.print(" ");
   }
   Serial.print("\r\n");
}


void SendDataSensores(int ident)
{
  CAN_FRAME outgoing;
  outgoing.id = ident;
  outgoing.extended = false;
  outgoing.priority = 0; //0-15 lower is higher priority
  outgoing.length = MAX_CAN_FRAME_DATA_LEN;
  
  outgoing.data.byte[0] = 0x01;
 // outgoing.data.byte[1] = 0x01;
//  outgoing.data.byte[2] = 0x02;
  //outgoing.data.high = 0x00;
  Can0.sendFrame(outgoing);
  
}

 void mensaje0(CAN_FRAME *frame){
   Serial.println("llego el mensaje 0");
  // printFrame(frame, 5);
   
  }


 void mensaje1(CAN_FRAME *frame){
   Serial.println("llego el mensaje 0");
  // printFrame(frame, 5);
    
  }


void loop() {
 if (Serial.available() > 0) {
                // read the incoming byte:
                incomingByte = Serial.read();

                // say what you got:
                Serial.print("I received: ");
                Serial.println(incomingByte, DEC);
        }
}
