#include <Arduino_EdgeControl.h>
#include <openmvrpc.h>

openmv::rpc_scratch_buffer<256> scratch_buffer;
openmv::rpc_callback_buffer<8> callback_buffer;

// #define I2C_MASTER_UART_SLAVE 1
#define I2C_SLAVE_UART_MASTER 1

#if defined(I2C_SLAVE_UART_MASTER)
openmv::rpc_hardware_serial1_uart_master master(115200);
openmv::rpc_i2c1_slave slave(0x13);
#elif defined(I2C_MASTER_UART_SLAVE)
openmv::rpc_i2c1_master master(0x13, 10000);
openmv::rpc_hardware_serial1_uart_slave slave(115200);
#endif

constexpr uint32_t requestInterval { 1000 };
uint32_t requestNow { };

void dataReceived(void *in_data, size_t in_data_len)
{
    uint8_t data;
    memcpy(&data, in_data, sizeof(data));
    Serial.print("New Data Received: ");
    Serial.println(data);
}

void setup()
{
    EdgeControl.begin();
    
    Power.on(PWR_3V3);
    Power.on(PWR_VBAT);

    Power.on(PWR_MKR2);
    delay(5000); // Wait for MKR2 to power-on

    Serial.begin(115200);
    while(!Serial);
    master.begin();

    slave.register_callback("dataReceived", dataReceived);
    delay(1000);
    slave.begin();

    requestNow = millis() + requestInterval;
}

void loop()
{
    slave.loop(10, 10);
    
    if (millis() > requestNow) {
        Serial.println("Requesting Data Upload");
        uint32_t data = micros();
        master.call("sendData", &data, sizeof(data), nullptr, 0, false);
        requestNow = millis() + requestInterval;
        Serial.println("Data Upload Requested");
    }
}