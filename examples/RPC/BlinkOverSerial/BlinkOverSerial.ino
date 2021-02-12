/* Sketch to load on the MKR
 *
 *
 * 
    void setup() {
      Serial1.begin(9600);
      pinMode(LED_BUILTIN, OUTPUT);
      digitalWrite(LED_BUILTIN, LOW);

      delay(1000);
    }

    void loop() {
      if (Serial1.available()) {
        auto c = Serial1.read();
        digitalWrite(LED_BUILTIN, c);
      }
    }
*
*
*/

#include <Arduino_EdgeControl.h>

bool led { false };

void setup()
{
    EdgeControl.begin();
    Power.on(PWR_3V3);
    Power.on(PWR_VBAT);
    Power.on(PWR_MKR2);

    // Wait for MKR to power on
    delay(5000);

    SerialMKR2.begin(115200);
    while (!SerialMKR2) {
        delay(500);
    }
}

void loop()
{
    SerialMKR2.write(led);
    led = !led;
    delay(1000);
}