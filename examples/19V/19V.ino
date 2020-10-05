#include <Arduino_EdgeControl.h>

constexpr unsigned int adcResolution { 12 };

struct Voltages {
    float volt3V3;
    float volt19V;
};

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
    Power.enable19V();

    Wire.begin();
    Expander.begin();

    Serial.print("Waiting for IO Expander Initialisation...");
    while (!Expander) {
        Serial.print(".");
        delay(100);
    }
    Serial.println(" done.");

    Inputs.begin();
    Inputs.enable();

    analogReadResolution(adcResolution);
}

void loop()
{
    Serial.print("19V Input Channel ");
    Serial.print(": ");

    auto [ voltsMuxer, voltsReference ] = getAverageAnalogRead(INPUTS_19V_REF);

    Serial.print(voltsReference);
    Serial.print(" (");
    Serial.print(voltsMuxer);
    Serial.println(")");
    delay(1000);
}

Voltages getAverageAnalogRead(int pin)  
{
    constexpr size_t loops { 100 };
    constexpr float toV { 3.3f / float { (1 << adcResolution) - 1 } };
    constexpr float rDiv { 0.0616f };

    int tot { 0 };

    for (auto i = 0; i < loops; i++)
        tot += Inputs.analogRead(pin);
    const auto avg = float { tot } * toV / float { loops };

    return { avg, avg / rDiv };
}