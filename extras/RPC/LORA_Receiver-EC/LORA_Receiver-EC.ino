// LoRa example for the Edge control.
//
// This script configures your MKR1300 to receive Lora messages and send it to the 
// Arduino Edge control.
//
// Connect one of the MKR1300 to the MKR2 socket in the EdgeControl board
// This script is designed to pair with "LORA_EdgeControl.ino". Also 
// you will need a LoRa transmitter. We have used the LoRa Sender example from the other
// MKR1300.
// 
// The sketch make use of the OpenMV RPC library for communicating between the EdgeControl
// and the MKR1300 via UART
//
//  Created 20 April. 2021
//  by e.lopez

#include <SPI.h>
#include <LoRa.h>
#include <openmvrpc.h>

openmv::rpc_scratch_buffer<256> scratch_buffer; // All RPC objects share this buffer.
openmv::rpc_callback_buffer<8> callback_buffer; // All RPC objects share this buffer.
openmv::rpc_hardware_serial_uart_slave rpc(115200);

String LoRa_data = "";
//output interrupt pin
constexpr uint8_t interruptPin {12};

//***********************
// Call Backs
//***********************
//**** retrieve data ****
void rpc_retrieve_LoRa_msg(void **in_data, size_t* in_data_len) {
    
  size_t LoRa_data_len = sizeof(LoRa_data);  
  memcpy(in_data, &LoRa_data, LoRa_data_len);  
}

//***********************
//  LoRa
//***********************
void ParsePacket_LoRa() {
  // try to parse packet
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    // received a packet
    Serial.print("Received packet '");
    LoRa_data = "";
    while (LoRa.available()) {      
      LoRa_data += (char)LoRa.read();
    }
    Serial.print(LoRa_data);
    // print RSSI of packet
    Serial.print("' with RSSI ");
    Serial.println(LoRa.packetRssi());
    //trigger the interrupt
    digitalWrite(interruptPin, LOW);
    delay(1);
    digitalWrite(interruptPin, HIGH);
  }
}

//***********************
// SETUP
//***********************
void setup() {
  pinMode(interruptPin, OUTPUT);
  digitalWrite(interruptPin, HIGH);
  //register callback
  rpc.register_callback(F("retrieve_msg"), rpc_retrieve_LoRa_msg);
  //rpc init
  rpc.begin();
  //Serial init
  Serial.begin(115200);
  while (!Serial);
  //LoRa init
  Serial.println("LoRa Receiver");
  if (!LoRa.begin(868E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
}

//***********************
// LOOP
//***********************
void loop() {
  ParsePacket_LoRa();
  rpc.loop();
}
