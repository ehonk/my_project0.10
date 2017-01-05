//
// Created by Andreas on 07.03.16.
//

#include "SPI_MCP3008.h"
#include <iostream>
#include "GPIOHandle.h"
#include <stdio.h>
#include <unistd.h>
#include <bitset>
#include <climits>
#include "math.h"
#include "stdio.h"
#include <stdlib.h>
#include <cmath>

using namespace std;

extern  GPIOHandle gpio_CS;
extern  GPIOHandle gpio_SCLK;
extern  GPIOHandle gpioMOSI;
extern  GPIOHandle gpioMISO;


int SPI_MCP3008::initGPIO(int sclkpin, int cspin, int misopin, int mosipin) {
    std::cout << "< Info > SPI_MCP3008 initGPIO" << std::endl;

    pin_sclk = sclkpin;
    pin_cs = cspin;
    pin_mosi = mosipin;
    pin_miso = misopin;

  
        // Chip Select Pin initieren
        //GPIOHandle gpio_CS(std::to_string(cspin));
        gpio_CS.setGPIO(std::to_string(cspin));
        gpio_CS.export_gpio();
        gpio_CS.setdir_gpio("out");
        gpio_CS.setval_gpio("1");

        // Clock Initialisieren
        //GPIOHandle gpio_SCLK( std::to_string(sclkpin) );
        gpio_SCLK.setGPIO(std::to_string(sclkpin));
        gpio_SCLK.export_gpio();
        gpio_SCLK.setdir_gpio("out");
        gpio_SCLK.setval_gpio("0");

        // MOSI Initialisieren
        //GPIOHandle gpioMOSI(std::to_string(mosipin) );
        gpioMOSI.setGPIO(std::to_string(mosipin));
        gpioMOSI.export_gpio();
        gpioMOSI.setdir_gpio("out");
        gpioMOSI.setval_gpio("0");

        // MISO Initialisieren
        //GPIOHandle gpioMISO(std::to_string(misopin) );
        gpioMISO.setGPIO(std::to_string(misopin));
        gpioMISO.export_gpio();
        gpioMISO.setdir_gpio("in");

}


int SPI_MCP3008::sayHello(){
    std::cout << "< Info > SPI_MCP3008 started" << std::endl;
    
    return 0;
}

SPI_MCP3008::SPI_MCP3008() {
    std::cout << "< Info > SPI_MCP3008 Konstruktor" << std::endl;

    //sGPIO="0";
    //iGPIO=0;

}


float SPI_MCP3008::doMeasurement(int channel, float taktrate){

    // std::cout << "< Info > SPI_MCP3008 setSPI_Values" << std::endl;


    float actual_voltage = 0;
    std::string inputstate;

    int mosi_hexZahl = 0x18 + channel;  
    int hexZahl = mosi_hexZahl;
    float periodendauer = 1 / taktrate;
    int mask = 0;
    int treshold = 2;
    int prev_mask = 0;

    size_t halfperiod = periodendauer/2 * 1000000;
    size_t t_sucs = 0; // Minimale Wartezeit nach Chip Select High 100 ns = 0,1 Mikrosekunden;  = 0,0001, Milisekunden
    size_t t_csh = 0; // Minimale Chip Select High Zeit


        gpio_CS.setval_gpio("0");
        usleep(periodendauer);

        hexZahl = mosi_hexZahl;
        for (int i = 0; i < 8; i++) {
            if ((hexZahl & 0x80) == 0x80) { //0x80 = 128
                // std::cout << " H | ";
                gpioMOSI.setval_gpio("1");
            }
            else {
                // std::cout << " L | ";
                gpioMOSI.setval_gpio("0");
            }

            // Clock generieren
            gpio_SCLK.setval_gpio("1");
            usleep(halfperiod);
            gpio_SCLK.setval_gpio("0");
            usleep(halfperiod);
            hexZahl <<= 1;
        }
        // Bitauslen
        mask = 0;

        for (int x = 0; x < 12; x++) {
            mask <<= 1;
            gpio_SCLK.setval_gpio("1");   // Clock Rais genererieren
            gpioMISO.getval_gpio(inputstate);

            if (inputstate=="1") {
                //std::cout << x << ".Generate 1" << std::endl;
                mask |= 0x01;
                //mask |= 0x01 << x;
            } else {
                //std::cout << x << ".Generate 0" << std::endl;
                mask |= 0x00;
                //mask |= 0x00 << x;
            }
            usleep(halfperiod);
            gpio_SCLK.setval_gpio("0");
            usleep(halfperiod);
        }

        actual_voltage = mask * voltresolution;

        if ( (abs(mask*1 - prev_mask*1)) > treshold)
        {
            //std::cout << "Final decimal: " << std::dec << mask;
            //std::cout << " | Voltage: " << actual_voltage << " V " << std::endl;
        }
        else {
            //std::cout << "Final decimal: " << std::dec << mask;
            //std::cout << " | Voltage: " << actual_voltage << " V " << " | treshold: " << treshold << std::endl;

        }
        prev_mask = mask;
        gpio_CS.setval_gpio("1");
        usleep(periodendauer);

    std::cout << "Debug: Volt: " << actual_voltage << " | Bit: " << mask << " | Taktrate: " << taktrate << " Hz | Periodendauer: " << periodendauer << " sec | Halfperiod: " << halfperiod / 1000000 << " sec | Hexwert:" << std::hex << mask << std::endl;
    //std::cout << "Perioden sec: " << periodendauer << " sec | " << periodendauer*1000 << " msec | " << periodendauer*1000000 << " ysec " << std::endl;

    return actual_voltage;

}

int SPI_MCP3008::doMeasurement_AllChannels(float taktrate) {
    //std::cout << "< Info > SPI_MCP3008 doMeasurement_AllChannels" << std::endl;

    float Ch0_Voltage = doMeasurement(0, taktrate);
    float Ch1_Voltage = doMeasurement(1, taktrate);
    float Ch2_Voltage = doMeasurement(2, taktrate);
    float Ch3_Voltage = doMeasurement(3, taktrate);
    float Ch4_Voltage = doMeasurement(4, taktrate);
    float Ch5_Voltage = doMeasurement(5, taktrate);
    float Ch6_Voltage = doMeasurement(6, taktrate);
    float Ch7_Voltage = doMeasurement(7, taktrate);

    std::cout << "Ch 0: " << Ch0_Voltage << " | Ch 1: " << Ch1_Voltage;
    std::cout << "Ch 2: " << Ch2_Voltage << " | Ch 3: " << Ch3_Voltage;
    std::cout << "Ch 4: " << Ch4_Voltage << " | Ch 5: " << Ch5_Voltage;
    std::cout << "Ch 6: " << Ch6_Voltage << " | Ch 7: " << Ch7_Voltage << std::endl;

    return 0;
}
int SPI_MCP3008::unexport(){
    std::cout << "< Info > SPI_MCP3008 unexport" << std::endl;

    gpio_CS.unexport_gpio();
    gpio_SCLK.unexport_gpio();
    gpioMOSI.unexport_gpio();
    gpioMISO.unexport_gpio();

    return 0;
}