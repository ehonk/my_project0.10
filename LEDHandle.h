//
// Created by Andreas on 03.01.16.
//

#ifndef MY_PROJECT_0_1_LEDHANDLE_H
#define MY_PROJECT_0_1_LEDHANDLE_H

class LEDHandle {
public:
    static LEDHandle* getInstance();
    static void release();

    void turnOn(int lednumber);
    void turnOff(int lednumber);
    void turnAllOn();
    void turnAllOff();
    void startup();
    void sayHello();

private:
    LEDHandle();	// Singleton private Konstruktor
    ~LEDHandle();	// Singleton private Destruktor
    static LEDHandle *theInstance;


    const char *LEDBrightness1="/sys/class/leds/led0/brightness";
    const char *LEDBrightness2="/sys/class/leds/led1/brightness";
    // const char *LEDBrightness3="/sys/class/leds/beaglebone:green:usr2/brightness";
    // const char *LEDBrightness4="/sys/class/leds/beaglebone:green:usr3/brightness";
    void switchLEDOn(const char *lednumber);
    void switchLEDOff(const char *lednumber);
};


#endif //MY_PROJECT_0_1_LEDHANDLE_H
