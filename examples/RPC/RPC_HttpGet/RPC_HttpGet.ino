#include <Arduino_EdgeControl.h>
#include <Arduino_JSON.h>
#include <openmvrpc.h>

openmv::rpc_scratch_buffer<256> scratch_buffer;
openmv::rpc_hardware_serial1_uart_master rpc(115200);

constexpr uint32_t requestInterval { 10 * 1000 };
uint32_t requestNow {};

bool ledStatus { false };

void setup()
{
    EdgeControl.begin();

    Power.on(PWR_3V3);
    Power.on(PWR_VBAT);

    Power.on(PWR_MKR2);
    delay(5000); // Wait for MKR2 to power-on

    Serial.println(EdgeControl.serialNumber());

    // Init the I2C bus
    Wire.begin();
    delay(500);

    // Init the I/O Expander
    Serial.print("I/O Expander initializazion ");
    if (!Expander.begin()) {
        Serial.println("failed.");
        Serial.println("Please, be sure to enable gated 3V3 and 5V power rails");
        Serial.println("via Power.on(PWR_3V3) and Power.on(PWR_VBAT).");
    }
    Serial.println("succeeded.");

    // Configure the LED1 pin
    Expander.pinMode(EXP_LED1, OUTPUT);
    Expander.digitalWrite(EXP_LED1, HIGH);


    Serial.begin(115200);
    while (!Serial)
        ;
    
    rpc.begin();

    requestNow = millis();
}

void loop()
{
    if (millis() > requestNow) {

        call1stCallBack();

        delay(1000);

        call2ndCallback();

        requestNow = millis() + requestInterval;
    }

    Expander.digitalWrite(EXP_LED1, ledStatus);
}

void call1stCallBack()
{
    Serial.print("Getting Data - 1st Callback: ");
    size_t bufferLen { scratch_buffer.buffer_size() };
    char buffer[bufferLen] {};
    auto ret = rpc.call_no_args("getResponseBodyAlt", buffer, bufferLen, true, 1000, 2500);
    if (ret == 0) {
        Serial.println("Error");
        return;
    }
    Serial.print(bufferLen);
    Serial.print(" ");
    Serial.println(buffer);
    JSONVar dweet = JSON.parse(buffer);
    String pretty = JSON.stringify(dweet);
    Serial.println(dweet["with"][0]["content"]["hello"]);
    // Serial.println(dweet["hello"]);
}

void call2ndCallback()
{
    auto sn = EdgeControl.serialNumber();
    
    Serial.print("Getting Data - 2nd Callback: ");

    // size_t bufferAltLen;
    // char* bufferAlt;
    // auto retAlt = rpc.call_no_copy_no_args("getResponseBodyAlt", (void**)(&bufferAlt), &bufferAltLen, 1000, 2500);
    // auto retAlt = rpc.call_no_copy("getResponseBodyArgs", (void *)(sn.c_str()), sn.length(), (void**)(&bufferAlt), &bufferAltLen, 1000, 2500);
    
    size_t bufferLen { scratch_buffer.buffer_size() };
    char buffer[bufferLen] {};
    auto ret = rpc.call("getResponseBodyArgs", (void *)(sn.c_str()), sn.length(), buffer, bufferLen, false, 1000, 2500);

    if (ret == 0) {
        Serial.println("Error");
        return;
    }
    // Null-terminate the received buffer
    // buffer[bufferLen] = 0;
    // Serial.print(bufferLen);
    // Serial.print(" ");
    Serial.println(buffer);
    JSONVar dweet = JSON.parse(buffer);
    
    JSONVar ledCommandJSON = dweet["with"][0]["content"]["hello"];
    String ledCommand = (const char *)ledCommandJSON;

    Serial.println(ledCommand);

    ledStatus = !(ledCommand == "on");
    Serial.println(ledStatus);
}
