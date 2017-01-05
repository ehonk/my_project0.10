//
// Created by Andreas on 07.03.16.
//

#ifndef MY_PROJECT_0_5_SPI_MCP3008_H
#define MY_PROJECT_0_5_SPI_MCP3008_H

class SPI_MCP3008 {
private:
    int pin_sclk, pin_cs, pin_mosi, pin_miso;
    float max_volt = 3.3;
    int bit_resolution = 1023; // 10bit
    float voltresolution = max_volt / bit_resolution;
    
public:
    SPI_MCP3008();
    int initGPIO(int sclkpin, int cspin, int misopin, int mosipin);
    float doMeasurement(int channel, float taktrate);
    int doMeasurement_AllChannels(float taktrate);
    int unexport();
    int sayHello();
};

#endif //MY_PROJECT_0_5_SPI_MCP3008_H
