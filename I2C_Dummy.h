//
// Created by Andreas on 28.03.16.
//

#ifndef MY_PROJECT_0_7_I2C_DUMMY_H
#define MY_PROJECT_0_7_I2C_DUMMY_H


class I2C_Dummy {
private:
    int pin_scl, pin_sda;
    float max_volt = 3.3;
    int bit_resolution = 1023; // 10bit
    float voltresolution = max_volt / bit_resolution;
public:
    int initGPIO(int sclpin, int sdapin);
    float doMeasurement(int channel, float taktrate);
    int doSomething(float taktrate);
    int doMeasurement_AllChannels(float taktrate);
    int sendHexValue8Bit(int hexZahl, float halfperiod);
    int unexport();
    int sayHello();
};



#endif //MY_PROJECT_0_7_I2C_DUMMY_H
