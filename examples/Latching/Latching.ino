#include <Arduino_OutdoorCarrier.h>

void setup()
{
    Serial.begin(9600);

    auto startNow = millis() + 2500;
    while (!Serial && millis() < startNow)
        ;

    delay(1000);
    Serial.println("Hello, Challenge!");

    Power.enable3V3();
    Power.enable5V();

    Wire.begin();
    Expander.begin();

    Serial.print("Waiting for IO Expander Initialisation...");
    while (!Expander) {
        Serial.print(".");
        delay(100);
    }
    Serial.println(" done.");

    Latching.begin();
}

void loop()
{
    Latching.pulse(LATCHING_CMD_1, POSITIVE);
    Latching.strobe(200);
    // Latching.digitalWrite(LATCHING_CMD_1, HIGH);
    // Latching.enable();
    // delay(200);
    // Latching.disable();
    Latching.pulse(LATCHING_CMD_1, NEGATIVE);
    Latching.strobe(200);
    // Latching.digitalWrite(LATCHING_CMD_1, LOW);
    // Latching.enable();
    // delay(200);
    // Latching.disable();
    Latching.pulse(LATCHING_OUT_1, POSITIVE);
    Latching.strobe(200);
    // Latching.digitalWrite(LATCHING_OUT_1, HIGH);
    // Latching.enable();
    // delay(200);
    // Latching.disable();
    Latching.pulse(LATCHING_OUT_1, NEGATIVE);
    Latching.strobe(200);
    // Latching.digitalWrite(LATCHING_OUT_1, LOW);
    // Latching.enable();
    // delay(200);
    // Latching.disable();
    delay(1000);
}