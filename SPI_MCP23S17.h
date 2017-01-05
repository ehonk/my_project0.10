//
// Created by Andreas on 20.03.16.
//

#ifndef MY_PROJECT_0_7_SPI_MCP23S17_H
#define MY_PROJECT_0_7_SPI_MCP23S17_H


class SPI_MCP23S17 {
private:
    int pin_sclk, pin_cs, pin_mosi, pin_miso;
    float max_volt = 3.3;
    int bit_resolution = 1023; // 10bit
    float voltresolution = max_volt / bit_resolution;

    int SPI_SLAVE_ADDR = 0x40;
    int SPI_SLAVE_WRITE_ADDR = 0x41;
    int SPI_IOCTRL = 0x0A;          // 0000 1010
    int SPI_IODIRA = 0x00;          // 0000 0000    //Adressierung der einzelnen A-Pins 1 = Read, 0 Write
    int SPI_IODIRB = 0x01;          // 0000 0001    //Adressierung der einzelnen B-Pins 1 = Read, 0 Write
    int SPI_GPIOA = 0x12;           // 0001 0010
    int SPI_GPIOB = 0x13;           // 0001 0011
    float Tcss = 5000; // min 5 ns


public:
    int sayHello();
    int setSPI_Values( std::string side, int hexdata, float taktrate);
    int getHighLow(float taktrate);
    int sendHexValue8Bit(int hexZahl, size_t halfperiod);
    int sendSPI_3x8Bit(int OP_hex, int ADDR_hex, int DATA_hex, size_t halfperiod);
    int initializeMCP23S17(int sideA, int sideB , float taktrate);
    int initGPIO(int sclkpin, int cspin, int misopin, int mosipin);
    int unexport();
};



#endif //MY_PROJECT_0_7_SPI_MCP23S17_H
