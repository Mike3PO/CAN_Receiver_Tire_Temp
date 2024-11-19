/*
 * CAN port receiver example
 * Receives data on the CAN buss and prints to the serial port
 */

#include <ASTCanLib.h>

#define MESSAGE_ID        1200       // Message ID
#define MESSAGE_PROTOCOL  0       // CAN protocol (0: CAN 2.0A, 1: CAN 2.0B)
#define MESSAGE_LENGTH    8       // Data length: 8 bytes
#define MESSAGE_RTR       0       // rtr bit

// Function prototypes
void serialPrintData(st_cmd_t *msg);

// CAN message object
st_cmd_t Msg;

// Buffer for CAN data
uint8_t Buffer[8] = {};

void setup() {
  canInit(9600);         // Initialise CAN port. must be before Serial.begin
  Serial.begin(9600);       // start serial port
  Msg.pt_data = &Buffer[0];    // reference message data to buffer
  
  // Initialise CAN packet.
  // All of these will be overwritten by a received packet
  Msg.ctrl.ide = MESSAGE_PROTOCOL;  // Set CAN protocol (0: CAN 2.0A, 1: CAN 2.0B)
  Msg.id.ext   = MESSAGE_ID;        // Set message ID
  Msg.dlc      = MESSAGE_LENGTH;    // Data length: 8 bytes
  Msg.ctrl.rtr = MESSAGE_RTR;       // Set rtr bit
}

void loop() {
  // Clear the message buffer
  clearBuffer(&Buffer[0]);
  
  // Send command to the CAN port controller
  Msg.cmd = CMD_RX_DATA;
  
  // Wait for the command to be accepted by the controller
  Serial.print("Hello World");
  while(can_cmd(&Msg) != CAN_CMD_ACCEPTED);
  // Wait for command to finish executing
  Serial.print("Can_Accepted");
  while(can_get_status(&Msg) == CAN_STATUS_NOT_COMPLETED);
  // Data is now available in the message object
  // Print received data to the terminal
  Serial.print("Can_Status_Complete");
  Serial.print("\r\n");
  serialPrintData(&Msg);
}

void serialPrintData(st_cmd_t *msg){
  char textBuffer[50] = {0};
  if (msg->ctrl.ide>0){
    sprintf(textBuffer,"id %d ",msg->id.ext);
  }
  else
  {
    sprintf(textBuffer,"id %x ",msg->id.std);
  }
  Serial.print(textBuffer);
  
  //  IDE
  sprintf(textBuffer,"ide %d ",msg->ctrl.ide);
  Serial.print(textBuffer);
  //  RTR
  sprintf(textBuffer,"rtr %d ",msg->ctrl.rtr);
  Serial.print(textBuffer);
  //  DLC
  sprintf(textBuffer,"dlc %d ",msg->dlc);
  Serial.print(textBuffer);
  //  Data
  sprintf(textBuffer,"data ");
  Serial.print(textBuffer);
  
  
  for (int i =0; i<msg->dlc; i++){
    int data = msg->pt_data[i];
    sprintf(textBuffer,"%d ",data);
    Serial.print(textBuffer);
  }
  Serial.print("\r\n");
}
