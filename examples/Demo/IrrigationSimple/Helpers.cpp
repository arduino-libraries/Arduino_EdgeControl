#include "Helpers.h"



/**
 * Load alarm task from parsed alarmtab.txt file on SD.
 */
std::list<AlarmTask> loadTasks()
{
    Power.on(PWR_3V3);
    SPI.begin();

    if (!SD.begin(SD_CS)) {
        Serial.println("SD initialization failed");
        while (true)
            ;
    }
    auto alarmtab = SD.open("alarmtab.txt");
    auto taskList = parseAlarmTab(alarmtab);

    alarmtab.close();
    SD.end();
    SPI.end();
    Power.off(PWR_3V3);

    return taskList;
}

/**
 * Set the alarm tasks from the loaded list.
 */
std::list<AlarmID_t> setTasks(std::list<AlarmTask> taskList)
{
    AlarmID_t alarmID { dtINVALID_ALARM_ID };
    std::list<AlarmID_t> alarmIDs;

    for (auto& task : taskList) {
        switch (task.method) {
        case ALARM_REPEAT: {
            if (task.day == 0)
                alarmID = Alarm.alarmRepeat(task.hour, task.min, task.sec, task.handler);
            else
                alarmID = Alarm.alarmRepeat(task.day, task.hour, task.min, task.sec, task.handler);
            break;
        }
        case ALARM_ONCE: {
            if (task.day == 0)
                alarmID = Alarm.alarmOnce(task.hour, task.min, task.sec, task.handler);
            else
                alarmID = Alarm.alarmOnce(task.day, task.hour, task.min, task.sec, task.handler);
            break;
        }
        case TIMER_REPEAT: {
            alarmID = Alarm.timerRepeat(task.sec, task.handler);
            break;
        }
        case TIMER_ONCE: {
            alarmID = Alarm.timerOnce(task.sec, task.handler);
            break;
        }
        case TRIGGER_ONCE: {
            alarmID = Alarm.triggerOnce(task.sec, task.handler);
            break;
        }
        default:
            break;
        }
        alarmIDs.push_back(alarmID);
    }

    return alarmIDs;
}

/**
 * Free the current alarm tasks
 */
void freeTasks(std::list<AlarmID_t>& alarmIDs)
{
    for (auto& id : alarmIDs) {
        Alarm.free(id);
        id = dtINVALID_ALARM_ID;
    }
}

/**
 * Load and set tasks from alarmtab.txt file.
 * 
 * Remove previously loaded tasks if parameter is true.
 */
void loadAndSetTasks(std::list<AlarmID_t>& alarmIDs, bool reload)
{
    LCD.clear();
    LCD.backlight();

    LCD.setCursor(16, 0);
    if (reload) {
        Serial.println("Reloading Tasks: ");
        LCD.print("Reloading Tasks:");
        freeTasks(alarmIDs);
    } else {
        Serial.println("Loading Tasks: ");
        LCD.print("Loading Tasks:");
    }
    
    auto taskList = loadTasks();
    alarmIDs = setTasks(taskList);
    printTasks(taskList);

    LCD.setCursor(16, 1);
    LCD.autoscroll();
    LCD.print(alarmIDs.size());
    LCD.print(" loaded        ");
    LCD.noAutoscroll();

    // Power off the backlight after 5 seconds
    // and power off everything else
    Alarm.timerOnce(5, [] { backlightOff(true); });    
}

void printTasks(const std::list<AlarmTask> taskList)
{
    for (auto const task : taskList) {
        Serial.print("- ");
        switch (task.method) {
        case ALARM_REPEAT:
            Serial.print("ALARM_REPEAT");
            break;
        case ALARM_ONCE:
            Serial.print("ALARM_ONCE");
            break;
        case TIMER_REPEAT:
            Serial.print("TIMER_REPEAT");
            break;
        case TIMER_ONCE:
            Serial.print("TIMER_ONCE");
            break;
        case TRIGGER_ONCE:
            Serial.print("TRIGGER_ONCE");
            break;
        default:
            Serial.print("UNKNOWN");
            break;
        }
        Serial.print(" ");
        Serial.print(task.day);
        Serial.print(" ");
        Serial.print(task.hour);
        Serial.print(" ");
        Serial.print(task.min);
        Serial.print(" ");
        Serial.print(task.sec);
        Serial.print(" ");
        Serial.println((uint64_t)(task.handler), HEX);
    }

    Serial.print(taskList.size());
    Serial.println(" tasks loaded.");
}

/**
 * Set system and TimeAlarm clock from compile datetime or RTC
 */
void setSystemClock(String compileDate, String compileTime)
{
    // Retrieve clock time from compile date...
    auto buildTime = compileDateTimeToSystemTime(compileDate, compileTime, true, 2);
    // ... ore use the one from integrated RTC.
    auto rtcTime = time(NULL);

    // Remeber to connect at least the CR2032 battery
    // to keep at least the RTC running.x
    auto actualTime = rtcTime > buildTime ? rtcTime : buildTime;

    // Set both system time and the alarms one
    set_time(actualTime);
    setTime(actualTime);

    Serial.print("Compile Date and Time: ");
    Serial.println(getLocaltime(buildTime));
    Serial.print("RTC Date and Time:     ");
    Serial.println(getLocaltime(rtcTime));
    Serial.print("System Clock:          ");
    Serial.println(getLocaltime());
}

void statusLCD()
{
    extern std::list<AlarmID_t> alarmTabIDs;
    extern std::list<AlarmID_t> alarmSketchIDs;

    String msg;

    LCD.clear();
    LCD.backlight();
    LCD.home();
    LCD.print("Loaded Tasks...");
    for (auto go = millis() + 1000; millis() < go; yield());
    LCD.setCursor(16,0);
    msg = "Custom: ";
    msg += alarmTabIDs.size();
    LCD.print(msg);
    LCD.autoscroll();
    LCD.setCursor(16, 1);
    msg = "Sketch: ";
    msg += alarmSketchIDs.size();
    LCD.print(msg);
    for (auto rem = 16 - msg.length(); rem > 0; rem--)
        LCD.write(' ');
    LCD.noAutoscroll();

    // Power off the backlight after 5 seconds
    // and power off everything else
    Alarm.timerOnce(5, [] { backlightOff(true); });
}
