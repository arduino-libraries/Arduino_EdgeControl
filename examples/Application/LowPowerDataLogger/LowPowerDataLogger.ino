#include <Arduino_EdgeControl.h>
#include <FATFilesystem.h>
#include <SDBlockDevice.h>
#include <SPIFBlockDevice.h>
#include <TDBStore.h>
#include <chrono>

// arduino-cli compile -b arduino-git:mbed:edge_control --build-property 'compiler.cpp.extra_flags="-DDEBUG=1"'
#include "DebugMode.h"
#include "Helpers.h"

using namespace mbed;
using namespace events;
using namespace rtos;
using namespace std::chrono_literals;

// Low Power Tickers for timed actions
LowPowerTicker meter;
LowPowerTicker printer;

// Event queue for managing IRQ-Unsafe tasks
EventQueue queue(32 * EVENTS_EVENT_SIZE);
Thread t;

// Serial Flash management
constexpr uint32_t SPIF_FREQ { 32000000 };
SPIFBlockDevice bd(SPI_MOSI, SPI_MISO, SPI_SCK, SPI_SS, SPIF_FREQ);
TDBStore tdb_store(&bd);

// Use a Mutex to coordinate reads and writes to SPIF storage
Mutex spifMutex;

/** UI Management **/
// Button statuses
enum ButtonStatus : byte {
    ZERO_TAP,
    SINGLE_TAP,
    DOUBLE_TAP,
    LOT_OF_TAPS
};

// ISR: count the button taps
volatile byte taps { 0 };
// ISR: keep elapsed timings
volatile unsigned long previousPress { 0 };
// ISR: Final button status
volatile ButtonStatus buttonStatus { ZERO_TAP };

// Struct for data storage
struct Data {
    time_t ts;
    uint32_t moist;
};

void setup()
{
    if constexpr (debugMode) {
        Serial.begin(115200);
        const uint32_t startNow { millis() + 2500 };
        while (!Serial && millis() < startNow)
            ;
        delay(500);
    }

    EdgeControl.begin();

    // Init system clock from compilation  time
    setSystemClock(__DATE__, __TIME__);

    Power.on(PWR_3V3);

    pinMode(POWER_ON, INPUT);
    attachInterrupt(POWER_ON, buttonPress, RISING);

    // Init the Key/Value Store
    DebugSerial.println("Init TinyDB Key Value store");
    auto err = tdb_store.init();
    DebugSerial.println("TDB Init " + String(err == 0 ? "OK" : "KO") + " (" + String(err) + ")");

    // Initilizing the SD Card and the FAT filsystem function-wide
    // helps managing SD Card extraction and insertion at run-time
    SDBlockDevice sd(SD_MOSI, SD_MISO, SD_CLK, SD_CS);
    FATFileSystem fat("fat");
    DebugSerial.print("Checking the SD Card");
    auto ret = fat.mount(&sd);
    if (ret != MBED_SUCCESS) {
        DebugSerial.println(": Error");
        DebugSerial.println("Please, check your SD Card.");
        while (true);
    }
    DebugSerial.println(": Ok");
    fat.unmount();

    // Start the Event Queue manager
    t.start(callback(&queue, &EventQueue::dispatch_forever));

    // Init the stats printer and metering tasks
    printer.attach(printStatsISR, 30s);
    meter.attach(readSensorsISR, 10s);
}

void loop()
{
    // Use loop() just to manage UI

    detectTaps();

    switch (buttonStatus) {
    case ZERO_TAP: {
        break;
    }
    case SINGLE_TAP: {
        DebugSerial.println("Single Tap");
        printStats();
        buttonStatus = ZERO_TAP;
        break;
    }
    case DOUBLE_TAP: {
        DebugSerial.println("Double Tap");
        storeData();
        buttonStatus = ZERO_TAP;
        break;
    }
    default: {
        DebugSerial.println("Too Many Taps");
        buttonStatus = ZERO_TAP;
        break;
    }
    }
}

void printStatsISR()
{
    // Do something IRQ safe
    // ... and distpatch IRQ-unsafe execution to Event Queue
    queue.call(printStats);
}

void printStats()
{
    powerOn();
    DebugSerial.begin(115200);

    Expander.pinMode(EXP_LED1, OUTPUT);
    Expander.digitalWrite(EXP_LED1, LOW);
    delay(100);
    Expander.digitalWrite(EXP_LED1, HIGH);
    delay(100);
    Expander.digitalWrite(EXP_LED1, LOW);
    delay(100);
    Expander.digitalWrite(EXP_LED1, HIGH);

    if constexpr (!debugMode)
        return;

    spifMutex.lock();

    // Use a TDBStore iterator to retrieve all the keys
    TDBStore::iterator_t it;
    TDBStore::info_t info;
    // Iterate over all the keys starting with name "key_"
    auto ret = tdb_store.iterator_open(&it, "key_");
    if (ret != MBED_SUCCESS) {
        DebugSerial.println("Error opening the iterator");
        spifMutex.unlock();
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
        DebugSerial.print(msg);

        // Get the value using parameters from the info retrieved
        Data out;
        size_t actual_size;
        tdb_store.get(key, &out, info.size, &actual_size);

        // Do something useful with the key-value pair...
        msg = "Value (";
        msg += actual_size;
        msg += ") - { ts: ";
        msg += static_cast<uint32_t>(out.ts);
        msg += ", moist: ";
        msg += out.moist;
        msg += " }";
        DebugSerial.print(msg);
        DebugSerial.println();
    }
    // Close the iterator at the end of the cycle
    tdb_store.iterator_close(it);
    spifMutex.unlock();

    Serial.end();
    powerOff();
}

void readSensorsISR()
{
    // Do something IRQ safe
    // ... and distpatch IRQ-unsafe execution to Event Queue
    queue.call(readSensors);
}

void readSensors()
{
    powerOn();

    DebugSerial.begin(115200);
    DebugSerial.println("User Context");

    Expander.pinMode(EXP_LED1, OUTPUT);
    Expander.digitalWrite(EXP_LED1, LOW);
    delay(50);
    Expander.digitalWrite(EXP_LED1, HIGH);

    auto value = getAverageInputsRead(INPUTS_05V_CH01, 10);

    String key = "key_";
    auto ts = time(nullptr);
    key += static_cast<uint32_t>(ts);

    Data data { ts, value };
    spifMutex.lock();
    auto res = tdb_store.set(key.c_str(), reinterpret_cast<void*>(&data), sizeof(data), 0);
    spifMutex.unlock();
    if (res == MBED_SUCCESS)
        DebugSerial.println(key + ": " + String(value));

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

void buttonPress()
{
    const auto now = millis();
    // Poor-man debouncing
    if (now - previousPress > 100)
        taps++;

    previousPress = now;
}

void detectTaps()
{
    // Timeout to validate the button taps counter
    constexpr unsigned int buttonTapsTimeout { 300 };

    // Set the button status and reset the taps counter when button has been
    // pressed at least once and button taps validation timeout has been reached.
    if (taps > 0 && millis() - previousPress >= buttonTapsTimeout) {
        buttonStatus = static_cast<ButtonStatus>(taps);
        taps = 0;
    }
}

void storeData()
{
    powerOn();
    DebugSerial.begin(115200);
    DebugSerial.println("Saving Data on SD Card");

    Expander.pinMode(EXP_LED1, OUTPUT);

    SDBlockDevice sd(SD_MOSI, SD_MISO, SD_CLK, SD_CS);
    FATFileSystem fat("fat");
    DebugSerial.print("Mounting the SD Card");
    auto ret = fat.mount(&sd);
    if (ret != MBED_SUCCESS) {
        DebugSerial.println(": Error");
        return;
    }
    DebugSerial.println(": Ok");

    DebugSerial.print("Opening the log file");
    auto f = fopen("/fat/datalog.csv", "a+");
    if (f == nullptr) {
        DebugSerial.println(": Error");
        return;
    }
    DebugSerial.println(": Ok");

    spifMutex.lock();
    TDBStore::iterator_t it;
    ret = tdb_store.iterator_open(&it, "key_");
    if (ret != MBED_SUCCESS) {
        DebugSerial.println("Error opening the iterator");
        spifMutex.unlock();
        return;
    }

    unsigned int counter { 0 };
    TDBStore::info_t info;
    char key[128] { 0 };
    while (tdb_store.iterator_next(it, key, sizeof(key)) != MBED_ERROR_ITEM_NOT_FOUND) {
        DebugSerial.print("Saving ");
        DebugSerial.print(key);
        ret = tdb_store.get_info(key, &info);
        if (ret != MBED_SUCCESS) {
            DebugSerial.println(": error getting the info");
            continue;
        }

        Data out;
        size_t actual_size;
        tdb_store.get(key, &out, info.size, &actual_size);
        if (ret != MBED_SUCCESS) {
            DebugSerial.println(": error getting the keyvalue");
            continue;
        }

        String line;
        line += static_cast<uint32_t>(out.ts);
        line += ",";
        line += out.moist;

        DebugSerial.print(" (");
        DebugSerial.print(line);
        DebugSerial.print(") ");

        line += "\r\n";
        ret = fputs(line.c_str(), f);
        if (ret < 0) {
            DebugSerial.print(": saving error: ");
            DebugSerial.print(strerror(errno));
            DebugSerial.print(" ");
            DebugSerial.println(-errno);
            continue;
        }
        ret = fflush(f);
        if (ret != 0) {
            DebugSerial.print(": flushing error: ");
            DebugSerial.print(strerror(errno));
            DebugSerial.print(" ");
            DebugSerial.println(-errno);
            continue;
        }

        ret = tdb_store.remove(key);
        if (ret != MBED_SUCCESS) {
            DebugSerial.println(": error removing.");
        }
        DebugSerial.println(" Removed.");
        counter++;
        Expander.digitalWrite(EXP_LED1, LOW);
        delay(25);
        Expander.digitalWrite(EXP_LED1, HIGH);
        delay(50);
    }
    DebugSerial.print(counter);
    DebugSerial.println(" keys found.");

    tdb_store.iterator_close(it);
    spifMutex.unlock();
    fclose(f);
    fat.unmount();
    DebugSerial.end();

    Expander.digitalWrite(EXP_LED1, LOW);
    delay(500);
    Expander.digitalWrite(EXP_LED1, HIGH);
    // delay(50);

    powerOff();
}