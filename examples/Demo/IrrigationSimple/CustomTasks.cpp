#include "CustomTasks.h"

void backlightOff(bool powerDown)
{
    LCD.noBacklight();
    if (powerDown)
        LCD.clear();
}

void openValve()
{
    Serial.println("Alarm: - Opening Valve");

    LCD.home();
    LCD.backlight();
    LCD.print(getLocaltime());
    LCD.setCursor(0, 1);
    LCD.print("Opening Valve   ");

    // Power on the on-board LED (active low)
    Expander.pinMode(EXP_LED1, OUTPUT);
    Expander.digitalWrite(EXP_LED1, LOW);

    LCD.setCursor(0, 1);
    LCD.print("Valve Open      ");

    // Power off the backlight after 5 seconds
    Alarm.timerOnce(5, [] { backlightOff(false); });
}

void closeValve()
{
    Serial.println("Alarm: - Closing Valve");

    LCD.home();
    LCD.backlight();
    LCD.print(getLocaltime());
    LCD.setCursor(0, 1);
    LCD.print("Closing Valve   ");

    // Power off the on-board LED (active low)
    Expander.pinMode(EXP_LED1, OUTPUT);
    Expander.digitalWrite(EXP_LED1, HIGH);

    LCD.setCursor(0, 1);
    LCD.print("Valve Closed    ");

    // Power off the backlight after 5 seconds
    // and power off everything else
    Alarm.timerOnce(5, [] { backlightOff(true); });
}

void displayClock()
{
    auto printDigits = [](int digits) {
        Serial.print(":");
        if (digits < 10)
            Serial.print('0');
        Serial.print(digits);
    };

    // digital clock display of the time
    Serial.print(hour());
    printDigits(minute());
    printDigits(second());
    Serial.println();
}

void blinkLed()
{
    Expander.pinMode(EXP_LED1, OUTPUT);
    Expander.digitalWrite(EXP_LED1, LOW);
    delay(10);

    // Power off the on-board LED (active low)
    Expander.pinMode(EXP_LED1, OUTPUT);
    Expander.digitalWrite(EXP_LED1, HIGH);
}

void getSensors()
{
    String msg = "Getting Sensors:";
    Serial.println(msg);
    LCD.clear();
    LCD.backlight();
    LCD.setCursor(16,0);
    LCD.autoscroll();
    LCD.print(msg);
    for (auto rem = 16 - msg.length(); rem > 0; rem--)
        LCD.write(' ');
    LCD.noAutoscroll();
    
    Alarm.timerOnce(5, [] { backlightOff(true); });
}


void saveData()
{
    String msg = "Saving Data";
    Serial.println(msg);
    LCD.clear();
    LCD.backlight();
    LCD.setCursor(16,0);
    LCD.autoscroll();
    LCD.print(msg);
    for (auto rem = 16 - msg.length(); rem > 0; rem--)
        LCD.write(' ');
    LCD.noAutoscroll();

    Alarm.timerOnce(5, [] { backlightOff(true); });
}