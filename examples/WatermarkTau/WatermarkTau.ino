#include <Arduino.h>
#include <mbed.h>

#include <Arduino_OutdoorCarrier.h>
#include <RunningMedian.h>

constexpr unsigned int adcResolution { 12 };

mbed::LowPowerTimeout TimerM;

uint8_t watermarkChannel { 1 };

constexpr float tauRatio { 0.63f };
constexpr float tauRatioSamples { tauRatio * float { (1 << adcResolution) - 1 } };
constexpr unsigned long sensorDischargeDelay { 2 };

constexpr unsigned int measuresCount { 20 };
RunningMedian measures { measuresCount };

constexpr unsigned int calibsCount { 10 };
RunningMedian calibs { calibsCount };

void setup()
{
    Serial.begin(9600);

    auto startNow = millis() + 2500;
    while (!Serial && millis() < startNow)
        ;
    delay(2000);

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

    Watermarks.begin();

    analogReadResolution(adcResolution);
}

void loop()
{
    static bool highPrec { false };
    Watermarks.setHighPrecision(highPrec);
    highPrec = !highPrec;

    // Init commands and reset devices
    Watermarks.calibrationMode(OUTPUT);
    Watermarks.calibrationWrite(LOW);
    Watermarks.commonMode(OUTPUT);
    Watermarks.commonWrite(LOW);

    Watermarks.fastDischarge(sensorDischargeDelay);

    // Calibration cycle:
    // disable Watermark demuxer
    Watermarks.disable();

    Watermarks.commonMode(INPUT);
    Watermarks.calibrationMode(OUTPUT);
    for (auto i = 0; i < measuresCount; i++) {
        Watermarks.calibrationWrite(HIGH);

        auto start = micros();
        while (Watermarks.analogRead(watermarkChannel) < tauRatioSamples)
            ;
        auto stop = micros();

        Watermarks.calibrationWrite(LOW);

        Watermarks.fastDischarge(sensorDischargeDelay);

        calibs.add(stop - start);
    }

    Serial.print("CALIBS   - Precision: ");
    Serial.print(highPrec ? "High" : "Low ");
    Serial.print(" - Median: ");
    Serial.print(calibs.getMedian());
    Serial.print(" - Average: ");
    Serial.print(calibs.getAverage());
    Serial.print(" - Lowest: ");
    Serial.print(calibs.getLowest());
    Serial.print(" - Highest: ");
    Serial.print(calibs.getHighest());
    Serial.println();

    calibs.clear();

    Watermarks.fastDischarge(sensorDischargeDelay);

    // Measures cycle:
    // enable Watermark demuxer
    Watermarks.enable();

    Watermarks.commonMode(OUTPUT);
    Watermarks.calibrationMode(INPUT);
    for (auto i = 0; i < measuresCount; i++) {
        Watermarks.commonWrite(HIGH);

        auto start = micros();
        while (Watermarks.analogRead(watermarkChannel) < tauRatioSamples)
            ;
        auto stop = micros();

        Watermarks.commonWrite(LOW);

        Watermarks.fastDischarge(sensorDischargeDelay);

        measures.add(stop - start);
    }

    Serial.print("MEASURES - Precision: ");
    Serial.print(highPrec ? "High" : "Low ");
    Serial.print(" - Median: ");
    Serial.print(measures.getMedian());
    Serial.print(" - Average: ");
    Serial.print(measures.getAverage());
    Serial.print(" - Lowest: ");
    Serial.print(measures.getLowest());
    Serial.print(" - Highest: ");
    Serial.print(measures.getHighest());
    Serial.println();

    measures.clear();

    Serial.println();

    delay(1000);
}