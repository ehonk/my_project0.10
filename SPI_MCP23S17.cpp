//
// Created by Andreas on 20.03.16.
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

#include "SPI_MCP23S17.h"

using namespace std;

extern  GPIOHandle gpio_CS;
extern  GPIOHandle gpio_SCLK;
extern  GPIOHandle gpioMOSI;
extern  GPIOHandle gpioMISO;


int SPI_MCP23S17::sayHello(){
    std::cout << "< Info > SPI_MCP23S17 started" << std::endl;

    return 0;
}


int SPI_MCP23S17::initGPIO(int sclkpin, int cspin, int misopin, int mosipin) {
    std::cout << "< Info > SPI_MCP23S17 initGPIO" << std::endl;

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

int SPI_MCP23S17::unexport(){
    std::cout << "< Info > SPI_MCP23S17 unexport" << std::endl;

    gpio_CS.unexport_gpio();
    gpio_SCLK.unexport_gpio();
    gpioMOSI.unexport_gpio();
    gpioMISO.unexport_gpio();

    return 0;
}




int SPI_MCP23S17::sendHexValue8Bit(int hexZahl, size_t halfperiod){

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

}

int SPI_MCP23S17::initializeMCP23S17(int sideA, int sideB , float taktrate) {

    // 1 = read, 0 = write

    float periodendauer = 1 / taktrate;
    size_t halfperiod = periodendauer/2 * 1000000;

    std::cout << " initializeMCP23S17 " << std::endl;

    if (sideA==1)
        sendSPI_3x8Bit(SPI_SLAVE_WRITE_ADDR, SPI_IODIRA, 0xFF, halfperiod );

        sendSPI_3x8Bit(SPI_SLAVE_ADDR, SPI_IODIRA, 0x00, halfperiod );

    if (sideB==1)
        sendSPI_3x8Bit(SPI_SLAVE_WRITE_ADDR, SPI_IODIRB, 0xFF, halfperiod );
    else
        sendSPI_3x8Bit(SPI_SLAVE_ADDR, SPI_IODIRB, 0x00, halfperiod );

    std::cout << " Reset" << std::endl;
    if (sideA==1)
        sendSPI_3x8Bit(SPI_SLAVE_ADDR, SPI_GPIOA, 0x00, halfperiod );
    if (sideB==1)
    sendSPI_3x8Bit(SPI_SLAVE_ADDR, SPI_GPIOB, 0x00, halfperiod );

}

int SPI_MCP23S17::getHighLow(float taktrate) {
    std::cout << "< Info > SPI_MCP23S17 getHighLow" << std::endl;

    /*
     *  OpCode | Adresse | Daten
     *  Controlregister | IODIRA oder IODIRB | Ausgänge als in or Out 1 or 0
     *
     *  Initialisierung: SPI_SLAVE_ADDR | SPI_IODIRB | 0 oder 1
     *  Datenschicken:  SPI_SLAVE_ADDR | SPI_GPIOB | Binary 0000 0000  Zustände
     *
     *  Adresse zuerst IODIRA/B , danach GPIOA/B
     *
     *  Beide Seiten gleichzeitig:
     *      std::cout << " Initialisierung" << std::endl;
     *      sendSPI_3x8Bit(SPI_SLAVE_ADDR, SPI_IODIRA, 0x00, halfperiod, Tcss );
     *      sendSPI_3x8Bit(SPI_SLAVE_ADDR, SPI_IODIRB, 0x00, halfperiod, Tcss );

     *      std::cout << " Reset" << std::endl;
     *      sendSPI_3x8Bit(SPI_SLAVE_ADDR, SPI_GPIOA, 0x00, halfperiod, Tcss );
     *      sendSPI_3x8Bit(SPI_SLAVE_ADDR, SPI_GPIOB, 0x00, halfperiod, Tcss );
     */

         // 0100 0000    // Adresse des MCP Chips, 0100 A3 A2 A1 RW


          // 0100 0000

    float periodendauer = 1 / taktrate;
    size_t halfperiod = periodendauer/2 * 1000000;

    std::cout << " Anfrage Read Side A" << std::endl;

    gpio_CS.setval_gpio("0");
    usleep(Tcss);

    sendHexValue8Bit(SPI_SLAVE_WRITE_ADDR, halfperiod);
    sendHexValue8Bit(SPI_GPIOA, halfperiod);

    int mask = 0;
    std::string inputstate;
    mask = 0;

    for (int x = 0; x < 8; x++) {
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

    std::cout << "Read: binar: " << std::bitset<8>(mask)  << " als hex: " << std::hex << mask << std::endl;

    gpio_CS.setval_gpio("1");
    usleep(Tcss);

    usleep(500000);

    return 0;
}

int SPI_MCP23S17::sendSPI_3x8Bit(int OP_hex, int ADDR_hex, int DATA_hex, size_t halfperiod ){

    gpio_CS.setval_gpio("0");
    usleep(Tcss);

    sendHexValue8Bit(OP_hex, halfperiod);
    sendHexValue8Bit(ADDR_hex, halfperiod);
    sendHexValue8Bit(DATA_hex, halfperiod);

    gpio_CS.setval_gpio("1");
    usleep(Tcss);

}

int SPI_MCP23S17::setSPI_Values( std::string side, int hexdata , float taktrate){
        std::cout << "< Info > SPI_MCP23S17 setSPI_Values" << std::endl;


    /*
     *  OpCode | Adresse | Daten
     *  Controlregister | IODIRA oder IODIRB | Ausgänge als in or Out 1 or 0
     *
     *  Initialisierung: SPI_SLAVE_ADDR | SPI_IODIRB | 0 oder 1
     *  Datenschicken:  SPI_SLAVE_ADDR | SPI_GPIOB | Binary 0000 0000  Zustände
     *
     *  Adresse zuerst IODIRA/B , danach GPIOA/B
     *
     *  Beide Seiten gleichzeitig:
     *      std::cout << " Initialisierung" << std::endl;
     *      sendSPI_3x8Bit(SPI_SLAVE_ADDR, SPI_IODIRA, 0x00, halfperiod, Tcss );
     *      sendSPI_3x8Bit(SPI_SLAVE_ADDR, SPI_IODIRB, 0x00, halfperiod, Tcss );

     *      std::cout << " Reset" << std::endl;
     *      sendSPI_3x8Bit(SPI_SLAVE_ADDR, SPI_GPIOA, 0x00, halfperiod, Tcss );
     *      sendSPI_3x8Bit(SPI_SLAVE_ADDR, SPI_GPIOB, 0x00, halfperiod, Tcss );
     */

    float periodendauer = 1 / taktrate;
    size_t halfperiod = periodendauer/2 * 1000000;

    if (side=="A")
        sendSPI_3x8Bit(SPI_SLAVE_ADDR, SPI_GPIOA, hexdata, halfperiod );
    if (side=="B")
        sendSPI_3x8Bit(SPI_SLAVE_ADDR, SPI_GPIOB, hexdata, halfperiod );



    return 0;    
}