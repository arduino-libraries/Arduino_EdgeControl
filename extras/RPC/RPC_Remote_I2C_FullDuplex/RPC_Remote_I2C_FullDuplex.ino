#include <openmvrpc.h>

openmv::rpc_scratch_buffer<256> scratch_buffer; // All RPC objects share this buffer.
openmv::rpc_callback_buffer<8> callback_buffer; // All RPC objects share this buffer.

 #define UART_SLAVE_I2C_MASTER 1
//#define UART_MASTER_I2C_SLAVE 1

#if defined(UART_SLAVE_I2C_MASTER)
openmv::rpc_hardware_serial_uart_slave slave(115200);
openmv::rpc_i2c_master master(0x13, 10000);
#elif defined(UART_MASTER_I2C_SLAVE)
openmv::rpc_i2c_slave slave(0x13);
openmv::rpc_hardware_serial_uart_master master(115200);
#endif


constexpr uint32_t sendInterval { 2500 };
uint32_t sendNow { 0 };
uint8_t receivedData { 0 };
//////////////////////////////////////////////////////////////
// Call Backs
//////////////////////////////////////////////////////////////

void sendDataUpstream(void *in_data, size_t in_data_len) {
  uint32_t upstreamData { };

  memcpy(&upstreamData, in_data, sizeof(upstreamData));
  
  Serial.print("Sending Data Upstream: ");
  Serial.println(upstreamData);
}

void setup() {

  slave.register_callback("sendData", sendDataUpstream);

  // Startup the RPC interface and a debug channel.
  slave.begin();
  master.begin();
  
  Serial.begin(115200);
  sendNow = millis() + sendNow;
}

// Once all call backs have been registered we can start
// processing remote events.

void loop() {
  
  slave.loop();

  if (millis() > sendNow + sendInterval) {
    Serial.println("Sending Received Data");
    master.call("dataReceived", &receivedData, sizeof(receivedData), nullptr, 0, false);
    receivedData++;
    sendNow = millis() + sendInterval;
  }
}
