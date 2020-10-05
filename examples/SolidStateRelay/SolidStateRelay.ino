#include "Arduino_EdgeControl.h"

#define SSR_POLL

constexpr unsigned long onInterval = { 5000 };
constexpr unsigned long offInterval = { 5000 };
constexpr unsigned long pollInterval = { 1000 };
unsigned long offTime;
unsigned long onTime;
unsigned long pollTime;
bool on = false;

int relayChannel { RELAYS_CH04 };

void setup()
{
    Serial.begin(9600);
    while (!Serial)
        ;

    delay(2000);

    Serial.println("Hello, SolidStateRelay!");
    
    Power.enable3V3();
    Power.enable5V();

    Wire.begin();
    Serial.print("Waiting for IO Expander Initialisation...");
    while (!Expander) {
        Serial.print(".");
        delay(100);
    }
    Serial.println(" done.");
    Expander.pinMode(EXP_LED1, OUTPUT);

    for (auto i = 0; i < 3; i++) {
        Expander.digitalWrite(EXP_LED1, LOW);
        delay(50);
        Expander.digitalWrite(EXP_LED1, HIGH);
        delay(100);
    }

    Relays.begin();
}

void loop()
{
    if (millis() > onTime && !on) {
        Serial.println("RELAY ON");

        Relays.on(relayChannel);

        Expander.digitalWrite(EXP_LED1, LOW);

        on = true;
        offTime = onInterval + millis();
    }

    if (millis() > offTime && on) {
        Serial.println("RELAY OFF");

        Relays.off(relayChannel);

        Expander.digitalWrite(EXP_LED1, HIGH);

        on = false;
        onTime = millis() + offInterval;
    }

#if defined(SSR_POLL)
    if (millis() > pollTime && on) {
        Serial.println("POLLING");

        Relays.poll(relayChannel);

        pollTime = millis() + pollInterval;
    }
#endif
}