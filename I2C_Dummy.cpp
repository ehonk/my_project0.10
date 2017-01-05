//
// Created by Andreas on 28.03.16.
//

#include "I2C_Dummy.h"
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

extern  GPIOHandle gpio_SDA;
extern  GPIOHandle gpio_SCL;


int I2C_Dummy::initGPIO(int sclpin, int sdapin) {
    std::cout << "< Info > I2C_Dummy initGPIO" << std::endl;

    pin_scl = sclpin;
    pin_sda = sdapin;



    // Chip Select Pin initieren
    //GPIOHandle gpio_CS(std::to_string(cspin));
    gpio_SDA.setGPIO(std::to_string(pin_scl));
    gpio_SDA.export_gpio();
    gpio_SDA.setdir_gpio("out");
    gpio_SDA.setval_gpio("0");

    // Clock Initialisieren
    //GPIOHandle gpio_SCLK( std::to_string(sclkpin) );
    gpio_SCL.setGPIO(std::to_string(pin_sda));
    gpio_SCL.export_gpio();
    gpio_SCL.setdir_gpio("out");
    gpio_SCL.setval_gpio("0");



}

int I2C_Dummy::doSomething(float taktrate) {

    std::cout << "< Info > I2C_Dummy doSomething" << std::endl;

    float periodendauer = 1 / taktrate;
    float halfperiod = periodendauer/2 * 1000000;


    int hexZahl = 0x20;

    sendHexValue8Bit(0x20, halfperiod);

    // do etwas takt

    sendHexValue8Bit(0x00, halfperiod);
    sendHexValue8Bit(0x00, halfperiod);

    sendHexValue8Bit(0x20, halfperiod);
    sendHexValue8Bit(0x12, halfperiod);
    sendHexValue8Bit(0xff, halfperiod);
}

int I2C_Dummy::sendHexValue8Bit(int hexZahl, float halfperiod){

    for (int i = 0; i < 8; i++) {
        if ((hexZahl & 0x80) == 0x80) { //0x80 = 128
            // std::cout << " H | ";
            gpio_SDA.setval_gpio("1");
        }
        else {
            // std::cout << " L | ";
            gpio_SDA.setval_gpio("0");
        }


        // Clock generieren
        gpio_SCL.setval_gpio("1");
        usleep(halfperiod);
        gpio_SCL.setval_gpio("0");
        usleep(halfperiod);
        hexZahl <<= 1;
    }

}

int I2C_Dummy::unexport(){
    std::cout << "< Info > I2C_Dummy unexport" << std::endl;

    gpio_SDA.unexport_gpio();
    gpio_SCL.unexport_gpio();


    return 0;
}

int I2C_Dummy::sayHello(){
    std::cout << "< Info > I2C_Dummy started" << std::endl;

    return 0;
}