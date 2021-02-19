#include <Arduino_EdgeControl.h>
#include <SPIFBlockDevice.h>
#include <TDBStore.h>
#include <chrono>

#include "Helpers.h"

using namespace mbed;
using namespace events;
using namespace rtos;
using namespace std::chrono_literals;

LowPowerTicker meter;
LowPowerTicker printer;
EventQueue queue(32 * EVENTS_EVENT_SIZE);
Thread t;

constexpr uint32_t SPIF_FREQ { 32000000 };

SPIFBlockDevice bd(SPI_MOSI, SPI_MISO, SPI_SCK, SPI_SS, SPIF_FREQ);
TDBStore tdb_store(&bd);

struct Data {
    time_t ts;
    uint32_t moist;
};

void setup()
{
    Serial.begin(115200);
    while (!Serial)
        ;
    delay(500);

    EdgeControl.begin();

    setSystemClock(__DATE__, __TIME__);

    Power.on(PWR_3V3);

    Serial.println("Init TinyDB Key Value store");
    auto err = tdb_store.init();
    Serial.println("TDB Init " + String(err == 0 ? "OK" : "KO") + " (" + String(err) + ")");

    t.start(callback(&queue, &EventQueue::dispatch_forever));

    printer.attach(printStats, 30s);
    meter.attach(readSensors, 10s);
}

void loop()
{
}

void printStats()
{
    // Do something IRQ safe
    // ... and distpatch IRQ-unsafe execution to Event Queue
    queue.call(printStatsUserContext);
}

void printStatsUserContext()
{
    powerOn();

    Serial.begin(115200);
    Serial.println(millis());

    // Use a TDBStore iterator to retrieve all the keys
    TDBStore::iterator_t it;
    TDBStore::info_t info;
    size_t actual_size;

    // Iterate over all the keys starting with name "key_"
    auto ret = tdb_store.iterator_open(&it, "key_");
    if (ret != MBED_SUCCESS) {
        Serial.println("Error opening the iterator");
        return;
    }

    char key[128] { 0 };
    while (tdb_store.iterator_next(it, key, sizeof(key)) != MBED_ERROR_ITEM_NOT_FOUND) {
        // Get info about the key and its contents
        tdb_store.get_info(key, &info);

        String msg = "Key: ";
        msg += String(key, 16);
        msg += " - Size: ";
        msg += info.size;
        msg += " - ";
        Serial.print(msg);

        // Get the value using parameters from the info retrieved
        Data out;
        tdb_store.get(key, &out, info.size, &actual_size);

        // Do something useful with the key-value pair...
        msg = "Value (";
        msg += actual_size;
        msg += ") - { ts: ";
        msg += static_cast<uint32_t>(out.ts);
        msg += ", moist: ";
        msg += out.moist;
        msg += " }";
        Serial.print(msg);

        // .. then (optionally) remove the key
        tdb_store.remove(key);
        Serial.println(" Removed.");
    }
    // Close the iterator at the end of the cycle
    tdb_store.iterator_close(it);

    Serial.end();
    powerOff();
}

void readSensors()
{
    // Do something IRQ safe
    // ... and distpatch IRQ-unsafe execution to Event Queue
    queue.call(readSensorsUserContext);
}

void readSensorsUserContext()
{
    Serial.begin(115200);
    Serial.println("User Context");

    powerOn();

    auto value = getAverageInputsRead(INPUTS_05V_CH01, 10);

    String key = "key_";
    auto ts = time(nullptr);
    key += static_cast<uint32_t>(ts);

    Data data { ts, value };
    // auto res = tdb_store.set(key.c_str(), &value, sizeof(value), 0);
    auto res = tdb_store.set(key.c_str(), reinterpret_cast<void*>(&data), sizeof(data), 0);
    if (res == MBED_SUCCESS)
        Serial.println(key + ": " + String(value));

    Serial.end();

    powerOff();
}

void powerOn()
{
    Power.on(PWR_VBAT);
    Power.on(PWR_3V3);
    Wire.begin();
    delay(500);
    Expander.begin();
    Inputs.begin();
}

void powerOff()
{
    Inputs.end();
    Expander.end();
    Wire.end();
    Power.off(PWR_3V3);
    Power.off(PWR_VBAT);
}

int getAverageInputsRead(int pin, const size_t loops)
{
    unsigned int tot { 0 };

    analogReadResolution(ADC_RESOLUTION);

    Inputs.enable();
    for (auto i = 0; i < loops; i++)
        tot += Inputs.analogRead(pin);
    Inputs.disable();

    return tot / loops;
}