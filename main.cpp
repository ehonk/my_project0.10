//============================================================================
// Name        : myBeagleProject.cpp
// Author      : Miky
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include "GPIOHandle.h"
#include "LEDHandle.h"
#include "SPI_MCP3008.h"
#include "SPI_MCP23S17.h"
#include "I2C_Dummy.h"
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <bitset>
#include <climits>
#include "math.h"
#include "stdio.h"
#include <stdlib.h>
#include <vector>
#include <cmath>
#include "i2c8Bit.h"
#include <chrono>



    GPIOHandle gpio_CS;
    GPIOHandle gpio_SCLK;
    GPIOHandle gpioMOSI;
    GPIOHandle gpioMISO;

    GPIOHandle gpio_SDA;
    GPIOHandle gpio_SCL;

    GPIOHandle gpio_DS;
    GPIOHandle gpio_STCP;
    GPIOHandle gpio_SHCP;

    GPIOHandle gpio_Echo;
    GPIOHandle gpio_Trigger;

float max_volt = 3.3;
int bit_resolution = 1023; // 10bit
float voltresolution = max_volt / bit_resolution;
 
using namespace std;

int wuerfel(void);

int bitshifter(void);

float LM75(void);

float HCSR04(void);



int main(){


    //wuerfel();

    //bitshifter();

    //LM75();

    HCSR04();

}


float HCSR04(void){
        std::cout << "< Info > HCSR04" << std::endl;

        // Ein 10 us langer Impuls am Trigger-Pin löst 8 40kHz Ultraschall Impulse aus. 

    int TriggerPin = 23;
    int EchoPin = 24;

    float taktrate = 1;
    float periodendauer = 1 / taktrate;
    size_t halfperiod = periodendauer/2 * 1000000;
    size_t period = periodendauer * 1000000;

    // MOSI Initialisieren
    //GPIOHandle gpioMOSI(std::to_string(mosipin) );
    gpio_Trigger.setGPIO(std::to_string(TriggerPin));    
    gpio_Trigger.export_gpio();
    gpio_Trigger.setdir_gpio("out");
    gpio_Trigger.setval_gpio("0");

    // MISO Initialisieren
    //GPIOHandle gpioMISO(std::to_string(misopin) );
    gpio_Echo.setGPIO(std::to_string(EchoPin));    
    gpio_Echo.export_gpio();
    gpio_Echo.setdir_gpio("in");

    std::chrono::time_point<std::chrono::system_clock> start, end;
        for (int i = 0; i < 1000; i++) {

            // Trigger generieren
            gpio_Trigger.setval_gpio("1");
            //std::cout << "< Info > HCSR04 gpio_Trigger HIGH" << std::endl;
            usleep( 1000 );
            start = std::chrono::system_clock::now();
            gpio_Trigger.setval_gpio("0");
            //std::cout << "< Info > HCSR04 gpio_Trigger LOW" << std::endl;
      

        // bessere Flankenerkennung einbauen
        
        string inputstate;
        gpio_Echo.getval_gpio(inputstate);
        while (inputstate=="0")
        {
            gpio_Echo.getval_gpio(inputstate);
            start = std::chrono::system_clock::now();

        }


        do
        {
            end = std::chrono::system_clock::now();
            
            gpio_Echo.getval_gpio(inputstate);

        }
        while (inputstate=="1");

        std::chrono::duration<double> elapsed_seconds = end-start;
        std::time_t end_time = std::chrono::system_clock::to_time_t(end);
        double distanz = elapsed_seconds.count();
        distanz = (distanz * 34300) / 2 ;
        std::cout << "elapsed time: " << elapsed_seconds.count() << " distanz: " << distanz  << "cm" << "\n";

            usleep(1000000);

        }
    return 0;
}


int i2c_dum(void)
{
    float taktrate = 1000; // Herz  1 Mhz = 1 000 000 Hz

    //I2C Dummy MCP23017
// SCL Pin 6, SDA Pin = 5

    I2C_Dummy myi2c;
    myi2c.initGPIO(6,5);

    myi2c.doSomething(taktrate);

    myi2c.unexport();


}

int bitshifter(void) {

/*
 *  74HC595
 *  DS Braun = 17
 *  STCP Geld = 27
 *  SHCP Orange = 22;
 */

    cout << "bitshifter starts" << endl;

    float taktrate = 50;

    int DSpin = 17;
    int STCPpin = 27;
    int SHCPpin = 22;

    float actual_voltage = 0;
    string inputstate;

    int mosi_hexZahl = 0xFF;
    int hexZahl = mosi_hexZahl;
    float periodendauer = 1 / taktrate;
    int mask = 0;
    int treshold = 2;
    int prev_mask = 0;

    size_t halfperiod = periodendauer/2 * 1000000;
    size_t t_sucs = 0; // Minimale Wartezeit nach Chip Select High 100 ns = 0,1 Mikrosekunden;  = 0,0001, Milisekunden
    size_t t_csh = 0; // Minimale Chip Select High Zeit


    // Chip Select Pin initieren
    //GPIOHandle gpio_CS(std::to_string(cspin));
    gpio_DS.setGPIO(std::to_string(DSpin));
    gpio_DS.export_gpio();
    gpio_DS.setdir_gpio("out");
    gpio_DS.setval_gpio("0");

    // Clock Initialisieren
    //GPIOHandle gpio_SCLK( std::to_string(sclkpin) );
    gpio_STCP.setGPIO(std::to_string(STCPpin));
    gpio_STCP.export_gpio();
    gpio_STCP.setdir_gpio("out");
    gpio_STCP.setval_gpio("0");

    // MOSI Initialisieren
    //GPIOHandle gpioMOSI(std::to_string(mosipin) );
    gpio_SHCP.setGPIO(std::to_string(SHCPpin));
    gpio_SHCP.export_gpio();
    gpio_SHCP.setdir_gpio("out");
    gpio_SHCP.setval_gpio("0");


    gpio_SHCP.setval_gpio("0");
    usleep(halfperiod);
    gpio_SHCP.setval_gpio("1");
    usleep(halfperiod);

    // generate clock
    // SHCP ist die Clock


    for (int i = 0; i < 3; i++) {

        hexZahl = mosi_hexZahl;

        // DS einmal high low
        gpio_SHCP.setval_gpio("0");
        gpio_DS.setval_gpio("1");
        usleep(halfperiod);
        gpio_SHCP.setval_gpio("1");
        gpio_DS.setval_gpio("1");
        usleep(halfperiod);

    for (int i = 0; i < 8; i++) {

        gpio_DS.setval_gpio("0");

        if ((hexZahl & 0x80) == 0x80) { //0x80 = 128
            // std::cout << " H | ";
            gpio_STCP.setval_gpio("1");
        }
        else {
            // std::cout << " L | ";
            gpio_STCP.setval_gpio("0");
        }



        // Clock generieren
        gpio_SHCP.setval_gpio("0");
        usleep(halfperiod);

        gpio_STCP.setval_gpio("0");
        gpio_SHCP.setval_gpio("1");
        usleep(halfperiod);
        hexZahl <<= 1;
    }
    }

    cout << "bitshifter end" << endl;
    return 0;
}

int mcp3008_control(){

    /*
    SPI_MCP3008 spi_mcp;
    spi_mcp.initGPIO(18, 25, 23, 24);

    for (int i = 0; i < 1; i++) {
        float Ch0_Voltage = spi_mcp.setSPI_Values(0, taktrate);
        std::cout << "Messung Channel0: " << Ch0_Voltage  << std::endl;
        //spi_mcp.doMeasurement_AllChannels(taktrate);
    }
    spi_mcp.unexport();

*/
}

int MCP23S17_control(){



    // MCP23S17
    /*
     *
     * MCP 23S17
     * SCLK GPIO26
     * MISO GPIO16
     * MOSI GPIO20
     * CS GPIO21
     */



    /*
    std::vector<unsigned char> hexvector {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A ,0x0B ,0x0C, 0x0D, 0x0E, 0x0F};

    SPI_MCP23S17 spi_mcpexpander;
    spi_mcpexpander.sayHello();
    spi_mcpexpander.initGPIO(26, 21, 16, 20);


    // Read Values
    // 1 = read, 0 = write

    spi_mcpexpander.initializeMCP23S17(0, 0, taktrate);


    for(unsigned int i = 0; i < hexvector.size(); ++i)
    {
        spi_mcpexpander.setSPI_Values("B", hexvector[i], taktrate);
        usleep(500000);
        spi_mcpexpander.setSPI_Values("B", 0x00, taktrate);

    }


    spi_mcpexpander.unexport();
*/
}

float LM75(void){


    cout << "LM75: "  << std::endl;
    float temp=0;
    bool go = true;
    i2c8Bit mcp23017(0x48, string("/dev/i2c-1")); // Instanzierung: specify a device address 0x20 and i2c device "/dev/i2c-1"
    unsigned char pinStatus = 0;

    while(go) // repeat the following 20 times and then exit;
    {
        mcp23017.readReg(0x00, pinStatus);
        //std::cout << " i2c binary: " << std::bitset<8>(pinStatus)  << " | hex: " << std::hex << static_cast<unsigned short>(pinStatus) << " | dec: " << std::dec << static_cast<unsigned short>(pinStatus) << std::endl;
        temp = static_cast<unsigned short>(pinStatus);
        cout << "Temperatur:" << temp << endl;
        sleep(5);
    }
    return temp;
}

int wuerfel(void)
{
    int augen = 0;
    bool go = true;
    unsigned char pinStatus = 0;
    unsigned char outToggleState = 0;

    std::vector<unsigned char> hexvector {0x00, 0x08, 0x41, 0x49, 0x55, 0x5d, 0x77};

    i2c8Bit mcp23017(0x20,string("/dev/i2c-1")); // Instanzierung: specify a device address 0x20 and i2c device "/dev/i2c-1"

    mcp23017.writeReg(0x00,0x00);     // register 00 (IODIRA) alles Ausgänge
    mcp23017.writeReg(0x01,0xff);   // register 01 (IODIRB) alles Eingänge

    while(go) // repeat the following 20 times and then exit;
    {
        augen = std::rand() % 6 +1;
        //cout << "zufalszahl: " << augen << std::endl;
        mcp23017.writeReg(0x12, hexvector[augen] );

        mcp23017.readReg(0x13,pinStatus);
        std::cout << " i2c binary: " << std::bitset<8>(pinStatus)  << " | hex: " << std::hex << static_cast<unsigned short>(pinStatus) << std::dec << std::endl;

        if( (pinStatus & 0x01) == 1){   // if Pin GPB1 = activ
            cout << "PushButton pressed..toggling LED" << endl;
            outToggleState ^= 1;
            go=false;
        }
        usleep(100000);
    }
    cout << "exiting" << endl;
    return 0;
}

int oCinitGPIO(int sclkpin, int cspin, int misopin, int mosipin) {
    std::cout << "< Info > initGPIO" << std::endl;

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

    return 0;
}

float oCdoMeasurement(int channel, float taktrate) {
    //std::cout << "< Info > oCdoMeasurement Channel." << channel << std::endl;


    // Startcmd für MCP ist 0001 1000 = 0x18 /Channel 0
    // Channel 1 für MCP ist 0001 1001 = 0x19 /Channel 0

    float actual_voltage = 0;
    string inputstate;

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

        for (int x = 0; x < 11; x++) {
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
    return actual_voltage;
}

int oCunexport(){
    std::cout << "< Info > unexport" << std::endl;

    gpio_CS.unexport_gpio();
    gpio_SCLK.unexport_gpio();
    gpioMOSI.unexport_gpio();
    gpioMISO.unexport_gpio();

    return 0;
}

// NO MORE CLASS DEFINITION
