#include <Arduino_PMIC.h>

void setup()
{
    // Optionally disable the LiPo charger
    PMIC.begin();
    PMIC.disableCharge();

    // Open the communication to the Edge Control
    Serial1.begin(115200);

    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);
}

void loop()
{
    // Wait for data
    if (Serial1.available()) {
        auto c = Serial1.read();
        digitalWrite(LED_BUILTIN, c);
    }
}
